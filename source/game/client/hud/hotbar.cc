// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/hud/hotbar.hh"

#include "common/config.hh"

#include "shared/world/item_def.hh"

#include "client/event/glfw_key.hh"
#include "client/event/glfw_scroll.hh"

#include "client/gui/settings.hh"

#include "client/hud/status_lines.hh"

#include "client/resource/texture2D.hh"

#include "client/globals.hh"


constexpr static float ITEM_SIZE = 20.0f;
constexpr static float ITEM_PADDING = 2.0f;
constexpr static float SELECTOR_PADDING = 1.0f;
constexpr static float HOTBAR_PADDING = 2.0f;

constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

unsigned int hotbar::active_slot = 0U;
ItemID hotbar::slots[HOTBAR_SIZE] = {};

static int hotbar_keys[HOTBAR_SIZE];

static std::shared_ptr<const Texture2D> hotbar_background = nullptr;
static std::shared_ptr<const Texture2D> hotbar_selector = nullptr;

static ImU32 get_color_alpha(ImGuiCol style_color, float alpha)
{
    const ImVec4 &color = ImGui::GetStyleColorVec4(style_color);
    return ImGui::GetColorU32(ImVec4(color.x, color.y, color.z, alpha));
}

static void update_hotbar_item(void)
{
    if(hotbar::slots[hotbar::active_slot] == NULL_VOXEL) {
        status_lines::unset(STATUS_HOTBAR);
        return;
    }

    if(const ItemInfo *info = item_def::find(hotbar::slots[hotbar::active_slot])) {
        status_lines::set(STATUS_HOTBAR, info->name, Vec4f::white(), 5.0f);
        return;
    }
}

static void on_glfw_key(const GlfwKeyEvent &event)
{
    if((event.action == GLFW_PRESS) && !globals::gui_screen) {
        for(unsigned int i = 0U; i < HOTBAR_SIZE; ++i) {
            if(event.key == hotbar_keys[i]) {
                hotbar::active_slot = i;
                update_hotbar_item();
                return;
            }
        }
    }
}

static void on_glfw_scroll(const GlfwScrollEvent &event)
{
    if(!globals::gui_screen) {
        if(event.dy < 0.0) {
            hotbar::active_slot += 1U;
            hotbar::active_slot %= HOTBAR_SIZE;
            update_hotbar_item();
            return;
        }

        if(event.dy > 0.0) {
            hotbar::active_slot += HOTBAR_SIZE - 1U;
            hotbar::active_slot %= HOTBAR_SIZE;
            update_hotbar_item();
            return;
        }
    }
}

void hotbar::init(void)
{
    hotbar_keys[0] = GLFW_KEY_1;
    hotbar_keys[1] = GLFW_KEY_2;
    hotbar_keys[2] = GLFW_KEY_3;
    hotbar_keys[3] = GLFW_KEY_4;
    hotbar_keys[4] = GLFW_KEY_5;
    hotbar_keys[5] = GLFW_KEY_6;
    hotbar_keys[6] = GLFW_KEY_7;
    hotbar_keys[7] = GLFW_KEY_8;
    hotbar_keys[8] = GLFW_KEY_9;

    Config::add(globals::client_config, "hotbar.key.0", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.1", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.3", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.4", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.5", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.6", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.7", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.8", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.9", hotbar_keys[0]);

    settings::add_key_binding(10, settings::KEYBOARD_GAMEPLAY, "hotbar.key.0", hotbar_keys[0]);
    settings::add_key_binding(11, settings::KEYBOARD_GAMEPLAY, "hotbar.key.1", hotbar_keys[1]);
    settings::add_key_binding(12, settings::KEYBOARD_GAMEPLAY, "hotbar.key.2", hotbar_keys[2]);
    settings::add_key_binding(13, settings::KEYBOARD_GAMEPLAY, "hotbar.key.3", hotbar_keys[3]);
    settings::add_key_binding(14, settings::KEYBOARD_GAMEPLAY, "hotbar.key.4", hotbar_keys[4]);
    settings::add_key_binding(15, settings::KEYBOARD_GAMEPLAY, "hotbar.key.5", hotbar_keys[5]);
    settings::add_key_binding(16, settings::KEYBOARD_GAMEPLAY, "hotbar.key.6", hotbar_keys[6]);
    settings::add_key_binding(17, settings::KEYBOARD_GAMEPLAY, "hotbar.key.7", hotbar_keys[7]);
    settings::add_key_binding(18, settings::KEYBOARD_GAMEPLAY, "hotbar.key.8", hotbar_keys[8]);

    hotbar_background = resource::load<Texture2D>("textures/hud/hotbar_background.png", TEXTURE2D_LOAD_CLAMP_S | TEXTURE2D_LOAD_CLAMP_T);
    hotbar_selector = resource::load<Texture2D>("textures/hud/hotbar_selector.png", TEXTURE2D_LOAD_CLAMP_S | TEXTURE2D_LOAD_CLAMP_T);

    globals::dispatcher.sink<GlfwKeyEvent>().connect<&on_glfw_key>();
    globals::dispatcher.sink<GlfwScrollEvent>().connect<&on_glfw_scroll>();
}

void hotbar::deinit(void)
{
    hotbar_background = nullptr;
    hotbar_selector = nullptr;
}

void hotbar::layout(void)
{
    ImGui::PushFont(globals::font_chat);

    ImGuiStyle &style = ImGui::GetStyle();

    const float item_size = ITEM_SIZE * globals::gui_scale;
    const float hotbar_width = HOTBAR_SIZE * item_size;
    const float hotbar_padding = HOTBAR_PADDING * globals::gui_scale;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 window_start = ImVec2(0.5f * viewport->Size.x - 0.5f * hotbar_width, viewport->Size.y - item_size - hotbar_padding);
    const ImVec2 window_size = ImVec2(hotbar_width, item_size);

    ImGui::SetNextWindowPos(window_start);
    ImGui::SetNextWindowSize(window_size);

    if(!ImGui::Begin("###HUD_Hotbar", nullptr, WINDOW_FLAGS)) {
        ImGui::End();
        ImGui::PopFont();
        return;
    }

    ImDrawList *draw_list = ImGui::GetForegroundDrawList();

    // Draw the hotbar background image
    const auto background_start = ImVec2(window_start.x, window_start.y);
    const auto background_end = ImVec2(background_start.x + window_size.x, background_start.y + window_size.y);
    draw_list->AddImage(hotbar_background->imgui, background_start, background_end);

    // Draw the hotbar selector image
    const auto selector_padding_a = SELECTOR_PADDING * globals::gui_scale;
    const auto selector_padding_b = SELECTOR_PADDING * globals::gui_scale * 2.0f;
    const auto selector_start = ImVec2(window_start.x + hotbar::active_slot * item_size - selector_padding_a, window_start.y - selector_padding_a);
    const auto selector_end = ImVec2(selector_start.x + item_size + selector_padding_b, selector_start.y + item_size + selector_padding_b);
    draw_list->AddImage(hotbar_selector->imgui, selector_start, selector_end);

    // Figure out item texture padding values
    const auto item_padding_a = ITEM_PADDING * globals::gui_scale;
    const auto item_padding_b = ITEM_PADDING * globals::gui_scale * 2.0f;

    // Draw individual item textures in the hotbar
    for(std::size_t i = 0; i < HOTBAR_SIZE; ++i) {
        const auto info = item_def::find(hotbar::slots[i]);

        if((info == nullptr) || (info->cached_texture == nullptr)) {
            // There's either no item in the slot
            // or the item doesn't have a texture
            continue;
        }

        const auto item_start = ImVec2(window_start.x + i * item_size + item_padding_a, window_start.y + item_padding_a);
        const auto item_end = ImVec2(item_start.x + item_size - item_padding_b, item_start.y + item_size - item_padding_b);
        draw_list->AddImage(info->cached_texture->imgui, item_start, item_end);
    }

    ImGui::End();
    ImGui::PopFont();
}
