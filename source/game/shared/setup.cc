// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/setup.hh"

#include "common/cmdline.hh"
#include "common/fstools.hh"

static std::filesystem::path get_gamepath(void)
{
    std::string value;

    if(cmdline::contains("dev")) {
        // We are running in a development environment.
        // Base game content is located in the repo.
        // UNDONE: allow this to be on by default in case
        // of a generic source build (like historic)
        return std::filesystem::current_path() / "assets";
    }

    if(cmdline::get_value("gamepath", value)) {
        if(!value.empty()) {
            // If there is a launcher that has profile
            // support, we can override the default game path
            // with whatever it finds fit for the profile.
            return std::filesystem::path(value);
        }
    }

    return std::filesystem::current_path();
}

static std::filesystem::path get_userpath(void)
{
    std::string value;

    if(cmdline::get_value("userpath", value)) {
        if(!value.empty()) {
            // Things like systemd scripts might account
            // for multiple server instances running on
            // the same machine, so it seems like a good
            // idea to allow overriding this path.
            return std::filesystem::path(value);
        }
    }

    if(const char *win_appdata = std::getenv("APPDATA")) {
        // On pre-seven Windows systems it's just AppData
        // On post-seven Windows systems it's AppData/Roaming
        return std::filesystem::path(win_appdata) / "voxelius";
    }

    if(const char *xdg_home = std::getenv("XDG_DATA_HOME")) {
        // Systems with an active X11/Wayland session
        // theoretically should have this defined, and
        // it can be different from ${HOME} (I think).
        return std::filesystem::path(xdg_home) / ".voxelius";
    }

    if(const char *unix_home = std::getenv("HOME")) {
        // Any spherical UNIX/UNIX-like system in vacuum
        // has this defined for every single user process.
        return std::filesystem::path(unix_home) / ".voxelius";
    }

    return std::filesystem::current_path();
}

void shared::setup(int argc, char **argv)
{
    auto logger = spdlog::default_logger();
    auto &logger_sinks = logger->sinks();

#if defined(_WIN32)
    logger_sinks.clear();
    logger_sinks.push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>(true));
    logger_sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
#endif

#if defined(__unix__)
    if(cmdline::contains("use-syslog")) {
        logger_sinks.clear();
        logger_sinks.push_back(std::make_shared<spdlog::sinks::syslog_sink_mt>("voxelius", (LOG_PID | LOG_NDELAY), LOG_USER, true));
    }
    else {
        logger_sinks.clear();
        logger_sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
    }
#endif

#if defined(NDEBUG)
    const auto default_loglevel = spdlog::level::info;
#else
    const auto default_loglevel = spdlog::level::trace;
#endif

    if(cmdline::contains("quiet"))
        logger->set_level(spdlog::level::warn);
    else if(cmdline::contains("verbose"))
        logger->set_level(spdlog::level::trace);
    else logger->set_level(default_loglevel);

    logger->set_pattern("[%H:%M:%S] %^[%L]%$ %v");
    logger->flush();

    if(!PHYSFS_init(argv[0])) {
        spdlog::critical("physfs: init failed: {}", fstools::error());
        std::terminate();
    }

    auto gamepath = get_gamepath();
    auto userpath = get_userpath();

    spdlog::info("main: set gamepath to {}", gamepath.string());
    spdlog::info("main: set userpath to {}", userpath.string());

    std::error_code ignore_error = {};
    std::filesystem::create_directories(gamepath, ignore_error);
    std::filesystem::create_directories(userpath, ignore_error);

    if(!PHYSFS_mount(gamepath.string().c_str(), nullptr, false)) {
        spdlog::critical("physfs: mount {} failed: {}", gamepath.string(), fstools::error());
        std::terminate();
    }

    if(!PHYSFS_mount(userpath.string().c_str(), nullptr, false)) {
        spdlog::critical("physfs: mount {} failed: {}", userpath.string(), fstools::error());
        std::terminate();
    }

    if(!PHYSFS_setWriteDir(userpath.string().c_str())) {
        spdlog::critical("physfs: setwritedir {} failed: {}", userpath.string(), fstools::error());
        std::terminate();
    }

    if(enet_initialize()) {
        spdlog::critical("enet: init failed");
        std::terminate();
    }
}

void shared::desetup(void)
{
    enet_deinitialize();

    if(!PHYSFS_deinit()) {
        spdlog::critical("physfs: deinit failed: {}", fstools::error());
        std::terminate();
    }
}
