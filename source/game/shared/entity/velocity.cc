// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/velocity.hh"
#include "shared/entity/transform.hh"
#include "shared/globals.hh"

void VelocityComponent::fixed_update(void)
{
    const auto group = globals::registry.group<VelocityComponent>(entt::get<TransformComponent>);

    for(const auto [entity, velocity, transform] : group.each()) {
        transform.position.local += velocity.linear * globals::fixed_frametime;
        transform.angles += velocity.angular * globals::fixed_frametime;
    }
}
