// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/voxel_id.hh"

namespace game_voxels
{
extern VoxelID cobblestone;
extern VoxelID dirt;
extern VoxelID grass;
extern VoxelID stone;
extern VoxelID vtest;
extern VoxelID vtest_ck;
extern VoxelID oak_leaves;
extern VoxelID oak_planks;
extern VoxelID oak_log;
extern VoxelID glass;
extern VoxelID slime;
extern VoxelID mud;
} // namespace game_voxels

namespace game_voxels
{
void populate(void);
} // namespace game_voxels
