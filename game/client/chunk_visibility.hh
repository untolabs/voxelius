// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include <entt/entity/entity.hpp>
#include <game/shared/chunk_coord.hh>

struct ChunkVisibleComponent final {};

namespace chunk_visibility
{
void update_chunk(entt::entity entity);
void update_chunk(const ChunkCoord &cvec);
void update_chunks(void);
} // namespace chunk_visibility

namespace chunk_visibility
{
void update(void);
} // namespace chunk_visibility
