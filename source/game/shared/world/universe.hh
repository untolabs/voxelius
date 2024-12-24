// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/chunk.hh"
#include "shared/world/chunk_coord.hh"

namespace universe
{
void setup(const std::string &directory);
void save_everything(void);
} // namespace universe

namespace universe
{
Chunk *load_chunk(const ChunkCoord &cpos);
void save_chunk(const ChunkCoord &cpos);
void save_all_chunks(void);
} // namespace universe
