// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/globals.hh"

#include "common/config.hh"

#include "client/gui/gui_screen.hh"


Config globals::client_config = {};

GLFWwindow *globals::window = nullptr;

float globals::window_frametime = 0.0f;
float globals::window_frametime_avg = 0.0f;
std::uint64_t globals::window_frametime_us = UINT64_MAX;
std::uint64_t globals::window_framecount = 0;

std::uint64_t globals::fixed_accumulator = 0;

int globals::width = 0;
int globals::height = 0;
float globals::aspect = 0.0f;

std::size_t globals::num_drawcalls = 0;
std::size_t globals::num_triangles = 0;

ENetHost *globals::client_host = nullptr;

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

ALCdevice *globals::sound_dev = nullptr;
ALCcontext *globals::sound_ctx = nullptr;
