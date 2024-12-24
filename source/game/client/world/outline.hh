// SPDX-License-Identifier: BSD-2-Clause
// Copyright (C) 2024, untodesu
#pragma once
#include "mathlib/mat4x4f.hh"
#include "mathlib/vec4f.hh"
#include "shared/world/world_coord.hh"

namespace outline
{
void init(void);
void deinit(void);
void prepare(void);
void cube(const WorldCoord &offset, const Vec3f &size, float thickness = 1.0f, const Vec4f &color = Vec4f::cyan());
void line(const WorldCoord &offset, const Vec3f &line, float thickness = 1.0f, const Vec4f &color = Vec4f::cyan());
} // namespace outline
