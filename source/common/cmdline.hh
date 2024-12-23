// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace cmdline
{
void append(int argc, char **argv);
void append(const std::string &option);
void append(const std::string &option, const std::string &argument);
bool get_value(const std::string &option, std::string &argument);
bool contains(const std::string &option);
} // namespace cmdline
