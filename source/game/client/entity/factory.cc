// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/entity/factory.hh"

#include "shared/entity/factory.hh"
#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"

#include "client/entity/sound_emitter.hh"

#include "client/resource/sound_effect.hh"

#include "client/globals.hh"


void client_entity_factory::create_player(entt::entity entity)
{
    shared_entity_factory::create_player(entity);

    const auto &head = globals::registry.get<HeadComponent>(entity);
    globals::registry.emplace_or_replace<HeadComponentIntr>(entity, head);
    globals::registry.emplace_or_replace<HeadComponentPrev>(entity, head);

    const auto &transform = globals::registry.get<TransformComponent>(entity);
    globals::registry.emplace_or_replace<TransformComponentIntr>(entity, transform);
    globals::registry.emplace_or_replace<TransformComponentPrev>(entity, transform);

    globals::registry.emplace_or_replace<SoundEmitterComponent>(entity);
}
