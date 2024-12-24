// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace crc64
{
std::uint64_t get(const void *data, std::size_t size, std::uint64_t combine = 0);
std::uint64_t get(const std::vector<std::uint8_t> &data, std::uint64_t combine = 0);
std::uint64_t get(const std::string &data, std::uint64_t combine = 0);
} // namespace crc64
