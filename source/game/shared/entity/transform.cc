// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/transform.hh"
#include "shared/globals.hh"

void TransformComponent::update(void)
{
    const auto view = globals::registry.view<TransformComponent>();

    for(auto [entity, transform] : view.each()) {
        for(std::size_t i = 0U; i < 3U; ++i) {
            if(transform.position.local[i] >= CHUNK_SIZE) {
                transform.position.local[i] -= CHUNK_SIZE;
                transform.position.chunk[i] += 1;
                continue;
            }

            if(transform.position.local[i] < 0.0f) {
                transform.position.local[i] += CHUNK_SIZE;
                transform.position.chunk[i] -= 1;
                continue;
            }
        }
    }
}
