// SPDX-License-Identifier: BSD-2-Clause
#include <entt/signal/dispatcher.hpp>
#include <game/server/globals.hh>
#include <game/server/sessions.hh>
#include <game/server/status.hh>
#include <game/shared/motd.hh>
#include <game/shared/protocol.hh>

static void on_status_request_packet(const protocol::StatusRequest &packet)
{
    protocol::StatusResponse response = {};
    response.version = protocol::VERSION;
    response.max_players = sessions::max_players;
    response.num_players = sessions::num_players;
    response.motd = motd::get();
    protocol::send(packet.peer, nullptr, response);
}

void status::init(void)
{
    globals::dispatcher.sink<protocol::StatusRequest>().connect<&on_status_request_packet>();
}
