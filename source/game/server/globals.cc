// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/globals.hh"

#include "common/config.hh"


Config globals::server_config = {};

ENetHost *globals::server_host = {};

bool globals::is_running = false;
unsigned int globals::tickrate = 30U;
std::uint64_t globals::tickrate_dt = 0;
