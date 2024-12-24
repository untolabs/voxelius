// SPDX-License-Identifier: BSD-2-Clause
#pragma once

using ProgressBarAction = void(*)(void);

namespace progress
{
void init(void);
void layout(void);
} // progress

namespace progress
{
void reset(void);
void set_title(const std::string &title);
void set_button(const std::string &text, const ProgressBarAction &action);
} // namespace progress
