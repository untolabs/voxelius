// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/chat.hh"

#include "common/config.hh"
#include "common/strtools.hh"

#include "shared/protocol.hh"

#include "client/event/glfw_key.hh"

#include "client/gui/gui_screen.hh"
#include "client/gui/language.hh"
#include "client/gui/settings.hh"

#include "client/game.hh"
#include "client/globals.hh"


constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

constexpr static unsigned int MAX_HISTORY_SIZE = 128U;

struct GuiChatMessage final {
    std::uint64_t spawn {};
    std::string text {};
    ImVec4 color {};
};

static int key_chat = GLFW_KEY_ENTER;
static unsigned int history_size = 32U;
static std::deque<GuiChatMessage> history = {};
static std::string chat_input = {};
static bool needs_focus = false;

static void append_text_message(const std::string &sender, const std::string &text)
{
    GuiChatMessage message = {};
    message.spawn = globals::curtime;
    message.text = fmt::format("[{}] {}", sender, text);
    message.color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
    history.push_back(message);
}

static void append_player_join(const std::string &sender)
{
    GuiChatMessage message = {};
    message.spawn = globals::curtime;
    message.text = fmt::format("{} {}", sender, language::resolve("chat.client_join"));
    message.color = ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget);
    history.push_back(message);
}

static void append_player_leave(const std::string &sender, const std::string &reason)
{
    GuiChatMessage message = {};
    message.spawn = globals::curtime;
    message.text = fmt::format("{} {} ({})", sender, language::resolve("chat.client_left"), language::resolve(reason));
    message.color = ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget);
    history.push_back(message);
}

static void on_chat_message_packet(const protocol::ChatMessage &packet)
{
    if(packet.type == protocol::ChatMessage::TEXT_MESSAGE) {
        append_text_message(packet.sender, packet.message);
        return;
    }
    
    if(packet.type == protocol::ChatMessage::PLAYER_JOIN) {
        append_player_join(packet.sender);
        return;
    }
    
    if(packet.type == protocol::ChatMessage::PLAYER_LEAVE) {
        append_player_leave(packet.sender, packet.message);
        return;
    }
}

static void on_glfw_key(const GlfwKeyEvent &event)
{
    if(event.action == GLFW_PRESS) {
        if((event.key == GLFW_KEY_ENTER) && (globals::gui_screen == GUI_CHAT)) {
            if(!strtools::is_empty_or_whitespace(chat_input)) {
                if(globals::is_singleplayer) {
                    // For singleplayer, just append a new
                    // chat message into the history without any
                    // kind of packet manipulation since peer is null
                    append_text_message(client_game::username, chat_input);
                }
                else {
                    protocol::ChatMessage packet = {};
                    packet.type = protocol::ChatMessage::TEXT_MESSAGE;
                    packet.sender = client_game::username;
                    packet.message = chat_input;
                    protocol::send(globals::session_peer, nullptr, packet);
                }
            }

            globals::gui_screen = GUI_SCREEN_NONE;

            chat_input.clear();

            return;
        }
        
        if((event.key == GLFW_KEY_ESCAPE) && (globals::gui_screen == GUI_CHAT)) {
            globals::gui_screen = GUI_SCREEN_NONE;
            return;
        }
        
        if((event.key == key_chat) && !globals::gui_screen) {
            globals::gui_screen = GUI_CHAT;
            needs_focus = true;
            return;
        }
    }
}

void client_chat::init(void)
{
    Config::add(globals::client_config, "chat.key", key_chat);
    Config::add(globals::client_config, "chat.history_size", history_size);

    settings::add_key_binding(2, settings::KEYBOARD_MISC, "key.chat", key_chat);
    settings::add_slider(1, settings::VIDEO_GUI, "chat.history_size", history_size, 0U, MAX_HISTORY_SIZE, false);

    globals::dispatcher.sink<protocol::ChatMessage>().connect<&on_chat_message_packet>();
    globals::dispatcher.sink<GlfwKeyEvent>().connect<&on_glfw_key>();
}

void client_chat::init_late(void)
{

}

void client_chat::update(void)
{
    history_size = cxpr::min(history_size, MAX_HISTORY_SIZE);
    
    if(history_size == 0U) {
        history.clear();
    }
    else {    
        while(history.size() > history_size) {
            history.pop_front();
        }
    }
}

void client_chat::layout(void)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 window_start = ImVec2(0.0f, 0.0f);
    const ImVec2 window_size = ImVec2(0.75f * viewport->Size.x, viewport->Size.y);

    ImGui::SetNextWindowPos(window_start);
    ImGui::SetNextWindowSize(window_size);

    ImGui::PushFont(globals::font_chat);

    if(!ImGui::Begin("###chat", nullptr, WINDOW_FLAGS)) {
        ImGui::End();
        return;
    }

    const ImVec2 &padding = ImGui::GetStyle().FramePadding;
    const ImVec2 &spacing = ImGui::GetStyle().ItemSpacing;
    const ImFont *font = ImGui::GetFont();

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // The text input widget occupies the bottom part
    // of the chat window, we need to reserve some space for it
    float ypos = window_size.y - font->FontSize - 2.0f * padding.y - 2.0f * spacing.y;

    if(globals::gui_screen == GUI_CHAT) {
        if(needs_focus) {
            ImGui::SetKeyboardFocusHere();
            needs_focus = false;
        }

        ImGui::SetNextItemWidth(window_size.x + 32.0f * padding.x);
        ImGui::SetCursorScreenPos(ImVec2(padding.x, ypos));
        ImGui::InputText("###chat.input", &chat_input);
    }

    if((globals::gui_screen == GUI_SCREEN_NONE) || (globals::gui_screen == GUI_CHAT) || (globals::gui_screen == GUI_DEBUG_WINDOW)) {
        for(auto it = history.crbegin(); it < history.crend(); ++it) {
            const ImVec2 text_size = ImGui::CalcTextSize(it->text.c_str(), it->text.c_str() + it->text.size(), false, window_size.x);
            const ImVec2 rect_size = ImVec2(window_size.x, text_size.y + 2.0f * padding.y);

            const ImVec2 rect_pos = ImVec2(padding.x, ypos - text_size.y - 2.0f * padding.y);
            const ImVec2 rect_end = ImVec2(rect_pos.x + rect_size.x, rect_pos.y + rect_size.y);
            const ImVec2 text_pos = ImVec2(rect_pos.x + padding.x, rect_pos.y + padding.y);

            const float fadeout_seconds = 10.0f;
            const float fadeout = std::exp(-1.0f * std::pow(1.0e-6 * static_cast<float>(globals::curtime - it->spawn) / fadeout_seconds, 10.0f));
            const float rect_alpha = ((globals::gui_screen == GUI_CHAT) ? (0.75f) : (0.50f * fadeout));
            const float text_alpha = ((globals::gui_screen == GUI_CHAT) ? (1.00f) : (1.00f * fadeout));

            const ImU32 rect_col = ImGui::GetColorU32(ImGuiCol_FrameBg, rect_alpha);
            const ImU32 text_col = ImGui::GetColorU32(ImVec4(it->color.x, it->color.y, it->color.z, it->color.w * text_alpha));

            draw_list->AddRectFilled(rect_pos, rect_end, rect_col);
            draw_list->AddText(font, font->FontSize, text_pos, text_col, it->text.c_str(), it->text.c_str() + it->text.size(), window_size.x);

            ypos -= rect_size.y;
        }
    }

    ImGui::End();
    ImGui::PopFont();
}

void client_chat::clear(void)
{
    history.clear();
}

void client_chat::refresh_timings(void)
{
    for(auto it = history.begin(); it < history.end(); ++it) {
        // Reset the spawn time so the fadeout timer
        // is reset; SpawnPlayer handler might call this
        it->spawn = globals::curtime;
    }
}

void client_chat::print(const std::string &text)
{
    GuiChatMessage message = {};
    message.spawn = globals::curtime;
    message.text = text;
    message.color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
    history.push_back(message);
}
