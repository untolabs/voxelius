// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <game/shared/voxel_coord.hh>
#include <game/shared/vdef.hh>

namespace player_target
{
extern Voxel voxel;
extern VoxelCoord vvec;
extern VoxelCoord vnormal;
extern const VoxelInfo *info;
} // namespace player_target

namespace player_target
{
void init(void);
void update(void);
void render(void);
} // namespace player_target
