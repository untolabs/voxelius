// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3f.hh"

class GravityComponent final {
public:
    static float acceleration;

public:
    static void fixed_update(void);
};
