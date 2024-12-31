// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3angles.hh"

struct HeadComponent {
    Vec3angles angles {};
    Vec3f offset {};
};

// Clientside-only: interpolation
// HeadComponentIntr is the interpolated state
// HeadComponentPrev is the previously received state
class HeadComponentIntr final : public HeadComponent {};
class HeadComponentPrev final : public HeadComponent {};
