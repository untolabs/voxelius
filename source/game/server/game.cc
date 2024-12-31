// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/game.hh"

#include "mathlib/constexpr.hh"

#include "common/cmdline.hh"
#include "common/config.hh"
#include "common/crc64.hh"
#include "common/epoch.hh"

#include "shared/entity/head.hh"
#include "shared/entity/player.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/world/game_items.hh"
#include "shared/world/game_voxels.hh"
#include "shared/world/universe.hh"
#include "shared/world/unloader.hh"
#include "shared/world/world.hh"

#include "shared/worldgen/worldgen.hh"

#include "shared/motd.hh"
#include "shared/protocol.hh"

#include "server/chat.hh"
#include "server/globals.hh"
#include "server/receive.hh"
#include "server/sessions.hh"
#include "server/status.hh"
#include "server/whitelist.hh"


unsigned int server_game::view_distance = 4U;
std::uint64_t server_game::password_hash = UINT64_MAX;

static unsigned int listen_port = protocol::PORT;
static unsigned int status_peers = 4U;
static std::string password_string = std::string();

static std::uint64_t worldgen_seed = UINT64_C(42);

void server_game::init(void)
{
    Config::add(globals::server_config, "game.listen_port", listen_port);
    Config::add(globals::server_config, "game.status_peers", status_peers);
    Config::add(globals::server_config, "game.password", password_string);
    Config::add(globals::server_config, "game.view_distance", server_game::view_distance);

    Config::add(globals::server_config, "worldgen.seed", worldgen_seed);

    sessions::init();

    whitelist::init();

    motd::init("motds/server.txt");
    status::init();

    server_chat::init();
    server_recieve::init();

    world::init();

    unloader::init();
}

void server_game::init_late(void)
{
    server_game::view_distance = cxpr::clamp(server_game::view_distance, 2U, 32U);
    server_game::password_hash = crc64::get(password_string);

    sessions::init_late();

    whitelist::init_late();

    listen_port = cxpr::clamp<unsigned int>(listen_port, 1024U, UINT16_MAX);
    status_peers = cxpr::clamp<unsigned int>(status_peers, 2U, 16U);

    ENetAddress address = {};
    address.host = ENET_HOST_ANY;
    address.port = listen_port;

    globals::server_host = enet_host_create(&address, sessions::max_players + status_peers, 1, 0, 0);

    if(!globals::server_host) {
        spdlog::critical("game: unable to setup an ENet host");
        std::terminate();
    }

    spdlog::info("game: host: {} player + {} status peers", sessions::max_players, status_peers);
    spdlog::info("game: host: listening on UDP port {}", address.port);

    game_voxels::populate();
    game_items::populate();

    std::string universe_name = {};

    if(!cmdline::get_value("universe", universe_name))
        universe_name = "save";
    universe::setup(universe_name);

    unloader::init_late(server_game::view_distance);
}

void server_game::deinit(void)
{
    protocol::send_disconnect(nullptr, globals::server_host, "protocol.server_shutdown");

    whitelist::deinit();

    sessions::deinit();

    enet_host_flush(globals::server_host);
    enet_host_service(globals::server_host, nullptr, 500);
    enet_host_destroy(globals::server_host);

    universe::save_everything();
}

void server_game::fixed_update(void)
{

}

void server_game::fixed_update_late(void)
{
    ENetEvent event = {};

    while(enet_host_service(globals::server_host, &event, 0) > 0) {
        if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            sessions::destroy(sessions::find(event.peer));
            sessions::refresh_player_list();
            continue;
        }

        if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            protocol::receive(event.packet, event.peer);
            enet_packet_destroy(event.packet);
            continue;
        }
    }

    unloader::update_late();
}
