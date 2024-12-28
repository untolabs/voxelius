// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/box3f.hh"

class CollisionComponent final {
public:
    Box3f hull {};

public:
    // NOTE: this must be called before
    // transform/velocity update are called
    // because this might as well just set
    // an entity's velocity value to zero
    static void update(void);
};
