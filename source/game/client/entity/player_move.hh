// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3f.hh"

constexpr static float PMOVE_MAX_SPEED_AIR = 16.0f;
constexpr static float PMOVE_MAX_SPEED_GROUND = 8.0f;
constexpr static float PMOVE_ACCELERATION_AIR = 3.0f;
constexpr static float PMOVE_ACCELERATION_GROUND = 6.0f;
constexpr static float PMOVE_FRICTION_GROUND = 10.0f;

namespace player_move
{
void init(void);
void fixed_update(void);
} // namespace player_move

namespace player_move
{
void set_direction(const Vec3f &direction);
} // namespace player_move
