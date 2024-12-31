// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/gravity.hh"

#include "shared/entity/velocity.hh"

#include "shared/globals.hh"


float GravityComponent::acceleration = 30.0f;

void GravityComponent::update(void)
{
    auto group = globals::registry.group<GravityComponent>(entt::get<VelocityComponent>);

    for(auto [entity, velocity] : group.each()) {
        velocity.linear[1] -= GravityComponent::acceleration * globals::frametime;
    }
}
