// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/chunk.hh"


Chunk *Chunk::create(void)
{
    Chunk *object = new Chunk();
    object->voxels.fill(NULL_VOXEL);
    object->entity = entt::null;
    return object;
}

void Chunk::destroy(Chunk *chunk)
{
    delete chunk;
}
