// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/entity/interpolation.hh"

#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"

#include "client/globals.hh"


static void interpolate_head(float alpha)
{
    auto group = globals::registry.group(entt::get<HeadComponent, HeadComponentPrev>);

    for(auto [entity, current, previous] : group.each()) {
        auto &intr = globals::registry.get_or_emplace<HeadComponentIntr>(entity);

        intr.angles[0] = cxpr::lerp(previous.angles[0], current.angles[0], alpha);
        intr.angles[1] = cxpr::lerp(previous.angles[1], current.angles[1], alpha);
        intr.angles[2] = cxpr::lerp(previous.angles[2], current.angles[2], alpha);

        intr.offset[0] = cxpr::lerp(previous.offset[0], current.offset[0], alpha);
        intr.offset[1] = cxpr::lerp(previous.offset[1], current.offset[1], alpha);
        intr.offset[2] = cxpr::lerp(previous.offset[2], current.offset[2], alpha);
    }
}

static void interpolate_transform(float alpha)
{
    auto group = globals::registry.group(entt::get<TransformComponent, TransformComponentPrev>);

    for(auto [entity, current, previous] : group.each()) {
        auto &intr = globals::registry.get_or_emplace<TransformComponentIntr>(entity);

        intr.angles[0] = cxpr::lerp(previous.angles[0], current.angles[0], alpha);
        intr.angles[1] = cxpr::lerp(previous.angles[1], current.angles[1], alpha);
        intr.angles[2] = cxpr::lerp(previous.angles[2], current.angles[2], alpha);

        // Figure out a local floating point coordinates that are relative
        // to the current (the one we're interpolating into) world coordinate
        // chunk position basis; this fixes the annoying jitter when the camera
        // moves through chunks (because TransformComponent::fixed_update
        // progressively forces local coordinates into chunk-internal range)
        const auto previous_shift = WorldCoord::to_vec3f(current.position, previous.position);
        const auto previous_local = current.position.local + previous_shift;

        intr.position.chunk[0] = current.position.chunk[0];
        intr.position.chunk[1] = current.position.chunk[1];
        intr.position.chunk[2] = current.position.chunk[2];

        intr.position.local[0] = cxpr::lerp(previous_local[0], current.position.local[0], alpha);
        intr.position.local[1] = cxpr::lerp(previous_local[1], current.position.local[1], alpha);
        intr.position.local[2] = cxpr::lerp(previous_local[2], current.position.local[2], alpha);
    }
}

void interpolation::update(void)
{
    const auto alpha = static_cast<float>(globals::fixed_accumulator) / static_cast<float>(globals::fixed_frametime_us);

    interpolate_head(alpha);
    interpolate_transform(alpha);
}
