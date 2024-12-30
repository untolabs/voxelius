// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/text_shadow.hh"

#include "client/globals.hh"


void text_shadow::layout(const std::string &text, const ImVec2 &position, ImU32 text_color, ImU32 shadow_color, const ImFont *font, ImDrawList *draw_list)
{
    const auto shadow_position = ImVec2(position.x + 0.5f * globals::gui_scale, position.y + 0.5f * globals::gui_scale);
    draw_list->AddText(font, font->FontSize, shadow_position, shadow_color, text.c_str(), text.c_str() + text.size());
    draw_list->AddText(font, font->FontSize, position, text_color, text.c_str(), text.c_str() + text.size());
}
