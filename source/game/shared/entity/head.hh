// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3angles.hh"

struct HeadComponent final {
    Vec3angles angles {};
    Vec3f offset {};
};
