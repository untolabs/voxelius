// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec2base.hh"
#include "shared/world/chunk_coord.hh"

using ChunkCoord2D = Vec2base<ChunkCoord::value_type>;

template<>
struct std::hash<ChunkCoord2D> final {
    constexpr inline std::size_t operator()(const ChunkCoord2D &cvec) const
    {
        std::size_t value = 0;
        value ^= cvec[0] * 73856093;
        value ^= cvec[1] * 19349663;
        return value;
    }
};
