// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace client_game
{
extern bool streamer_mode;
extern bool vertical_sync;
extern bool world_curvature;
extern unsigned int pixel_size;
extern unsigned int fog_mode;
extern std::string username;
} // namespace client_game

namespace client_game
{
void init(void);
void init_late(void);
void deinit(void);
void update(void);
void update_late(void);
void render(void);
void layout(void);
} // namespace client_game
