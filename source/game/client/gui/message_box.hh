// SPDX-License-Identifier: BSD-2-Clause
#pragma once

using MessageBoxAction = void(*)(void);

namespace message_box
{
void init(void);
void layout(void);
void reset(void);
} // namespace message_box

namespace message_box
{
void set_title(const std::string &title);
void set_subtitle(const std::string &subtitle);
void add_button(const std::string &text, const MessageBoxAction &action);
} // namespace message_box
