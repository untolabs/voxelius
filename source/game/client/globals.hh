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


extern float frametime;
extern float frametime_avg;
extern std::uint64_t frametime_us;
extern std::uint64_t curtime;
extern std::uint64_t framecount;

extern int width;
extern int height;
extern float aspect;

extern std::size_t num_drawcalls;
extern std::size_t num_triangles;

extern ENetHost *client_host;

extern ENetPeer *session_peer;
extern std::uint16_t session_index;
extern std::uint64_t session_identity;
extern std::uint64_t session_tick_delta;
extern std::uint64_t session_next_transmit;

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
