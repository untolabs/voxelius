// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec4f.hh"

namespace status_lines
{
void init(void);
void layout(void);
} // namespace status_lines

namespace status_lines
{
void set_item(const std::string &text, const Vec4f &color = Vec4f::white(), float fadeout = 5.0f);
} // namespace status_lines
