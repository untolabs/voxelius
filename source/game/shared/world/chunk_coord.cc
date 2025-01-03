// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/chunk_coord.hh"

#include "shared/world/local_coord.hh"
#include "shared/world/world_coord.hh"
#include "shared/world/voxel_coord.hh"


VoxelCoord ChunkCoord::to_voxel(const ChunkCoord &cvec, std::size_t index)
{
    return ChunkCoord::to_voxel(cvec, LocalCoord::from_index(index));
}

VoxelCoord ChunkCoord::to_voxel(const ChunkCoord &cvec, const LocalCoord &lvec)
{
    VoxelCoord result = {};
    result[0] = lvec[0] + (cvec[0] << CHUNK_SIZE_LOG2);
    result[1] = lvec[1] + (cvec[1] << CHUNK_SIZE_LOG2);
    result[2] = lvec[2] + (cvec[2] << CHUNK_SIZE_LOG2);
    return result;
}

WorldCoord ChunkCoord::to_world(const ChunkCoord &cvec, const Vec3f &lvec)
{
    WorldCoord result = {};
    result.chunk = cvec;
    result.local = lvec;
    return result;
}

WorldCoord ChunkCoord::to_world(const ChunkCoord &cvec, const LocalCoord &lvec)
{
    WorldCoord result = {};
    result.chunk = cvec;
    result.local[0] = static_cast<float>(lvec[0]);
    result.local[1] = static_cast<float>(lvec[1]);
    result.local[2] = static_cast<float>(lvec[2]);
    return result;
}

WorldCoord ChunkCoord::to_world(const ChunkCoord &cvec)
{
    WorldCoord result = {};
    result.chunk = cvec;
    return result;
}

Vec3f ChunkCoord::to_vec3f(const ChunkCoord &pivot, const WorldCoord &offset)
{
    Vec3f result = {};
    result[0] = static_cast<float>((offset.chunk[0] - pivot[0]) << CHUNK_SIZE_LOG2) + offset.local[0];
    result[1] = static_cast<float>((offset.chunk[1] - pivot[1]) << CHUNK_SIZE_LOG2) + offset.local[1];
    result[2] = static_cast<float>((offset.chunk[2] - pivot[2]) << CHUNK_SIZE_LOG2) + offset.local[2];
    return result;
}

Vec3f ChunkCoord::to_vec3f(const ChunkCoord &cvec)
{
    Vec3f result = {};
    result[0] = static_cast<float>(cvec[0] << CHUNK_SIZE_LOG2);
    result[1] = static_cast<float>(cvec[1] << CHUNK_SIZE_LOG2);
    result[2] = static_cast<float>(cvec[2] << CHUNK_SIZE_LOG2);
    return result;
}
