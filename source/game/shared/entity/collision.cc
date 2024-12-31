// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/entity/collision.hh"

#include "shared/entity/gravity.hh"
#include "shared/entity/grounded.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "shared/world/local_coord.hh"
#include "shared/world/voxel_def.hh"
#include "shared/world/world.hh"

#include "shared/globals.hh"


static int vgrid_collide(int d, CollisionComponent &collision, TransformComponent &transform, VelocityComponent &velocity)
{
    const auto move = globals::fixed_frametime * velocity.linear[d];
    const auto move_sign = cxpr::sign<int>(move);

    const auto &ref_hull = collision.hull;
    const auto current_hull = Box3f::push(ref_hull, transform.position.local);

    auto next_hull = current_hull;
    next_hull.min[d] += move;
    next_hull.max[d] += move;

    Box3base<LocalCoord::value_type> lpos_hull;
    lpos_hull.min[0] = cxpr::floor<LocalCoord::value_type>(next_hull.min[0]);
    lpos_hull.min[1] = cxpr::floor<LocalCoord::value_type>(next_hull.min[1]);
    lpos_hull.min[2] = cxpr::floor<LocalCoord::value_type>(next_hull.min[2]);
    lpos_hull.max[0] = cxpr::ceil<LocalCoord::value_type>(next_hull.max[0]);
    lpos_hull.max[1] = cxpr::ceil<LocalCoord::value_type>(next_hull.max[1]);
    lpos_hull.max[2] = cxpr::ceil<LocalCoord::value_type>(next_hull.max[2]);

    // Other axes
    const int u = (d + 1) % 3;
    const int v = (d + 2) % 3;

    LocalCoord::value_type ddir;
    LocalCoord::value_type dmin;
    LocalCoord::value_type dmax;

    if(move < 0.0f) {
        ddir = LocalCoord::value_type(+1);
        dmin = lpos_hull.min[d];
        dmax = lpos_hull.max[d];
    }
    else {
        ddir = LocalCoord::value_type(-1);
        dmin = lpos_hull.max[d];
        dmax = lpos_hull.min[d];
    }

    VoxelTouch latch_touch = TOUCH_NOTHING;
    Vec3f latch_multipliers = Vec3f::zero();
    Box3f latch_vbox = {};

    for(auto i = dmin; i != dmax; i += ddir) {
        for(auto j = lpos_hull.min[u]; j < lpos_hull.max[u]; ++j)
        for(auto k = lpos_hull.min[v]; k < lpos_hull.max[v]; ++k) {
            LocalCoord lpos;
            lpos[d] = i;
            lpos[u] = j;
            lpos[v] = k;

            const auto vpos = ChunkCoord::to_voxel(transform.position.chunk, lpos);
            const auto info = voxel_def::find(world::get_voxel(vpos));

            if(info == nullptr) {
                // Don't collide with something
                // that we assume to be nothing
                continue;
            }

            Box3f vbox;
            vbox.min = Vec3f(lpos);
            vbox.max = Vec3f(lpos + 1);

            if(!Box3f::intersect(next_hull, vbox)) {
                // No intersection between the voxel
                // and the entity's collision hull
                continue;
            }

            if(info->touch_type == TOUCH_SOLID) {
                // Solid touch type makes a collision
                // response whenever it is encountered
                velocity.linear[d] = 0.0f;
                return move_sign;
            }

            // In case of other touch types, they
            // are latched and the last ever touch
            // type is then responded to
            if(info->touch_type != TOUCH_NOTHING) {
                latch_touch = info->touch_type;
                latch_multipliers = info->touch_multipliers;
                latch_vbox = vbox;
                continue;
            }
        }
    }

    if(latch_touch != TOUCH_NOTHING) {
        if(latch_touch == TOUCH_BOUNCE) {
            const auto move_distance = cxpr::abs(current_hull.min[d] - next_hull.min[d]);
            const auto threshold = 2.0f * globals::fixed_frametime;

            if(move_distance > threshold)
                velocity.linear[d] *= -latch_multipliers[d];
            else velocity.linear[d] = 0.0f;

            return move_sign;
        }

        if(latch_touch == TOUCH_SINK) {
            velocity.linear[d] *= latch_multipliers[d];
            return move_sign;
        }
    }

    return 0;
}

void CollisionComponent::fixed_update(void)
{
    // FIXME: this isn't particularly accurate considering
    // some voxels might be passable and some other voxels
    // might apply some slowing factor; what I might do in the
    // future is to add a specific value to the voxel registry
    // entries that would specify the amount of force we apply
    // to prevent player movement inside a specific voxel, plus
    // we shouldn't treat all voxels as full cubes if we want
    // to support slabs, stairs and non-full liquid voxels in the future

    auto group = globals::registry.group<CollisionComponent>(entt::get<TransformComponent, VelocityComponent>);

    for(auto [entity, collision, transform, velocity] : group.each()) {
        if(vgrid_collide(1, collision, transform, velocity) == (-cxpr::sign<int>(GravityComponent::acceleration)))
            globals::registry.emplace_or_replace<GroundedComponent>(entity);
        else globals::registry.remove<GroundedComponent>(entity);

        vgrid_collide(0, collision, transform, velocity);
        vgrid_collide(2, collision, transform, velocity);
    }
}
