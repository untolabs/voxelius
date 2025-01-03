// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "common/resource/resource.hh"

constexpr static unsigned int IMAGE_LOAD_VFLIP      = 0x0001;
constexpr static unsigned int IMAGE_LOAD_GRAYSCALE  = 0x0002;

struct Image final : public Resource {
    void *pixels;
    int height;
    int width;
};
