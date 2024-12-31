// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace globals
{
extern float fixed_frametime;
extern float fixed_frametime_avg;
extern std::uint64_t fixed_frametime_us;
extern std::uint64_t fixed_framecount;

extern std::uint64_t curtime;

extern entt::dispatcher dispatcher;
extern entt::registry registry;
} // namespace globals
