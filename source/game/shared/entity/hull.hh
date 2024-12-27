// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/box3f.hh"
#include "mathlib/vec3f.hh"

class HullComponent final {
public:
    Box3f local_box {};

public:
    // NOTE: this must be called before
    // transform/velocity update are called
    // because this might as well just set
    // an entity's velocity value to zero
    static void update(void);

public:
    static inline Box3f player(void)
    {
        return Box3f::from_bounds(Vec3f(-0.4f, -0.4f, -0.4f), Vec3f(+0.4f, +0.4f, +0.4f));
    }
};
