// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/unloader.hh"

#include "mathlib/box3base.hh"

#include "shared/entity/chunk.hh"
#include "shared/entity/inhabited.hh"
#include "shared/entity/player.hh"
#include "shared/entity/transform.hh"

#include "shared/event/chunk_update.hh"
#include "shared/event/voxel_set.hh"

#include "shared/world/universe.hh"

#include "shared/globals.hh"


// FIXME: move this in a header?
using Box3chunk = Box3base<ChunkCoord::value_type>;

static unsigned int unload_threshold = 0U;
static std::vector<Box3chunk> player_boxes = {};

static void on_chunk_update(const ChunkUpdateEvent &event)
{
    globals::registry.emplace_or_replace<InhabitedComponent>(event.chunk->entity);
}

static void on_voxel_set(const VoxelSetEvent &event)
{
    globals::registry.emplace_or_replace<InhabitedComponent>(event.chunk->entity);
}

void unloader::init(void)
{
    globals::dispatcher.sink<ChunkUpdateEvent>().connect<&on_chunk_update>();
    globals::dispatcher.sink<VoxelSetEvent>().connect<&on_voxel_set>();
}

void unloader::init_late(unsigned int view_distance)
{
    unload_threshold = view_distance;
    player_boxes.clear();
}

void unloader::update_late(void)
{
    player_boxes.clear();

    auto group = globals::registry.group(entt::get<PlayerComponent, TransformComponent>);

    for(const auto [entity, transform] : group.each()) {
        Box3chunk box = {};
        box.min = transform.position.chunk - unload_threshold;
        box.max = transform.position.chunk + unload_threshold;
        player_boxes.push_back(box);
    }

    auto view = globals::registry.view<ChunkComponent>();
    auto chunk_in_view = false;

    for(const auto [entity, chunk] : view.each()) {
        chunk_in_view = false;

        for(const Box3chunk &box : player_boxes) {
            if(Box3chunk::contains(box, chunk.coord)) {
                chunk_in_view = true;
                break;
            }
        }

        if(chunk_in_view) {
            // The chunk is within view box of at least
            // a single player; we shouldn't unload it now
            continue;
        }

        if(globals::registry.any_of<InhabitedComponent>(entity)) {
            // Only store inhabited chunks on disk
            universe::save_chunk(chunk.coord);
        }

        // On server-side this will also notify all the
        // connected clients that this specific chunk has been
        // unloaded; please note that adding support for view
        // distances greater than ones defined by the server is
        // very cumbersome considering chunk entity desync errors
        globals::registry.destroy(entity);
    }
}
