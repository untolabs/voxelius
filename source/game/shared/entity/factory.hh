// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/world_coord.hh"

namespace entity_factory
{
void create_player(entt::entity entity, bool interpolated, const WorldCoord &location);
} // namespace entity_factory
