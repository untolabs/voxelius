// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace motd
{
void init(const std::string &path);
const std::string &get(void);
} // namespace motd
