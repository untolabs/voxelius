// SPDX-License-Identifier: BSD-2-Clause
#pragma once

#include "common/resource/resource.hh"

constexpr static unsigned int TEXTURE2D_LOAD_CLAMP_S    = 0x0001;
constexpr static unsigned int TEXTURE2D_LOAD_CLAMP_T    = 0x0002;
constexpr static unsigned int TEXTURE2D_LOAD_LINEAR_MAG = 0x0004;
constexpr static unsigned int TEXTURE2D_LOAD_LINEAR_MIN = 0x0008;
constexpr static unsigned int TEXTURE2D_LOAD_VFLIP      = 0x0010;

struct Texture2D final {
    ImTextureID imgui;
    GLuint handle;
    int height;
    int width;
};
