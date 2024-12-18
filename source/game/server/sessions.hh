// SPDX-License-Identifier: BSD-2-Clause
#pragma once

struct Session final {
    std::uint16_t session_id {};
    std::uint64_t player_uid {};
    std::string username {};
    entt::entity player {};
    ENetPeer *peer {};
};

namespace sessions
{
extern unsigned int max_players;
extern unsigned int num_players;
extern std::uint64_t password_hash;
} // namespace sessions

namespace sessions
{
void init(void);
void init_late(void);
void deinit(void);
} // namespace sessions

namespace sessions
{
Session *create(ENetPeer *peer, std::uint64_t player_uid, const std::string &username);
Session *find(std::uint16_t session_id);
Session *find(std::uint64_t player_uid);
Session *find(ENetPeer *peer);
void destroy(Session *session);
} // namespace sessions

namespace sessions
{
void refresh_player_list(void);
} // namespace sessions
