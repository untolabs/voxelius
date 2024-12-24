// SPDX-License-Identifier: BSD-2-Clause
#pragma once

struct Session;

namespace server_chat
{
void init(void);
void broadcast(const std::string &message);
void broadcast(const std::string &message, const std::string &sender);
void send(Session *session, const std::string &message);
void send(Session *session, const std::string &message, const std::string &sender);
} // namespace server_chat
