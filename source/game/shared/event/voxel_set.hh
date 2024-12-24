// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/chunk.hh"
#include "shared/world/local_coord.hh"
#include "shared/world/voxel_coord.hh"

struct VoxelSetEvent final {
    std::size_t index {};
    ChunkCoord cpos {};
    LocalCoord lpos {};
    VoxelCoord vpos {};
    Chunk *chunk {};
    VoxelID voxel {};    
};
