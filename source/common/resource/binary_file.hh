// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "common/resource/resource.hh"

struct BinaryFile final {
    std::uint8_t *buffer;
    std::size_t length;
};
