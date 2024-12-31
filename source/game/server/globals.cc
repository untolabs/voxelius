// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/globals.hh"

#include "common/config.hh"

#include "shared/protocol.hh"


Config globals::server_config = {};

ENetHost *globals::server_host = {};

bool globals::is_running = false;
unsigned int globals::tickrate = protocol::TICKRATE;
std::uint64_t globals::tickrate_dt = 0;
