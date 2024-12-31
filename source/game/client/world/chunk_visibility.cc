// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/world/chunk_visibility.hh"

#include "shared/entity/chunk.hh"

#include "shared/world/universe.hh"
#include "shared/world/world.hh"

#include "shared/protocol.hh"

#include "client/globals.hh"
#include "client/session.hh"
#include "client/view.hh"

static ChunkCoord cached_cpos = {};
static unsigned int cached_dist = {};
static std::vector<ChunkCoord> requests = {};

static void request_chunk(const ChunkCoord &cpos)
{
    if(globals::is_singleplayer) {
        universe::load_chunk(cpos);
        return;
    }

    protocol::RequestChunk packet = {};
    packet.coord[0] = cpos[0];
    packet.coord[1] = cpos[1];
    packet.coord[2] = cpos[2];

    protocol::send(session::peer, nullptr, packet);
}

// Go through the list of chunk positions that should
// be visible client-side but seem to not exist yet
static void request_new_chunks(void)
{
    auto cmin = cached_cpos - cached_dist;
    auto cmax = cached_cpos + cached_dist;

    requests.clear();

    for(auto cx = cmin[0]; cx <= cmax[0]; ++cx)
    for(auto cy = cmin[1]; cy <= cmax[1]; ++cy)
    for(auto cz = cmin[2]; cz <= cmax[2]; ++cz) {
        if(world::find(ChunkCoord(cx, cy, cz))) {
            // The chunk already exists, we don't need
            // to request it from the server anymore
            continue;
        }

        requests.push_back(ChunkCoord(cx, cy, cz));
    }

    std::sort(requests.begin(), requests.end(), [](const ChunkCoord &ca, const ChunkCoord &cb) {
        auto dir_a = ca - cached_cpos;
        auto dir_b = cb - cached_cpos;

        const auto da = dir_a[0] * dir_a[0] + dir_a[1] * dir_a[1] + dir_a[2] * dir_a[2];
        const auto db = dir_b[0] * dir_b[0] + dir_b[1] * dir_b[1] + dir_b[2] * dir_b[2];

        return da > db;
    });
}

static bool is_chunk_visible(const ChunkCoord &cvec)
{
    const auto dx = cxpr::abs(cvec[0] - cached_cpos[0]);
    const auto dy = cxpr::abs(cvec[2] - cached_cpos[2]);

    if((dx <= cached_dist) && (dy <= cached_dist))
        return true;
    return false;
}

void chunk_visibility::update_chunk(entt::entity entity)
{
    if(const ChunkComponent *chunk = globals::registry.try_get<ChunkComponent>(entity)) {
        if(is_chunk_visible(chunk->coord))
            globals::registry.emplace_or_replace<ChunkVisibleComponent>(entity);
        else globals::registry.remove<ChunkVisibleComponent>(entity);
    }
}

void chunk_visibility::update_chunk(const ChunkCoord &cvec)
{
    if(const Chunk *chunk = world::find(cvec)) {
        if(is_chunk_visible(cvec))
            globals::registry.emplace_or_replace<ChunkVisibleComponent>(chunk->entity);
        else globals::registry.remove<ChunkVisibleComponent>(chunk->entity);
    }
}

void chunk_visibility::update_chunks(void)
{
    const auto view = globals::registry.view<ChunkComponent>();
    
    for(const auto [entity, chunk] : view.each()) {
        if(is_chunk_visible(chunk.coord))
            globals::registry.emplace_or_replace<ChunkVisibleComponent>(entity);
        else globals::registry.remove<ChunkVisibleComponent>(entity);
    }

    request_new_chunks();
}

void chunk_visibility::cleanup(void)
{
    cached_cpos = view::position.chunk + 1;
    cached_dist = view::max_distance + 1;
    requests.clear();
}

void chunk_visibility::update(void)
{
    if(globals::registry.valid(globals::player)) {
        if((cached_cpos != view::position.chunk) || (cached_dist != view::max_distance)) {
            cached_cpos = view::position.chunk;
            cached_dist = view::max_distance;
            chunk_visibility::update_chunks();
            return;
        }
        else {
            for(int i = 0; i < 16; ++i) {
                if(requests.empty()) {
                    // Done sending requests
                    break;
                }

                request_chunk(requests.back());

                requests.pop_back();
            }
        }
        
        cached_cpos = view::position.chunk;
        cached_dist = view::max_distance;
        return;
    }
    
    cached_cpos = view::position.chunk + 1;
    cached_dist = view::max_distance + 1;
}
