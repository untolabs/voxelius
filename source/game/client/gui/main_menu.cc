// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/main_menu.hh"

#include "cmake/config.hh"

#include "mathlib/constexpr.hh"

#include "client/event/language_set.hh"
#include "client/event/glfw_key.hh"

#include "client/gui/gui_screen.hh"

#include "client/resource/texture2D.hh"

#include "client/globals.hh"
#include "client/session.hh"


constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

static std::string str_play = {};
static std::string str_resume = {};
static std::string str_settings = {};
static std::string str_leave = {};
static std::string str_quit = {};

static std::shared_ptr<const Texture2D> title = nullptr;
static float title_aspect = 0.0f;

static void on_glfw_key(const GlfwKeyEvent &event)
{
    if((event.key == GLFW_KEY_ESCAPE) && (event.action == GLFW_PRESS)) {
        if(globals::registry.valid(globals::player)) {
            if(globals::gui_screen == GUI_SCREEN_NONE) {
                globals::gui_screen = GUI_MAIN_MENU;
                return;
            }

            if(globals::gui_screen == GUI_MAIN_MENU) {
                globals::gui_screen = GUI_SCREEN_NONE;
                return;
            }
        }
    }
}

static void on_language_set(const LanguageSetEvent &event)
{
    str_play = language::resolve_ui("main_menu.play");
    str_resume = language::resolve_ui("main_menu.resume");
    str_settings = language::resolve("main_menu.settings");
    str_leave = language::resolve("main_menu.leave");
    str_quit = language::resolve("main_menu.quit");
}

static void do_disconnect(void)
{

}

void main_menu::init(void)
{
    title = resource::load<Texture2D>("textures/gui/voxelius.png", TEXTURE2D_LOAD_CLAMP_S | TEXTURE2D_LOAD_CLAMP_T);

    if(!title) {
        spdlog::critical("main_menu: texture load failed");
        std::terminate();
    }

    title_aspect = static_cast<float>(title->width) / static_cast<float>(title->height);

    globals::dispatcher.sink<GlfwKeyEvent>().connect<&on_glfw_key>();
    globals::dispatcher.sink<LanguageSetEvent>().connect<&on_language_set>();
}

void main_menu::deinit(void)
{
    title = nullptr;
}

void main_menu::layout(void)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 window_start = ImVec2(0.0f, viewport->Size.y * 0.15f);
    const ImVec2 window_size = ImVec2(viewport->Size.x, viewport->Size.y);

    ImGui::SetNextWindowPos(window_start);
    ImGui::SetNextWindowSize(window_size);

    if(ImGui::Begin("###main_menu", nullptr, WINDOW_FLAGS)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 2.0f * globals::gui_scale));

        if(!globals::registry.valid(globals::player)) {
            const float reference_height = 0.225f * window_size.y;
            const float image_width = cxpr::min(window_size.x, reference_height * title_aspect);
            const float image_height = image_width / title_aspect;
            ImGui::SetCursorPosX(0.5f * (window_size.x - image_width));
            ImGui::Image(title->imgui, ImVec2(image_width, image_height));
        }
        else {
            ImGui::Dummy(ImVec2(0.0f, 32.0f * globals::gui_scale));
        }

        ImGui::Dummy(ImVec2(0.0f, 24.0f * globals::gui_scale));

        const float button_width = 240.0f * globals::gui_scale;
        const float button_xpos = 0.5f * (window_size.x - button_width);

        if(globals::registry.valid(globals::player)) {
            ImGui::SetCursorPosX(button_xpos);
            if(ImGui::Button(str_resume.c_str(), ImVec2(button_width, 0.0f)))
                globals::gui_screen = GUI_SCREEN_NONE;
            ImGui::Spacing();
        }
        else {
            ImGui::SetCursorPosX(button_xpos);
            if(ImGui::Button(str_play.c_str(), ImVec2(button_width, 0.0f)))
                globals::gui_screen = GUI_PLAY_MENU;
            ImGui::Spacing();
        }

        ImGui::SetCursorPosX(button_xpos);
        if(ImGui::Button(str_settings.c_str(), ImVec2(button_width, 0.0f)))
            globals::gui_screen = GUI_SETTINGS;
        ImGui::Spacing();

        if(globals::registry.valid(globals::player)) {
            ImGui::SetCursorPosX(button_xpos);
            if(ImGui::Button(str_leave.c_str(), ImVec2(button_width, 0.0f))) {
                if(globals::is_singleplayer)
                    session::sp::unload_world();
                else session::mp::disconnect("protocol.client_disconnect");
            
                globals::gui_screen = GUI_PLAY_MENU;
            }

            ImGui::Spacing();
        }
        else {
            ImGui::SetCursorPosX(button_xpos);
            if(ImGui::Button(str_quit.c_str(), ImVec2(button_width, 0.0f)))
                glfwSetWindowShouldClose(globals::window, true);
            ImGui::Spacing();            
        }

        if(!globals::registry.valid(globals::player)) {
            const ImVec2 &padding = ImGui::GetStyle().FramePadding;
            const ImVec2 &spacing = ImGui::GetStyle().ItemSpacing;

            ImGui::PushFont(globals::font_debug);
            ImGui::SetCursorScreenPos(ImVec2(padding.x + spacing.x, window_size.y - globals::font_debug->FontSize - padding.y - spacing.y));
            ImGui::Text("Voxelius %s", PROJECT_VERSION_STRING);
            ImGui::PopFont();
        }

        ImGui::PopStyleVar();
    }

    ImGui::End();
}
