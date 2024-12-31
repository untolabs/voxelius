// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3angles.hh"

struct HeadComponent {
    Vec3angles angles {};
    Vec3f position {};
};

// Clientside-only: interpolation
// HeadComponentIntr is the interpolated state
// HeadComponentPrev is the previously received state
struct HeadComponentIntr final : public HeadComponent {};
struct HeadComponentPrev final : public HeadComponent {};
