// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/voxel_coord.hh"
#include "shared/world/voxel_id.hh"
#include "shared/world/world_coord.hh"

class RayDDA final {
public:
    Vec3f direction {};
    WorldCoord start {};

public:
    Vec3f delta_dist {};
    Vec3f side_dist {};
    VoxelCoord vstep {};

public:
    double distance {};
    VoxelCoord vnormal {};
    VoxelCoord vpos {};

public:
    static void setup(RayDDA &ray, const WorldCoord &start, const Vec3f &direction);
    static VoxelID step(RayDDA &ray);
};
