// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/chunk.hh"
#include "shared/world/chunk_coord.hh"

class Config;

namespace worldgen::overworld
{
void setup(Config &config);
void setup_late(std::uint64_t seed);
bool generate(const ChunkCoord &cpos, VoxelStorage &voxels);
} // namespace worldgen::overworld
