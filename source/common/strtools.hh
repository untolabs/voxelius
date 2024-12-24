// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace strtools
{
bool contains(const std::string &str, char character);
bool contains(const std::string &str, const std::string &substr);
bool is_empty_or_whitespace(const std::string &str);
bool is_whitespace(const std::string &str);
std::string join(const std::vector<std::string> &strings, const std::string &separator);
std::vector<std::string> split(const std::string &str, const std::string &separator);
std::string trim_whitespace(const std::string &str);
} // namespace strtools
