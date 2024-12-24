// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace fstools
{
const char *error(void);
bool read_bytes(const std::string &path, std::vector<std::uint8_t> &buffer);
bool read_string(const std::string &path, std::string &buffer);
bool write_bytes(const std::string &path, const std::vector<std::uint8_t> &buffer);
bool write_string(const std::string &path, const std::string &buffer);
bool read_line(PHYSFS_File *file, std::string &line);
} // namespace fstools
