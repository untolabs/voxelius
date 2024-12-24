// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/motd.hh"

#include "common/fstools.hh"


static std::mt19937_64 twister = {};
static std::vector<std::string> titles = {};

void motd::init(const std::string &path)
{
    std::string source = {};

    if(!fstools::read_string(path, source)) {
        spdlog::warn("motd: {}: {}", path, fstools::error());
        titles.push_back("motd::get returned this");
    }
    else {
        std::string line = {};
        std::istringstream stream = std::istringstream(source);
        while(std::getline(stream, line)) {
            titles.push_back(line);
        }
    }

    twister.seed(std::random_device()());
}

const std::string &motd::get(void)
{
    auto distrib = std::uniform_int_distribution<std::size_t>(0, titles.size() - 1);
    return titles.at(distrib(twister));
}
