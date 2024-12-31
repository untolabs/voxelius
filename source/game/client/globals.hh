// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/globals.hh"

class Config;

struct GLFWwindow;
struct ImFont;

namespace globals
{
extern Config client_config;

extern GLFWwindow *window;

// Some gamesystems that aren't really
// gameplay-oriented might still use client
// framerate to interpolate discrete things
// so it's still a good idea to keep these available
extern float window_frametime;
extern float window_frametime_avg;
extern std::uint64_t window_frametime_us;
extern std::uint64_t window_framecount;

// https://gafferongames.com/post/fix_your_timestep/
extern std::uint64_t fixed_accumulator;

extern int width;
extern int height;
extern float aspect;

extern std::size_t num_drawcalls;
extern std::size_t num_triangles;

extern ENetHost *client_host;

extern bool is_singleplayer;

extern entt::entity player;

extern GLuint world_fbo;
extern GLuint world_fbo_color;
extern GLuint world_fbo_depth;

extern ImFont *font_debug;
extern ImFont *font_default;
extern ImFont *font_chat;

extern int *gui_keybind_ptr;
extern unsigned int gui_scale;
extern unsigned int gui_screen;
} // namespace globals
