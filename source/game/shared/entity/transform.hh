// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3angles.hh"
#include "shared/world/world_coord.hh"

class TransformComponent final {
public:
    Vec3angles angles {};
    WorldCoord position {};

public:
    // Updates TransformComponent values so that
    // the local part of WorldCoord field is always
    // within a single chunk - floating point precision fixes
    static void update(void);
};
