// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace server_game
{
extern unsigned int view_distance;
extern std::uint64_t password_hash;
} // namespace server_game

namespace server_game
{
void init(void);
void init_late(void);
void deinit(void);
void fixed_update(void);
void fixed_update_late(void);
} // namespace server_game
