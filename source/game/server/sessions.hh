// SPDX-License-Identifier: BSD-2-Clause
#pragma once

namespace sessions
{
extern unsigned int max_players;
extern unsigned int num_players;
} // namespace sessions

struct Session final {
    std::uint16_t client_index {};
    std::uint64_t client_identity {};
    std::string client_username {};
    entt::entity player_entity {};
    ENetPeer *peer {};
};

namespace sessions
{
void init(void);
void init_late(void);
void deinit(void);
} // namespace sessions

namespace sessions
{
Session *create(ENetPeer *peer, const std::string &client_username);
Session *find(const std::string &client_username);
Session *find(std::uint16_t client_index);
Session *find(std::uint64_t client_identity);
Session *find(ENetPeer *peer);
void destroy(Session *session);
} // namespace sessions

namespace sessions
{
void refresh_player_list(void);
} // namespace sessions
