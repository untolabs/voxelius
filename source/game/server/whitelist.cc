// SPDX-License-Identifier: BSD-2-Clause
#include "server/precompiled.hh"
#include "server/whitelist.hh"

#include "common/config.hh"
#include "common/crc64.hh"
#include "common/fstools.hh"
#include "common/strtools.hh"

#include "server/game.hh"
#include "server/globals.hh"


constexpr static const char *WHITELIST_DEFAULT_FILENAME = "whitelist.txt";
constexpr static char WHITELIST_SEPARATOR = ':';

bool whitelist::enabled = false;
std::string whitelist::filename = WHITELIST_DEFAULT_FILENAME;

static emhash8::HashMap<std::string, std::uint64_t> whitelist_map = {};

void whitelist::init(void)
{
    Config::add(globals::server_config, "whitelist.enabled", whitelist::enabled);
    Config::add(globals::server_config, "whitelist.filename", whitelist::filename);
}

void whitelist::init_late(void)
{
    whitelist_map.clear();

    if(!whitelist::enabled) {
        // Not enabled, shouldn't
        // even bother with parsing
        // the whitelist file
        return;
    }

    if(strtools::is_empty_or_whitespace(whitelist::filename)) {
        spdlog::warn("whitelist: enabled but filename is empty, using default ({})", WHITELIST_DEFAULT_FILENAME);
        whitelist::filename = WHITELIST_DEFAULT_FILENAME;
    }

    PHYSFS_File *file = PHYSFS_openRead(whitelist::filename.c_str());

    if(file == nullptr) {
        spdlog::warn("whitelist: {}: {}", whitelist::filename, fstools::error());
        whitelist::enabled = false;
        return;
    }

    std::string line;

    while(fstools::read_line(file, line)) {
        const auto location = line.find_last_of(WHITELIST_SEPARATOR);

        if(location == std::string::npos) {
            // Entries that don't define a password field default
            // to the global server password; this allows easier adding
            // of guest accounts which can later be edited to use a better password
            whitelist_map[line] = server_game::password_hash;
        }
        else {
            const auto username = line.substr(0, location);
            const auto password = line.substr(location + 1);
            whitelist_map[username] = crc64::get(password);
        }
    }

    PHYSFS_close(file);
}

void whitelist::deinit(void)
{
    // UNDONE: implement saving
}

bool whitelist::contains(const std::string &username)
{
    return whitelist_map.contains(username);
}

bool whitelist::matches(const std::string &username, std::uint64_t password_hash)
{
    const auto it = whitelist_map.find(username);

    if(it == whitelist_map.cend()) {
        // Not whitelisted, no match
        return false;
    }

    if(it->second == password_hash)
        return true;
    return false;
}
