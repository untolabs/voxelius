// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/chunk_coord.hh"
#include "shared/world/local_coord.hh"
#include "shared/world/voxel_coord.hh"

class WorldCoord final {
public:
    ChunkCoord chunk {};
    Vec3f local {};

public:
    static LocalCoord to_local(const WorldCoord &wvec);
    static VoxelCoord to_voxel(const WorldCoord &wvec);
    static Vec3f to_vec3f(const WorldCoord &wvec);
    static Vec3f to_vec3f(const WorldCoord &pivot, const ChunkCoord &cvec);
    static Vec3f to_vec3f(const WorldCoord &pivot, const WorldCoord &wvec);
};
