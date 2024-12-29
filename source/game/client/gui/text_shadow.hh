// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace text_shadow
{
void layout(const std::string &text, const ImVec2 &position, ImU32 text_color, ImU32 shadow_color, ImFont *font, ImDrawList *draw_list);
} // namespace text_shadow
