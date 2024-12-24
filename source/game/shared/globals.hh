// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace globals
{
extern float frametime;
extern float frametime_avg;
extern std::uint64_t frametime_us;
extern std::uint64_t curtime;
extern std::uint64_t framecount;

extern entt::dispatcher dispatcher;
extern entt::registry registry;
} // namespace globals
