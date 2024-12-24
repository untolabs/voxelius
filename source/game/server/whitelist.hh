// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace whitelist
{
extern bool enabled;
extern std::string filename;
} // namespace whitelist

namespace whitelist
{
void init(void);
void init_late(void);
void deinit(void);
} // namespace whitelist

namespace whitelist
{
bool contains(const std::string &username);
bool matches(const std::string &username, std::uint64_t password_hash);
} // namespace whitelist
