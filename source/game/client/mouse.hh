// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace mouse
{
extern bool enable_raw_input;
extern float sensitivity;
} // namespace mouse

namespace mouse
{
void init(void);
void update_late(void);
} // namespace mouse
