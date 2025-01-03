// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/entity/player_move.hh"

#include "cmake/config.hh"

#include "common/config.hh"

#include "mathlib/constexpr.hh"
#include "mathlib/vec2f.hh"

#include "shared/entity/gravity.hh"
#include "shared/entity/grounded.hh"
#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "client/gui/gui_screen.hh"
#include "client/gui/settings.hh"

#include "client/hud/status_lines.hh"

#include "client/resource/sound_effect.hh"

#include "client/sound/sound.hh"

#include "client/globals.hh"


constexpr static std::uint64_t JUMP_COOLDOWN = 500000; // 0.5 seconds

static float prev_speed_xz = 0.0f;
static bool enable_speedometer = true;
static Vec3f pmove_wish_dir = Vec3f::zero();
static std::uint64_t next_jump = UINT64_C(0);

static std::shared_ptr<const SoundEffect> sfx_jump = nullptr;

static Vec3f accelerate(const Vec3f &wish_dir, const Vec3f &velocity, float wish_speed, float accel)
{
    const auto current_speed = Vec3f::dot(velocity, wish_dir);
    const auto add_speed = wish_speed - current_speed;

    if(add_speed <= 0.0f) {
        // Not accelerating
        return velocity;
    }

    const auto accel_speed = cxpr::min(add_speed, accel * globals::fixed_frametime * wish_speed);

    auto result = Vec3f(velocity);
    result[0] += accel_speed * wish_dir[0];
    result[2] += accel_speed * wish_dir[2];
    return result;
}

static Vec3f air_move(const Vec3f &wish_dir, const Vec3f &velocity)
{
    return accelerate(wish_dir, velocity, PMOVE_ACCELERATION_AIR, PMOVE_MAX_SPEED_AIR);
}

static Vec3f ground_move(const Vec3f &wish_dir, const Vec3f &velocity)
{
    if(const auto speed = Vec3f::length(velocity)) {
        const auto speed_drop = speed * PMOVE_FRICTION_GROUND * globals::fixed_frametime;
        const auto speed_factor = cxpr::max(speed - speed_drop, 0.0f) / speed;
        return accelerate(wish_dir, velocity * speed_factor, PMOVE_ACCELERATION_GROUND, PMOVE_MAX_SPEED_GROUND);
    }

    return accelerate(wish_dir, velocity, PMOVE_ACCELERATION_GROUND, PMOVE_MAX_SPEED_GROUND);
}

void player_move::init(void)
{
    prev_speed_xz = 0.0f;
    pmove_wish_dir = Vec3f::zero();

    Config::add(globals::client_config, "player_move.enable_speedometer", enable_speedometer);
    settings::add_checkbox(2, settings::VIDEO_GUI, "player_move.enable_speedometer", enable_speedometer, true);

    sfx_jump = resource::load<SoundEffect>("sounds/jump.wav");
}

void player_move::deinit(void)
{
    sfx_jump = nullptr;
}

void player_move::fixed_update(void)
{
    if(!globals::registry.valid(globals::player)) {
        // There's no point in updating movement
        // while we're not loaded into a world
        pmove_wish_dir = Vec3f::zero();
        return;
    }

    if(globals::gui_screen) {
        // UI is active - player movement should not
        // update; we at least want to decelerate
        pmove_wish_dir = Vec3f::zero();
    }

    const auto &head = globals::registry.get<HeadComponent>(globals::player);
    auto &transform = globals::registry.get<TransformComponent>(globals::player);
    auto &velocity = globals::registry.get<VelocityComponent>(globals::player);

    // Interpolation - preserve current component states
    globals::registry.emplace_or_replace<TransformComponentPrev>(globals::player, transform);

    Vec3f forward, right;
    Vec3angles::vectors(Vec3angles(0.0f, head.angles[1], 0.0f), &forward, &right, nullptr);

    Vec3f wish_dir = Vec3f::zero();
    Vec3f move_vars_xz = Vec3f(pmove_wish_dir.get_x(), 0.0f, pmove_wish_dir.get_z());
    wish_dir.set_x(Vec3f::dot(move_vars_xz, right));
    wish_dir.set_z(Vec3f::dot(move_vars_xz, forward));

    const auto is_grounded = globals::registry.any_of<GroundedComponent>(globals::player);
    const auto velocity_xz = Vec3f(velocity.linear.get_x(), 0.0f, velocity.linear.get_z());

    if(is_grounded) {
        const auto xz = ground_move(wish_dir, velocity_xz);
        velocity.linear.set_x(xz.get_x());
        velocity.linear.set_z(xz.get_z());
    }
    else {
        const auto xz = air_move(wish_dir, velocity_xz);
        velocity.linear.set_x(xz.get_x());
        velocity.linear.set_z(xz.get_z());
    }

    if(pmove_wish_dir.get_y() == 0.0f) {
        // Allow players to spam the jump key to
        // bunnyhop, otherwise jumping is done on a cooldown
        next_jump = UINT64_C(0);
        return;
    }

    if(is_grounded && (pmove_wish_dir.get_y() > 0.0f) && (globals::curtime >= next_jump)) {
        velocity.linear.set_y(GravityComponent::acceleration * 0.275f);

        auto new_speed_xz = Vec2f::length(Vec2f(velocity.linear.get_x(), velocity.linear.get_z()));
        auto new_speed_text = fmt::format("{:.02f} M/S", new_speed_xz);
        auto speed_change_xz = new_speed_xz - prev_speed_xz;

        prev_speed_xz = new_speed_xz;
        next_jump = globals::curtime + JUMP_COOLDOWN;

        if(enable_speedometer) {
            if(cxpr::abs(speed_change_xz) < 0.01f) {
                // No considerable speed increase within
                // the precision we use to draw the speedometer
                status_lines::set(STATUS_DEBUG, new_speed_text, Vec4f::light_gray(), 1.0f);
            }
            else if(speed_change_xz < 0.0f) {
                // Speed change is negative, we are actively
                // slowing down; use the red color for the status line
                status_lines::set(STATUS_DEBUG, new_speed_text, Vec4f::red(), 1.0f);
            }
            else {
                // Speed change is positive, we are actively
                // speeding up; use the green color for the status line
                status_lines::set(STATUS_DEBUG, new_speed_text, Vec4f::green(), 1.0f);
            }
        }

        if(sfx_jump != nullptr) {
            sound::play_player(sfx_jump->name, false, 1.0f, 1.0f);
        }
    }
}

void player_move::set_direction(const Vec3f &wish_dir)
{
    pmove_wish_dir = wish_dir;
}
