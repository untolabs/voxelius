// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec4f.hh"

constexpr static unsigned int STATUS_DEBUG  = 0x0000; // generic debug line
constexpr static unsigned int STATUS_HOTBAR = 0x0001; // hotbar item line
constexpr static unsigned int STATUS_COUNT  = 0x0002;

namespace status_lines
{
void init(void);
void init_late(void);
void layout(void);
} // namespace status_lines

namespace status_lines
{
void set(unsigned int line, const std::string &text, const Vec4f &color, float fadeout);
void unset(unsigned int line);
} // namespace status_lines
