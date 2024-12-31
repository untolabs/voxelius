// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/box3f.hh"

class CollisionComponent final {
public:
    Box3f hull {};

public:
    // NOTE: CollisionComponent::fixed_update must be called
    // before TransformComponent::fixed_update and VelocityComponent::fixed_update
    // because both transform and velocity may be updated internally
    static void fixed_update(void);
};
