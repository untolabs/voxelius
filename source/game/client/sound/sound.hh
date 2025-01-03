// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace sound
{
extern float master_volume;
extern float effects_volume;
extern float music_volume;
} // namespace sound

namespace sound
{
void init(void);
void init_late(void);
void deinit(void);
void update(void);
void render(void);
} // namespace sound

namespace sound
{
void play_generic(const std::string &sound, bool looping, float pitch);
void play_entity(entt::entity entity, const std::string &sound, bool looping, float pitch);
void play_player(const std::string &sound, bool looping, float pitch);
} // namespace sound
