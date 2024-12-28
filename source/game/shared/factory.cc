// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/factory.hh"

#include "shared/entity/collision.hh"
#include "shared/entity/gravity.hh"
#include "shared/entity/head.hh"
#include "shared/entity/player.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/globals.hh"


void factory::create_player(entt::entity entity)
{
    globals::registry.emplace_or_replace<GravityComponent>(entity);
    globals::registry.emplace_or_replace<PlayerComponent>(entity);
    globals::registry.emplace_or_replace<TransformComponent>(entity);
    globals::registry.emplace_or_replace<VelocityComponent>(entity);

    auto &head = globals::registry.emplace_or_replace<HeadComponent>(entity);
    head.offset = Vec3f(0.0f, 0.0f, 0.0f);

    auto &collision = globals::registry.emplace_or_replace<CollisionComponent>(entity);
    collision.hull.min = Vec3f(-0.4f, -1.6f, -0.4f);
    collision.hull.max = Vec3f(+0.4f, +0.2f, +0.4f);

    spdlog::debug("factory: assign player components to {}", static_cast<std::uint64_t>(entity));
}
