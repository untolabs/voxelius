// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/entity/player_look.hh"

#include "shared/entity/head.hh"

#include "client/globals.hh"


constexpr static float PITCH_MIN = -1.0f * cxpr::radians(90.0f);
constexpr static float PITCH_MAX = +1.0f * cxpr::radians(90.0f);

void player_look::add_angles(float pitch_d, float yaw_d)
{
    if(globals::registry.valid(globals::player)) {
        auto &head = globals::registry.get<HeadComponent>(globals::player);
        head.angles[0] += pitch_d;
        head.angles[1] += yaw_d;
        head.angles[0] = cxpr::clamp(head.angles[0], PITCH_MIN, PITCH_MAX);
        head.angles = Vec3angles::wrap_180(head.angles);
    }
}
