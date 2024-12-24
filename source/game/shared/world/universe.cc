// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/universe.hh"

#include "common/config.hh"
#include "common/epoch.hh"
#include "common/fstools.hh"
#include "common/packet_buffer.hh"

#include "shared/entity/chunk.hh"
#include "shared/entity/inhabited.hh"

#include "shared/world/world.hh"

#include "shared/worldgen/worldgen.hh"

#include "shared/globals.hh"
#include "shared/protocol.hh"


static Config universe_config = {};
static std::string universe_dir = {};
static std::string universe_chunk_dir = {};
static std::string universe_config_path = {};

static std::uint64_t worldgen_seed = UINT64_MAX;

static std::string chunk_filename(const ChunkCoord &cpos)
{
    auto cx = static_cast<std::uint32_t>(cpos.get_x());
    auto cy = static_cast<std::uint32_t>(cpos.get_y());
    auto cz = static_cast<std::uint32_t>(cpos.get_z());
    return fmt::format("{:08X}-{:08X}-{:08X}.zvox", cx, cy, cz);
}

void universe::setup(const std::string &directory)
{
    worldgen_seed = epoch::milliseconds();

    universe_dir = directory;
    universe_chunk_dir = fmt::format("{}/chunk", universe_dir);
    universe_config_path = fmt::format("{}/universe.conf", universe_dir);

    if(!PHYSFS_mkdir(universe_dir.c_str())) {
        spdlog::critical("universe: mkdir {}: {}", universe_dir, fstools::error());
        std::terminate();
    }

    if(!PHYSFS_mkdir(universe_chunk_dir.c_str())) {
        spdlog::critical("universe: mkdir {}: {}", universe_chunk_dir, fstools::error());
        std::terminate();
    }

    Config::add(universe_config, "worldgen.seed", worldgen_seed);
    
    worldgen::setup(universe_config);

    Config::load(universe_config, universe_config_path);

    worldgen::setup_late();
}

void universe::save_everything(void)
{
    universe::save_all_chunks();

    Config::save(universe_config, universe_config_path);
}

Chunk *universe::load_chunk(const ChunkCoord &cpos)
{
    if(auto chunk = world::find(cpos)) {
        // Just return the existing chunk which is
        // most probable to be up to date compared to
        // whatever the hell is currently stored on disk
        return chunk;
    }

    auto path = fmt::format("{}/chunk/{}", universe_dir, chunk_filename(cpos));
    auto buffer = std::vector<std::uint8_t>();

    if(fstools::read_bytes(path, buffer)) {
        auto chunk = Chunk::create();
        chunk->entity = globals::registry.create();
        chunk->voxels.fill(NULL_VOXEL);

        auto size = static_cast<mz_ulong>(sizeof(VoxelStorage));
        auto bound = static_cast<mz_ulong>(buffer.size());

        mz_uncompress(reinterpret_cast<unsigned char *>(chunk->voxels.data()), &size, buffer.data(), bound);

        for(std::size_t i = 0; i < CHUNK_VOLUME; ++i) {
            // Compressed voxels are stored in the network
            // byte order; just like voxels in ChunkVoxels packet
            chunk->voxels[i] = ENET_NET_TO_HOST_16(chunk->voxels[i]);
        }

        world::emplace_or_replace(cpos, chunk);

        // Ensure the loaded chunk is marked as inhabited as-is
        globals::registry.emplace_or_replace<InhabitedComponent>(chunk->entity);

        return chunk;
    }

    return worldgen::generate(cpos);
}

void universe::save_chunk(const ChunkCoord &cpos)
{
    if(auto chunk = world::find(cpos)) {
        VoxelStorage voxels = {};

        for(std::size_t i = 0; i < CHUNK_VOLUME; ++i) {
            // Compressed voxels are stored in the network
            // byte order; just like voxels in ChunkVoxels packet
            voxels[i] = ENET_HOST_TO_NET_16(chunk->voxels[i]);
        }

        auto bound = mz_compressBound(sizeof(VoxelStorage));
        auto buffer = std::vector<std::uint8_t>(bound, UINT8_C(0x00));
        mz_compress(buffer.data(), &bound, reinterpret_cast<const unsigned char *>(voxels.data()), sizeof(VoxelStorage));

        // Make sure we're not writing any excess
        // data that didn't wasn't used by mz_compress
        buffer.resize(bound);

        auto path = fmt::format("{}/chunk/{}", universe_dir, chunk_filename(cpos));

        if(!fstools::write_bytes(path, buffer)) {
            spdlog::warn("universe::save_chunk: {}: {}", path, fstools::error());
            return;
        }
    }
}

void universe::save_all_chunks(void)
{
    auto group = globals::registry.group(entt::get<ChunkComponent, InhabitedComponent>);

    for(auto [entity, chunk] : group.each()) {
        universe::save_chunk(chunk.coord);
    }
}
