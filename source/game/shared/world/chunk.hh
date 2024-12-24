// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/const.hh"
#include "shared/world/voxel_id.hh"


using VoxelStorage = std::array<VoxelID, CHUNK_VOLUME>;
using LightStorage = std::array<std::int8_t, CHUNK_VOLUME>;

class Chunk final {
public:
    entt::entity entity {};
    VoxelStorage voxels {};

public:
    static Chunk *create(void);
    static void destroy(Chunk *chunk);
};
