// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/hull.hh"

#include "shared/entity/gravity.hh"
#include "shared/entity/grounded.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/world/local_coord.hh"
#include "shared/world/vdef.hh"
#include "shared/world/world.hh"

#include "shared/globals.hh"


static int grid_collide(int d, HullComponent &hull, TransformComponent &transform, VelocityComponent &velocity)
{
    const float move = globals::frametime * velocity.linear[d];

    // The template-coordinated bounding box for the
    // collision hull of an entity (most likely the player)
    const auto &box = hull.local_box;

    // The bounding box that we have moved
    // into position to check for voxels inside it
    const auto refbox = Box3f::push(box, transform.position.local);

    // The bounding box that has been moved
    // according to the linear delta value
    auto movbox = refbox;
    movbox.min[d] += move;
    movbox.max[d] += move;

    // The same bounding box except in voxel
    // grid coordinates; this encapsulates all
    // the voxels potentially inside 
    Box3base<LocalCoord::value_type> localbox;
    localbox.min[0] = cxpr::floor<LocalCoord::value_type>(movbox.min[0]);
    localbox.min[1] = cxpr::floor<LocalCoord::value_type>(movbox.min[1]);
    localbox.min[2] = cxpr::floor<LocalCoord::value_type>(movbox.min[2]);
    localbox.max[0] = cxpr::ceil<LocalCoord::value_type>(movbox.max[0]);
    localbox.max[1] = cxpr::ceil<LocalCoord::value_type>(movbox.max[1]);
    localbox.max[2] = cxpr::ceil<LocalCoord::value_type>(movbox.max[2]);

    // Other axes
    const int u = (d + 1) % 3;
    const int v = (d + 2) % 3;

    LocalCoord::value_type ddir;
    LocalCoord::value_type dmin;
    LocalCoord::value_type dmax;

    if(move < 0.0f) {
        ddir = LocalCoord::value_type(+1);
        dmin = localbox.min[d];
        dmax = localbox.max[d];
    }
    else {
        ddir = LocalCoord::value_type(-1);
        dmin = localbox.max[d];
        dmax = localbox.min[d];
    }

    for(auto i = dmin; i != dmax; i += ddir) {
        for(auto j = localbox.min[u]; j <= localbox.max[u]; ++j)
        for(auto k = localbox.min[v]; k <= localbox.max[v]; ++k) {
            LocalCoord lpos;
            lpos[d] = i;
            lpos[u] = j;
            lpos[v] = k;

            const auto vpos = ChunkCoord::to_voxel(transform.position.chunk, lpos);
            const auto info = vdef::find(world::get_voxel(vpos));

            if(info == nullptr) {
                // Don't collide with something
                // that we assume to be nothing
                continue;
            }

            Box3f vbox;
            vbox.min = Vec3f(lpos);
            vbox.max = Vec3f(lpos) + 1.0f;

            if(!Box3f::intersect(movbox, vbox)) {
                // No intersection between the voxel
                // and the entity's collision hull
                continue;
            }

            velocity.linear[d] = 0.0f;

            return cxpr::sign<int>(move);
        }
    }

    return 0;
}

void HullComponent::update(void)
{
    // FIXME: this isn't particularly accurate considering
    // some voxels might be passable and some other voxels
    // might apply some slowing factor; what I might do in the
    // future is to add a specific value to the voxel registry
    // entries that would specify the amount of force we apply
    // to prevent player movement inside a specific voxel, plus
    // we shouldn't treat all voxels as full cubes if we want
    // to support slabs, stairs and non-full liquid voxels in the future

    auto group = globals::registry.group<HullComponent>(entt::get<TransformComponent, VelocityComponent>);

    for(auto [entity, hull, transform, velocity] : group.each()) {
        if(grid_collide(1, hull, transform, velocity) == (-cxpr::sign<int>(GravityComponent::acceleration)))
            globals::registry.emplace_or_replace<GroundedComponent>(entity);
        else globals::registry.remove<GroundedComponent>(entity);

        grid_collide(0, hull, transform, velocity);
        grid_collide(2, hull, transform, velocity);
    }
}
