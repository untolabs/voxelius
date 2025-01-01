// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/background.hh"

#include "mathlib/constexpr.hh"

#include "client/resource/texture2D.hh"

#include "client/globals.hh"


static std::shared_ptr<const Texture2D> texture = nullptr;

void background::init(void)
{
    texture = resource::load<Texture2D>("textures/gui/background.png", TEXTURE2D_LOAD_VFLIP);

    if(!texture) {
        spdlog::critical("background: texture load failed");
        std::terminate();
    }
}

void background::deinit(void)
{
    texture = nullptr;
}

void background::layout(void)
{
    auto viewport = ImGui::GetMainViewport();
    auto draw_list = ImGui::GetBackgroundDrawList();

    auto scaled_width = 0.75f * static_cast<float>(globals::width / globals::gui_scale);
    auto scaled_height = 0.75f * static_cast<float>(globals::height / globals::gui_scale);
    auto scale_uv = ImVec2(scaled_width / static_cast<float>(texture->width), scaled_height / static_cast<float>(texture->height));
    draw_list->AddImage(texture->imgui, ImVec2(0.0f, 0.0f), viewport->Size, ImVec2(0.0f, 0.0f), scale_uv);
}
