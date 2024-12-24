// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/worldgen/worldgen.hh"

#include "common/config.hh"

#include "shared/event/chunk_update.hh"

#include "shared/world/game_voxels.hh"
#include "shared/world/local_coord.hh"
#include "shared/world/voxel_coord.hh"
#include "shared/world/world.hh"

#include "shared/worldgen/overworld.hh"

#include "shared/globals.hh"
#include "shared/protocol.hh"


static std::uint64_t seed = UINT64_C(42);

void worldgen::setup(Config &config)
{
    Config::add(config, "worldgen.seed", seed);

    worldgen::overworld::setup(config);
}

void worldgen::setup_late(void)
{
    worldgen::overworld::setup_late(seed);
}

Chunk *worldgen::generate(const ChunkCoord &cpos)
{
    VoxelStorage generated = {};

    if(worldgen::overworld::generate(cpos, generated)) {
        auto chunk = Chunk::create();
        chunk->entity = globals::registry.create();
        chunk->voxels = generated;

        world::emplace_or_replace(cpos, chunk);

        return chunk;
    }

    return nullptr;
}
