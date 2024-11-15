// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <game/shared/chunk_coord.hh>

namespace worldgen
{
void init(void);
void init_late(std::uint64_t seed);
void deinit(void);
void update(void);
} // namespace worldgen

namespace worldgen
{
void generate(const ChunkCoord &cpos);
} // namespace worldgen
