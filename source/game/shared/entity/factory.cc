// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/factory.hh"

#include "shared/entity/collision.hh"
#include "shared/entity/gravity.hh"
#include "shared/entity/head.hh"
#include "shared/entity/player.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/globals.hh"


void entity_factory::create_player(entt::entity entity, bool interpolated, const WorldCoord &location)
{
    spdlog::debug("entity_factory: assigning player components to {}", static_cast<std::uint64_t>(entity));

    auto &collision = globals::registry.emplace_or_replace<CollisionComponent>(entity);
    collision.hull.min = Vec3f(-0.4f, -1.6f, -0.4f);
    collision.hull.max = Vec3f(+0.4f, +0.2f, +0.4f);

    globals::registry.emplace_or_replace<GravityComponent>(entity);

    auto &head = globals::registry.emplace_or_replace<HeadComponent>(entity);
    head.angles = Vec3angles::zero();
    head.position = Vec3f::zero();

    if(interpolated) {
        globals::registry.emplace_or_replace<HeadComponentIntr>(entity, head);
        globals::registry.emplace_or_replace<HeadComponentPrev>(entity, head);
    }

    globals::registry.emplace_or_replace<PlayerComponent>(entity);

    auto &transform = globals::registry.emplace_or_replace<TransformComponent>(entity);
    transform.angles = Vec3angles::zero();
    transform.position = location;

    if(interpolated) {
        globals::registry.emplace_or_replace<TransformComponentIntr>(entity, transform);
        globals::registry.emplace_or_replace<TransformComponentPrev>(entity, transform);
    }

    auto &velocity = globals::registry.emplace_or_replace<VelocityComponent>(entity);
    velocity.angular = Vec3angles::zero();
    velocity.linear = Vec3f::zero();
}
