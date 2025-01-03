// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/sound/listener.hh"

#include "shared/entity/velocity.hh"

#include "client/sound/sound.hh"

#include "client/globals.hh"
#include "client/view.hh"


void listener::update(void)
{
    if(globals::registry.valid(globals::player)) {
        auto &velocity = globals::registry.get<VelocityComponent>(globals::player).linear;
        auto &position = view::position.local;

        alListener3f(AL_POSITION, position.get_x(), position.get_y(), position.get_z());
        alListener3f(AL_VELOCITY, velocity.get_x(), velocity.get_y(), velocity.get_z());

        float orientation[6];
        orientation[0] = view::direction.get_x();
        orientation[1] = view::direction.get_y();
        orientation[2] = view::direction.get_z();
        orientation[3] = Vec3f::dir_up().get_x();
        orientation[4] = Vec3f::dir_up().get_y();
        orientation[5] = Vec3f::dir_up().get_z();

        alListenerfv(AL_ORIENTATION, orientation);
    }

    alListenerf(AL_GAIN, sound::master_volume * 0.01f);
}
