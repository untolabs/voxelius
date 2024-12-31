// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/entity/interpolation.hh"

#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"

#include "client/globals.hh"


static void head_interpolate(float alpha)
{
    auto group = globals::registry.group<HeadComponentIntr>(entt::get<HeadComponent, HeadComponentPrev>);

    for(auto [entity, interp, current, previous] : group.each()) {
        interp.angles[0] = cxpr::lerp(previous.angles[0], current.angles[0], alpha);
        interp.angles[1] = cxpr::lerp(previous.angles[1], current.angles[1], alpha);
        interp.angles[2] = cxpr::lerp(previous.angles[2], current.angles[2], alpha);

        interp.position[0] = cxpr::lerp(previous.position[0], current.position[0], alpha);
        interp.position[1] = cxpr::lerp(previous.position[1], current.position[1], alpha);
        interp.position[2] = cxpr::lerp(previous.position[2], current.position[2], alpha);
    }
}

static void transform_interpolate(float alpha)
{
    auto group = globals::registry.group<TransformComponentIntr>(entt::get<TransformComponent, TransformComponentPrev>);

    for(auto [entity, interp, current, previous] : group.each()) {
        interp.angles[0] = cxpr::lerp(previous.angles[0], current.angles[0], alpha);
        interp.angles[1] = cxpr::lerp(previous.angles[1], current.angles[1], alpha);
        interp.angles[2] = cxpr::lerp(previous.angles[2], current.angles[2], alpha);

        // Figure out previous chunk-local floating-point coordinates transformed
        // to the current WorldCoord's chunk domain coordinates; we're interpolating
        // against these instead of using previous.position.local to prevent jittering
        const auto previous_shift = WorldCoord::to_vec3f(current.position, previous.position);
        const auto previous_local = current.position.local + previous_shift;

        interp.position.chunk[0] = current.position.chunk[0];
        interp.position.chunk[1] = current.position.chunk[1];
        interp.position.chunk[2] = current.position.chunk[2];

        interp.position.local[0] = cxpr::lerp(previous_local[0], current.position.local[0], alpha);
        interp.position.local[1] = cxpr::lerp(previous_local[1], current.position.local[1], alpha);
        interp.position.local[2] = cxpr::lerp(previous_local[2], current.position.local[2], alpha);
    }
}

void interpolation::update(void)
{
    const auto alpha = static_cast<float>(globals::fixed_accumulator) / static_cast<float>(globals::fixed_frametime_us);

    head_interpolate(alpha);

    transform_interpolate(alpha);
}
