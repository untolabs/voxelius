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

#include "client/globals.hh"


static float prev_speed_xz = 0.0f;
static bool enable_speedometer = true;
static Vec3f pmove_wish_dir = Vec3f::zero();

static Vec3f accelerate(const Vec3f &wish_dir, const Vec3f &velocity, float wish_speed, float accel)
{
    const auto current_speed = Vec3f::dot(velocity, wish_dir);
    const auto add_speed = wish_speed - current_speed;

    if(add_speed <= 0.0f) {
        // Not accelerating
        return velocity;
    }

    const auto accel_speed = cxpr::min(add_speed, accel * globals::frametime * wish_speed);

    auto result = Vec3f(velocity);
    result[0] += accel_speed * wish_dir[0];
    result[2] += accel_speed * wish_dir[2];
    return std::move(result);
}

static Vec3f air_move(const Vec3f &wish_dir, const Vec3f &velocity)
{
    return accelerate(wish_dir, velocity, PMOVE_ACCELERATION_AIR, PMOVE_MAX_SPEED_AIR);
}

static Vec3f ground_move(const Vec3f &wish_dir, const Vec3f &velocity)
{
    if(const auto speed = Vec3f::length(velocity)) {
        const auto speed_drop = speed * PMOVE_FRICTION_GROUND * globals::frametime;
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
    settings::add_checkbox(2, settings::GENERAL, "player_move.enable_speedometer", enable_speedometer, true);
}

void player_move::update(void)
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

    if(is_grounded && (pmove_wish_dir.get_y() > 0.0f)) {
        velocity.linear[1] = GravityComponent::acceleration;

        if(enable_speedometer) {
            const auto new_speed_xz = Vec2f::length(Vec2f(velocity.linear.get_x(), velocity.linear.get_z()));
            const auto new_speed_text = fmt::format("{:.02f} M/S", new_speed_xz);
            const auto increase = new_speed_xz - prev_speed_xz;

            if(cxpr::abs(increase) < 0.01f) {
                // No speed increase, use the gray color
                status_lines::set(STATUS_DEBUG, new_speed_text, Vec4f::light_gray(), 1.0f);
            }
            else if(increase < 0.0f) {
                // We're slowing down, use the red color
                status_lines::set(STATUS_DEBUG, new_speed_text, Vec4f::red(), 1.0f);
            }
            else {
                // We're speeding up, use the green color
                status_lines::set(STATUS_DEBUG, new_speed_text, Vec4f::green(), 1.0f);
            }

            prev_speed_xz = new_speed_xz;
        }
    }
}

void player_move::set_direction(const Vec3f &wish_dir)
{
    pmove_wish_dir = wish_dir;
}
