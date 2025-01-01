// SPDX-License-Identifier: BSD-2-Clause
// Copyright (C) 2024, untodesu
#include "client/precompiled.hh"
#include "client/hud/crosshair.hh"

#include "mathlib/constexpr.hh"

#include "client/resource/texture2D.hh"

#include "client/globals.hh"


static std::shared_ptr<const Texture2D> texture = nullptr;

void crosshair::init(void)
{
    texture = resource::load<Texture2D>("textures/hud/crosshair.png", TEXTURE2D_LOAD_CLAMP_S | TEXTURE2D_LOAD_CLAMP_T | TEXTURE2D_LOAD_VFLIP);

    if(!texture) {
        spdlog::critical("crosshair: texture load failed");
        std::terminate();
    }
}

void crosshair::deinit(void)
{
    texture = nullptr;
}

void crosshair::layout(void)
{    
    auto viewport = ImGui::GetMainViewport();
    auto draw_list = ImGui::GetForegroundDrawList();

    auto scaled_width = cxpr::max<int>(texture->width, globals::gui_scale * texture->width / 2);
    auto scaled_height = cxpr::max<int>(texture->height, globals::gui_scale * texture->height / 2);
    auto start = ImVec2(static_cast<int>(0.5f * viewport->Size.x) - (scaled_width / 2), static_cast<int>(0.5f * viewport->Size.y) - (scaled_height / 2));
    auto end = ImVec2(start.x + scaled_width, start.y + scaled_height);
    draw_list->AddImage(texture->imgui, start, end);
}
