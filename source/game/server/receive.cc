// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/receive.hh"

#include "mathlib/box3base.hh"

#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/world/universe.hh"
#include "shared/world/world.hh"

#include "shared/protocol.hh"

#include "server/game.hh"
#include "server/globals.hh"
#include "server/sessions.hh"


static void on_entity_transform_packet(const protocol::EntityTransform &packet)
{
    if(Session *session = sessions::find(packet.peer)) {
        if(globals::registry.valid(session->player_entity)) {
            auto &transform = globals::registry.get_or_emplace<TransformComponent>(session->player_entity);
            transform.angles = packet.angles;
            transform.position = packet.coord;

            // Propagate changes to the rest of the world
            // except the peer that has sent the packet in the first place
            // UNDONE: pass nullptr instead of packet.peer when we want to correct the client
            protocol::send_entity_transform(packet.peer, globals::server_host, session->player_entity);
        }
    }
}

static void on_entity_velocity_packet(const protocol::EntityVelocity &packet)
{
    if(Session *session = sessions::find(packet.peer)) {
        if(globals::registry.valid(session->player_entity)) {
            auto &velocity = globals::registry.get_or_emplace<VelocityComponent>(session->player_entity);
            velocity.angular = packet.angular;
            velocity.linear = packet.linear;

            // Propagate changes to the rest of the world
            // except the peer that has sent the packet in the first place
            // UNDONE: pass nullptr instead of packet.peer when we want to correct the client
            protocol::send_entity_velocity(packet.peer, globals::server_host, session->player_entity);
        }
    }
}

static void on_entity_head_packet(const protocol::EntityHead &packet)
{
    if(Session *session = sessions::find(packet.peer)) {
        if(globals::registry.valid(session->player_entity)) {
            auto &transform = globals::registry.get_or_emplace<HeadComponent>(session->player_entity);
            transform.angles = packet.angles;

            // Propagate changes to the rest of the world
            // except the peer that has sent the packet in the first place
            // UNDONE: pass nullptr instead of packet.peer when we want to correct the client
            protocol::send_entity_head(packet.peer, globals::server_host, session->player_entity);
        }
    }
}

static void on_set_voxel_packet(const protocol::SetVoxel &packet)
{
    if(!world::set_voxel(packet.voxel, packet.coord)) {
        const auto cpos = VoxelCoord::to_chunk(packet.coord);
        const auto lpos = VoxelCoord::to_local(packet.coord);
        const auto index = LocalCoord::to_index(lpos);

        Chunk *chunk = Chunk::create();
        chunk->entity = globals::registry.create();
        chunk->voxels[index] = packet.voxel;

        world::emplace_or_replace(cpos, chunk);
        
        // Broadcast the newly created chunk to peers
        protocol::send_chunk_voxels(nullptr, globals::server_host, chunk->entity);
    }
}

static void on_request_chunk_packet(const protocol::RequestChunk &packet)
{
    if(auto session = reinterpret_cast<const Session *>(packet.peer->data)) {
        if(!globals::registry.valid(session->player_entity)) {
            // De-spawned sessions cannot request
            // chunks from the server; that's cheating!!!
            return;
        }

        if(auto transform = globals::registry.try_get<TransformComponent>(session->player_entity)) {
            auto view_box = Box3base<ChunkCoord::value_type>();
            view_box.min = transform->position.chunk - server_game::view_distance;
            view_box.max = transform->position.chunk + server_game::view_distance;

            if(Box3base<ChunkCoord::value_type>::contains(view_box, packet.coord)) {
                if(auto chunk = universe::load_chunk(packet.coord)) {
                    protocol::send_chunk_voxels(packet.peer, nullptr, chunk->entity);
                }
            }
        }
    }
}

void server_recieve::init(void)
{
    globals::dispatcher.sink<protocol::EntityTransform>().connect<&on_entity_transform_packet>();
    globals::dispatcher.sink<protocol::EntityVelocity>().connect<&on_entity_velocity_packet>();
    globals::dispatcher.sink<protocol::EntityHead>().connect<&on_entity_head_packet>();
    globals::dispatcher.sink<protocol::SetVoxel>().connect<&on_set_voxel_packet>();
    globals::dispatcher.sink<protocol::RequestChunk>().connect<&on_request_chunk_packet>();
}
