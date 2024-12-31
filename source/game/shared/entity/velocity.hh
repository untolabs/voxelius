// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3angles.hh"

class VelocityComponent final {
public:
    Vec3angles angular {};
    Vec3f linear {};

public:
    // Updates entities TransformComponent values
    // according to velocities multiplied by fixed_frametime.
    // NOTE: This system was previously called inertial
    static void fixed_update(void);
};
