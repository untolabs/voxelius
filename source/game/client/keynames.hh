// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace keynames
{
void init(void);
void deinit(void);
} // namespace keynames

namespace keynames
{
const std::string &get(int key_code);
} // namespace keynames
