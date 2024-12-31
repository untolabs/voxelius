// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/globals.hh"


float globals::fixed_frametime = 0.0f;
float globals::fixed_frametime_avg = 0.0f;
std::uint64_t globals::fixed_frametime_us = UINT64_MAX;
std::uint64_t globals::fixed_framecount = 0;

std::uint64_t globals::curtime = 0;

entt::dispatcher globals::dispatcher = {};
entt::registry globals::registry = {};
