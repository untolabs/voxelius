// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/chunk.hh"
#include "shared/world/chunk_coord.hh"

class Config;

namespace worldgen
{
void setup(Config &config);
void setup_late(void);
Chunk *generate(const ChunkCoord &cpos);
} // namespace worldgen
