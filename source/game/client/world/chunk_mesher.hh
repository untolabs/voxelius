// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "client/world/chunk_vbo.hh"

struct ChunkMeshComponent final {
    std::vector<ChunkVBO> quad_nb {};
    std::vector<ChunkVBO> quad_b {};
};

namespace chunk_mesher
{
void init(void);
void deinit(void);
void update(void);
} // namespace chunk_mesher
