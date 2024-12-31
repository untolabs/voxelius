// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/receive.hh"

#include "shared/entity/head.hh"
#include "shared/entity/player.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/world/world.hh"

#include "shared/factory.hh"
#include "shared/protocol.hh"

#include "client/gui/chat.hh"
#include "client/gui/gui_screen.hh"

#include "client/globals.hh"
#include "client/session.hh"


static bool make_entity(entt::entity entity)
{
    if(!globals::registry.valid(entity)) {
        entt::entity created = globals::registry.create(entity);

        if(created != entity) {
            session::mp::disconnect("protocol.chunk_entity_mismatch");
            spdlog::critical("receive: chunk entity mismatch");
            return false;
        }
    }

    return true;
}

static void on_chunk_voxels_packet(const protocol::ChunkVoxels &packet)
{
    if(session::peer) {
        if(!globals::registry.valid(packet.entity)) {
            entt::entity created = globals::registry.create(packet.entity);

            if(created != packet.entity) {
                globals::registry.destroy(created);
                session::mp::disconnect("protocol.chunk_entity_mismatch");
                spdlog::critical("receive: chunk entity mismatch");
                return;
            }
        }

        Chunk *chunk = Chunk::create();
        chunk->entity = packet.entity;
        chunk->voxels = packet.voxels;

        world::emplace_or_replace(packet.chunk, chunk);
    }
}

static void on_entity_head_packet(const protocol::EntityHead &packet)
{
    if(session::peer) {
        if(!make_entity(packet.entity))
            return;
        auto &component = globals::registry.get_or_emplace<HeadComponent>(packet.entity);
        component.angles = packet.angles;
    }
}

static void on_entity_transform_packet(const protocol::EntityTransform &packet)
{
    if(session::peer) {
        if(!globals::registry.valid(packet.entity))
            static_cast<void>(globals::registry.create(packet.entity));
        auto &component = globals::registry.get_or_emplace<TransformComponent>(packet.entity);
        component.angles = packet.angles;
        component.position = packet.coord;
    }
}

static void on_entity_velocity_packet(const protocol::EntityVelocity &packet)
{
    if(session::peer) {
        if(!globals::registry.valid(packet.entity))
            static_cast<void>(globals::registry.create(packet.entity));
        auto &component = globals::registry.get_or_emplace<VelocityComponent>(packet.entity);
        component.angular = packet.angular;
        component.linear = packet.linear;
    }
}

static void on_entity_player_packet(const protocol::EntityPlayer &packet)
{
    if(session::peer) {
        if(!globals::registry.valid(packet.entity))
            static_cast<void>(globals::registry.create(packet.entity));
        factory::create_player(packet.entity);
    }
}

static void on_spawn_player_packet(const protocol::SpawnPlayer &packet)
{
    if(session::peer) {
        if(!globals::registry.valid(packet.entity)) {
            static_cast<void>(globals::registry.create(packet.entity));
            factory::create_player(packet.entity);
        }

        globals::player = packet.entity;
        globals::gui_screen = GUI_SCREEN_NONE;

        client_chat::refresh_timings();
    }
}

static void on_remove_entity_packet(const protocol::RemoveEntity &packet)
{
    if(globals::registry.valid(packet.entity)) {
        if(packet.entity == globals::player)
            globals::player = entt::null;
        globals::registry.destroy(packet.entity);
    }
}

void client_receive::init(void)
{
    globals::dispatcher.sink<protocol::ChunkVoxels>().connect<&on_chunk_voxels_packet>();
    globals::dispatcher.sink<protocol::EntityHead>().connect<&on_entity_head_packet>();
    globals::dispatcher.sink<protocol::EntityTransform>().connect<&on_entity_transform_packet>();
    globals::dispatcher.sink<protocol::EntityVelocity>().connect<&on_entity_velocity_packet>();
    globals::dispatcher.sink<protocol::EntityPlayer>().connect<&on_entity_player_packet>();
    globals::dispatcher.sink<protocol::SpawnPlayer>().connect<&on_spawn_player_packet>();
    globals::dispatcher.sink<protocol::RemoveEntity>().connect<&on_remove_entity_packet>();
}
