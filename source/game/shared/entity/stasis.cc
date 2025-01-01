// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/stasis.hh"

#include "shared/entity/transform.hh"

#include "shared/world/world.hh"

#include "shared/globals.hh"


void StasisComponent::fixed_update(void)
{
    auto view = globals::registry.view<TransformComponent>();

    for(auto [entity, transform] : view.each()) {
        if(nullptr == world::find(transform.position.chunk))
            globals::registry.emplace_or_replace<StasisComponent>(entity);
        else globals::registry.remove<StasisComponent>(entity);
    }
}
