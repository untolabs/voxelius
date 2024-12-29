// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/view.hh"

#include "common/config.hh"

#include "client/entity/player_move.hh"

#include "client/gui/settings.hh"

#include "client/globals.hh"

#include "shared/entity/grounded.hh"
#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"


float view::roll_angle = 2.0f;
float view::vertical_fov = 90.0f;
unsigned int view::max_distance = 16U;

Vec3angles view::angles = {};
Vec3f view::direction = {};
Mat4x4f view::matrix = {};
WorldCoord view::position = {};

static void reset(void)
{
    view::angles = Vec3angles();
    view::direction = Vec3f::dir_forward();
    view::matrix = Mat4x4f::identity();
    view::position = WorldCoord();
}

void view::init(void)
{
    Config::add(globals::client_config, "view.roll_angle", view::roll_angle);
    Config::add(globals::client_config, "view.vertical_fov", view::vertical_fov);
    Config::add(globals::client_config, "view.max_distance", view::max_distance);

    settings::add_slider(1, settings::GENERAL, "view.vertical_fov", view::vertical_fov, 45.0, 120.0f, true, "%.0f");
    settings::add_slider(0, settings::VIDEO, "view.max_distance", view::max_distance, 2U, 32U, false);
    settings::add_slider(10, settings::VIDEO, "view.roll_angle", view::roll_angle, 0.0f, 4.0f, true, "%.01f");

    reset();
}

void view::update(void)
{
    if(!globals::registry.valid(globals::player)) {
        reset();
        return;
    }

    const auto &head = globals::registry.get<HeadComponent>(globals::player);
    const auto &tform = globals::registry.get<TransformComponent>(globals::player);

    view::angles = tform.angles + head.angles;
    view::position = tform.position;
    view::position.local += head.offset;

    // Figure out where the camera is pointed
    Vec3f right_vector;
    Vec3angles::vectors(view::angles, &view::direction, &right_vector, nullptr);

    auto patch_angles = view::angles;
    const auto &velocity = globals::registry.get<VelocityComponent>(globals::player);
    const auto roll = view::roll_angle * Vec3f::dot(velocity.linear / PMOVE_MAX_SPEED_GROUND, right_vector);
    patch_angles[2] = cxpr::radians(-roll);

    const auto z_near = 0.01f;
    const auto z_far = static_cast<float>(CHUNK_SIZE * cxpr::clamp(view::max_distance, 2U, 32U));
    Mat4x4f proj = Mat4x4f::proj_persp(cxpr::radians(view::vertical_fov), globals::aspect, z_near, z_far);
    Mat4x4f view = Mat4x4f::view_psrc(view::position.local, patch_angles);

    view::matrix = Mat4x4f::product(proj, view);
}
