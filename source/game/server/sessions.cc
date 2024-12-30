// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/sessions.hh"

#include "mathlib/constexpr.hh"

#include "common/config.hh"
#include "common/crc64.hh"
#include "common/fstools.hh"
#include "common/strtools.hh"

#include "shared/entity/head.hh"
#include "shared/entity/player.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/event/chunk_create.hh"
#include "shared/event/chunk_update.hh"
#include "shared/event/voxel_set.hh"

#include "shared/world/chunk.hh"
#include "shared/world/voxel_def.hh"

#include "shared/protocol.hh"

#include "server/game.hh"
#include "server/globals.hh"
#include "server/whitelist.hh"


unsigned int sessions::max_players = 8U;
unsigned int sessions::num_players = 0U;

static emhash8::HashMap<std::string, Session *> username_map = {};
static emhash8::HashMap<std::uint64_t, Session *> identity_map = {};
static std::vector<Session> sessions_vector = {};

static void on_login_request_packet(const protocol::LoginRequest &packet)
{
    if(packet.version > protocol::VERSION) {
        protocol::send_disconnect(packet.peer, nullptr, "protocol.outdated_server");
        return;
    }

    if(packet.version < protocol::VERSION) {
        protocol::send_disconnect(packet.peer, nullptr, "protocol.outdated_client");
        return;
    }

    // FIXME: calculate voxel registry checksum ahead of time
    // instead of figuring it out every time a new player connects
    if(packet.voxel_def_checksum != voxel_def::calc_checksum()) {
        protocol::send_disconnect(packet.peer, nullptr, "protocol.voxel_def_checksum_mismatch");
        return;
    }

    // Don't assign new usernames and just kick the player if
    // an another client using the same username is already connected
    // and playing; since we have a whitelist, adding "(1)" isn't feasible anymore
    if(username_map.contains(packet.username)) {
        protocol::send_disconnect(packet.peer, nullptr, "protocol.username_taken");
        return;
    }

    if(whitelist::enabled) {
        if(!whitelist::contains(packet.username)) {
            protocol::send_disconnect(packet.peer, nullptr, "protocol.not_whitelisted");
            return;
        }

        if(!whitelist::matches(packet.username, packet.password_hash)) {
            protocol::send_disconnect(packet.peer, nullptr, "protocol.password_incorrect");
            return;
        }
    }
    else if(packet.password_hash != server_game::password_hash) {
        protocol::send_disconnect(packet.peer, nullptr, "protocol.password_incorrect");
        return;
    }

    if(Session *session = sessions::create(packet.peer, packet.username)) {
        protocol::LoginResponse response = {};
        response.client_index = session->client_index;
        response.client_identity = session->client_identity;
        response.server_tickrate = globals::tickrate;
        protocol::send(packet.peer, nullptr, response);

        spdlog::info("sessions: {} [{}] logged in with client_index={}", session->client_username, session->client_identity, session->client_index);

        // FIXME: only send entities that are present within the current
        // player's view bounding box; this also would mean we're not sending
        // anything here and just straight up spawing the player and await them
        // to receive all the chunks and entites they feel like requesting
        for(auto entity : globals::registry.view<entt::entity>()) {
            protocol::send_entity_head(session->peer, nullptr, entity);
            protocol::send_entity_transform(session->peer, nullptr, entity);
            protocol::send_entity_velocity(session->peer, nullptr, entity);
            protocol::send_entity_player(session->peer, nullptr, entity);
        }

        session->player_entity = globals::registry.create();
        globals::registry.emplace<HeadComponent>(session->player_entity, HeadComponent());
        globals::registry.emplace<PlayerComponent>(session->player_entity, PlayerComponent());
        globals::registry.emplace<TransformComponent>(session->player_entity, TransformComponent());
        globals::registry.emplace<VelocityComponent>(session->player_entity, VelocityComponent());

        // The player entity is to be spawned in the world the last;
        // We don't want to interact with the still not-loaded world!
        protocol::send_entity_head(nullptr, globals::server_host, session->player_entity);
        protocol::send_entity_transform(nullptr, globals::server_host, session->player_entity);
        protocol::send_entity_velocity(nullptr, globals::server_host, session->player_entity);
        protocol::send_entity_player(nullptr, globals::server_host, session->player_entity);

        // SpawnPlayer serves a different purpose compared to EntityPlayer
        // The latter is used to construct entities (as in "attach a component")
        // whilst the SpawnPlayer packet is used to notify client-side that the
        // entity identifier in the packet is to be treated as the local player entity
        protocol::send_spawn_player(session->peer, nullptr, session->player_entity);

        protocol::ChatMessage message = {};
        message.type = protocol::ChatMessage::PLAYER_JOIN;
        message.sender = session->client_username;
        message.message = std::string();
        protocol::send(nullptr, globals::server_host, message);

        sessions::refresh_player_list();

        return;
    }

    protocol::send_disconnect(packet.peer, nullptr, "protocol.server_full");
}

static void on_disconnect_packet(const protocol::Disconnect &packet)
{
    if(Session *session = sessions::find(packet.peer)) {
        protocol::ChatMessage message = {};
        message.type = protocol::ChatMessage::PLAYER_LEAVE;
        message.sender = session->client_username;
        message.message = packet.reason;
        protocol::send(session->peer, globals::server_host, message);

        spdlog::info("{} disconnected ({})", session->client_username, packet.reason);

        sessions::destroy(session);
        sessions::refresh_player_list();
    }
}

// NOTE: [sessions] is a good place for this since [receive]
// handles entity data sent by players and [sessions] handles
// everything else network related that is not player movement
static void on_chunk_create(const ChunkCreateEvent &event)
{
    protocol::ChunkVoxels packet = {};
    packet.entity = event.chunk->entity;
    packet.chunk = event.coord;
    packet.voxels = event.chunk->voxels;
    protocol::send(nullptr, globals::server_host, packet);
}

static void on_chunk_update(const ChunkUpdateEvent &event)
{
    protocol::ChunkVoxels packet = {};
    packet.entity = event.chunk->entity;
    packet.chunk = event.coord;
    packet.voxels = event.chunk->voxels;
    protocol::send(nullptr, globals::server_host, packet);
}

static void on_voxel_set(const VoxelSetEvent &event)
{
    protocol::send_set_voxel(nullptr, globals::server_host, event.vpos, event.voxel);
}

static void on_destroy_entity(const entt::registry &registry, entt::entity entity)
{
    protocol::RemoveEntity packet = {};
    packet.entity = entity;
    protocol::send(nullptr, globals::server_host, packet);
}

void sessions::init(void)
{
    Config::add(globals::server_config, "sessions.max_players", sessions::max_players);

    globals::dispatcher.sink<protocol::LoginRequest>().connect<&on_login_request_packet>();
    globals::dispatcher.sink<protocol::Disconnect>().connect<&on_disconnect_packet>();

    globals::dispatcher.sink<ChunkCreateEvent>().connect<&on_chunk_create>();
    globals::dispatcher.sink<ChunkUpdateEvent>().connect<&on_chunk_update>();
    globals::dispatcher.sink<VoxelSetEvent>().connect<&on_voxel_set>();

    globals::registry.on_destroy<entt::entity>().connect<&on_destroy_entity>();
}

void sessions::init_late(void)
{
    sessions::max_players = cxpr::clamp<unsigned int>(sessions::max_players, 1U, UINT16_MAX);
    sessions::num_players = 0U;

    username_map.clear();
    identity_map.clear();
    sessions_vector.resize(sessions::max_players, Session());

    for(unsigned int i = 0U; i < sessions::max_players; ++i) {
        sessions_vector[i].client_index = UINT16_MAX;
        sessions_vector[i].client_identity = UINT64_MAX;
        sessions_vector[i].client_username = std::string();
        sessions_vector[i].player_entity = entt::null;
        sessions_vector[i].peer = nullptr;
    }
}

void sessions::deinit(void)
{
    username_map.clear();
    identity_map.clear();
    sessions_vector.clear();
}

Session *sessions::create(ENetPeer *peer, const std::string &client_username)
{
    for(unsigned int i = 0U; i < sessions::max_players; ++i) {
        if(!sessions_vector[i].peer) {
            std::uint64_t client_identity = crc64::get(client_username);

            sessions_vector[i].client_index = i;
            sessions_vector[i].client_identity = client_identity;
            sessions_vector[i].client_username = client_username;
            sessions_vector[i].player_entity = entt::null;
            sessions_vector[i].peer = peer;        

            username_map[client_username] = &sessions_vector[i];
            identity_map[client_identity] = &sessions_vector[i];

            peer->data = &sessions_vector[i];

            sessions::num_players += 1U;

            return &sessions_vector[i];
        }
    }

    return nullptr;
}

Session *sessions::find(const std::string &client_username)
{
    const auto it = username_map.find(client_username);
    if(it != username_map.cend())
        return it->second;
    return nullptr;
}

Session *sessions::find(std::uint16_t client_index)
{
    if(client_index < sessions_vector.size()) {
        if(!sessions_vector[client_index].peer)
            return nullptr;
        return &sessions_vector[client_index];
    }

    return nullptr;
}

Session *sessions::find(std::uint64_t client_identity)
{
    const auto it = identity_map.find(client_identity);
    if(it != identity_map.cend())
        return it->second;
    return nullptr;
}

Session *sessions::find(ENetPeer *peer)
{
    return reinterpret_cast<Session *>(peer->data);
}

void sessions::destroy(Session *session)
{
    if(session) {
        if(session->peer) {
            // Make sure we don't leave a mark
            session->peer->data = nullptr;
        }
        
        globals::registry.destroy(session->player_entity);

        username_map.erase(session->client_username);
        identity_map.erase(session->client_identity);

        session->client_index = UINT16_MAX;
        session->client_identity = UINT64_MAX;
        session->client_username = std::string();
        session->player_entity = entt::null;
        session->peer = nullptr;

        sessions::num_players -= 1U;
    }
}

void sessions::refresh_player_list(void)
{
    protocol::PlayerListUpdate packet = {};

    for(std::size_t i = 0; i < sessions::max_players; ++i) {
        if(!sessions_vector[i].peer)
            continue;
        packet.names.push_back(sessions_vector[i].client_username);
    }

    protocol::send(nullptr, globals::server_host, packet);
}
