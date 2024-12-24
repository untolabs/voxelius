// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/status.hh"

#include "shared/motd.hh"
#include "shared/protocol.hh"

#include "server/globals.hh"
#include "server/sessions.hh"


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
