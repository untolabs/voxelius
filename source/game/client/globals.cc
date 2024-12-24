// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/globals.hh"

#include "common/config.hh"

#include "client/gui/gui_screen.hh"


Config globals::client_config = {};

GLFWwindow *globals::window = nullptr;

int globals::width = 0;
int globals::height = 0;
float globals::aspect = 0.0f;

std::size_t globals::num_drawcalls = 0;
std::size_t globals::num_triangles = 0;

ENetHost *globals::client_host = nullptr;

ENetPeer *globals::session_peer = nullptr;
std::uint16_t globals::session_index = UINT16_MAX;
std::uint64_t globals::session_identity = UINT64_MAX;
std::uint64_t globals::session_tick_delta = UINT64_MAX;
std::uint64_t globals::session_next_transmit = UINT64_MAX;

bool globals::is_singleplayer = true;

entt::entity globals::player = entt::null;

GLuint globals::world_fbo = 0;
GLuint globals::world_fbo_color = 0;
GLuint globals::world_fbo_depth = 0;

ImFont *globals::font_debug = nullptr;
ImFont *globals::font_default = nullptr;
ImFont *globals::font_chat = nullptr;

int *globals::gui_keybind_ptr = nullptr;
unsigned int globals::gui_scale = 0U;
unsigned int globals::gui_screen = GUI_SCREEN_NONE;
