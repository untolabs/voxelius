// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/splash.hh"

#include "mathlib/constexpr.hh"

#include "common/epoch.hh"
#include "common/cmdline.hh"
#include "common/fstools.hh"

#include "client/event/glfw_key.hh"
#include "client/event/glfw_mouse_button.hh"
#include "client/event/glfw_scroll.hh"

#include "client/gui/language.hh"

#include "client/resource/texture2D.hh"

#include "client/globals.hh"


constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

constexpr static int SPLASH_COUNT = 4;
constexpr static std::size_t DELAY_MICROSECONDS = 2000000;
constexpr static const char *SPLASH_FORMAT = "textures/gui/untolabs.{}.png";

static std::shared_ptr<const Texture2D> texture = nullptr;
static float texture_aspect;
static float texture_alpha;

static std::uint64_t end_time;

static void on_glfw_key(const GlfwKeyEvent &event)
{
    end_time = UINT64_C(0);
}

static void on_glfw_mouse_button(const GlfwMouseButtonEvent &event)
{
    end_time = UINT64_C(0);
}

static void on_glfw_scroll(const GlfwScrollEvent &event)
{
    end_time = UINT64_C(0);
}

void splash::init(void)
{
    if(cmdline::contains("nosplash")) {
        texture = nullptr;
        texture_aspect = 0.0f;
        texture_alpha = 0.0f;
        return;
    }

    std::random_device randev = {};
    std::uniform_int_distribution<int> dist(0, SPLASH_COUNT - 1);
    std::string path = fmt::format(SPLASH_FORMAT, dist(randev));

    texture = resource::load<Texture2D>(path, TEXTURE2D_LOAD_CLAMP_S | TEXTURE2D_LOAD_CLAMP_T);
    texture_aspect = 0.0f;
    texture_alpha = 0.0f;

    if(texture) {
        texture_aspect = static_cast<float>(texture->width) / static_cast<float>(texture->height);
        texture_alpha = 1.0f;
    }
}

void splash::init_late(void)
{
    if(!texture) {
        // We don't have to waste time
        // rendering the missing splash texture
        return;
    }

    end_time = epoch::microseconds() + DELAY_MICROSECONDS;

    globals::dispatcher.sink<GlfwKeyEvent>().connect<&on_glfw_key>();
    globals::dispatcher.sink<GlfwMouseButtonEvent>().connect<&on_glfw_mouse_button>();
    globals::dispatcher.sink<GlfwScrollEvent>().connect<&on_glfw_scroll>();

    const std::string skip_status = language::resolve("splash.skip_status");

    while(!glfwWindowShouldClose(globals::window)) {
        const std::uint64_t curtime = epoch::microseconds();
        const std::uint64_t remains = end_time - curtime;

        if(curtime >= end_time)
            break;

        texture_alpha = cxpr::smoothstep(0.15f, 0.5f, static_cast<float>(remains) / static_cast<float>(DELAY_MICROSECONDS));

        splash::render(skip_status);
    }

    globals::dispatcher.sink<GlfwKeyEvent>().disconnect<&on_glfw_key>();
    globals::dispatcher.sink<GlfwMouseButtonEvent>().disconnect<&on_glfw_mouse_button>();
    globals::dispatcher.sink<GlfwScrollEvent>().disconnect<&on_glfw_scroll>();

    texture = nullptr;
    texture_aspect = 0.0f;
    texture_alpha = 0.0f;
    end_time = UINT64_C(0);
}

void splash::render(const std::string &status)
{
    if(!texture) {
        // We don't have to waste time
        // rendering the missing splash texture
        return;
    }

    // The splash is rendered outside the main
    // render loop, so we have to manually begin
    // and render both window and ImGui frames
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    const ImVec2 window_start = ImVec2(0.0f, 0.0f);
    const ImVec2 window_size = ImVec2(viewport->Size.x, viewport->Size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, viewport->Size.x, viewport->Size.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::SetNextWindowPos(window_start);
    ImGui::SetNextWindowSize(window_size);

    if(ImGui::Begin("###splash", nullptr, WINDOW_FLAGS)) {
        const float image_width = 0.80f * viewport->Size.x;
        const float image_height = image_width / texture_aspect;
        const ImVec2 image_size = ImVec2(image_width, image_height);

        const float image_x = 0.5f * (viewport->Size.x - image_width);
        const float image_y = 0.5f * (viewport->Size.y - image_height);
        const ImVec2 image_pos = ImVec2(image_x, image_y);

        if(!status.empty()) {
            ImGui::PushFont(globals::font_chat);
            ImGui::SetCursorPos(ImVec2(16.0f, 16.0f));
            ImGui::TextDisabled("%s", status.c_str());
            ImGui::PopFont();
        }

        const ImVec2 uv_a = ImVec2(0.0f, 0.0f);
        const ImVec2 uv_b = ImVec2(1.0f, 1.0f);
        const ImVec4 tint = ImVec4(1.0f, 1.0f, 1.0f, texture_alpha);

        ImGui::SetCursorPos(image_pos);
        ImGui::Image(texture->imgui, image_size, uv_a, uv_b, tint);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(globals::window);
    glfwPollEvents();
}
