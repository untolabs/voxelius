// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"

#include "cmake/config.hh"

#include "mathlib/constexpr.hh"

#include "common/resource/binary_file.hh"
#include "common/resource/image.hh"

#include "common/config.hh"
#include "common/cmdline.hh"
#include "common/epoch.hh"

#include "shared/setup.hh"

#include "server/game.hh"
#include "server/globals.hh"


static void on_termination_signal(int)
{
    spdlog::warn("server: received termination signal");
    globals::is_running = false;
}

int main(int argc, char **argv)
{
    cmdline::append(argc, argv);

    shared::setup(argc, argv);

    spdlog::info("server: game version: {}", PROJECT_VERSION_STRING);

    globals::fixed_frametime = 0.0f;
    globals::fixed_frametime_avg = 0.0f;
    globals::fixed_frametime_us = 0;
    globals::fixed_framecount = 0;

    globals::curtime = epoch::microseconds();

    globals::is_running = true;

    std::signal(SIGINT, &on_termination_signal);
    std::signal(SIGTERM, &on_termination_signal);

    server_game::init();

    Config::add(globals::server_config, "server.tickrate", globals::tickrate);
    Config::load(globals::server_config, "server.conf");

    globals::tickrate = cxpr::clamp(globals::tickrate, 10U, 300U);
    globals::tickrate_dt = static_cast<std::uint64_t>(1000000.0f / static_cast<float>(globals::tickrate));

    server_game::init_late();

    std::uint64_t last_curtime = globals::curtime;
    
    while(globals::is_running) {
        globals::curtime = epoch::microseconds();

        globals::fixed_frametime_us = globals::curtime - last_curtime;
        globals::fixed_frametime = static_cast<float>(globals::fixed_frametime_us) / 1000000.0f;
        globals::fixed_frametime_avg += globals::fixed_frametime;
        globals::fixed_frametime_avg *= 0.5f;

        last_curtime = globals::curtime;
        
        server_game::fixed_update();
        server_game::fixed_update_late();

        globals::dispatcher.update();
        
        globals::fixed_framecount += 1;

        std::this_thread::sleep_for(std::chrono::microseconds(globals::tickrate_dt));

        resource::soft_cleanup<BinaryFile>();
        resource::soft_cleanup<Image>();
    }

    server_game::deinit();
    
    resource::hard_cleanup<BinaryFile>();
    resource::hard_cleanup<Image>();

    spdlog::info("server: shutdown after {} frames", globals::fixed_framecount);
    spdlog::info("server: average framerate: {:.03f} TPS", 1.0f / globals::fixed_frametime_avg);
    spdlog::info("server: average frametime: {:.03f} MSPT", 1000.0f * globals::fixed_frametime_avg);

    Config::save(globals::server_config, "server.conf");

    shared::desetup();

    return 0;
}
