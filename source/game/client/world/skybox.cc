// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/world/skybox.hh"

Vec3f skybox::fog_color = Vec3f(0.0f, 0.0f, 0.0f);

void skybox::init(void)
{
    // https://convertingcolors.com/hex-color-B1F3FF.html
    skybox::fog_color = Vec3f(0.690f, 0.950f, 1.000f);
}
