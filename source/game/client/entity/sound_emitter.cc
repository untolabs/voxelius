// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/entity/sound_emitter.hh"

#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "client/sound/sound.hh"

#include "client/globals.hh"
#include "client/view.hh"


SoundEmitterComponent::SoundEmitterComponent(void)
{
    sound = nullptr;
    alGenSources(1, &source);
}

SoundEmitterComponent::~SoundEmitterComponent(void)
{
    alSourceStop(source);
    alDeleteSources(1, &source);
}

void SoundEmitterComponent::update(void)
{
    const auto view = globals::registry.view<SoundEmitterComponent>();

    // Pivot coordinate for calculating relative
    // sound positions for spacial audio effects
    const auto &pivot = view::position.chunk;

    // The sound::effects_volume value stores gain in a GUI-friendly
    // format of percentage; to avoid per-entity calculations we figure
    // out the fractional value here and set it to sources later
    auto effects_gain = sound::effects_volume * 0.01f;

    for(const auto [entity, emitter] : view.each()) {
        alSourcef(emitter.source, AL_GAIN, effects_gain);

        // Update source positions
        if(const auto *transform = globals::registry.try_get<TransformComponentIntr>(entity)) {
            auto position = ChunkCoord::to_vec3f(pivot, transform->position);
            alSourcefv(emitter.source, AL_POSITION, position.data());
        }

        // Update source velocity
        if(const auto *velocity = globals::registry.try_get<VelocityComponent>(entity)) {
            alSourcefv(emitter.source, AL_VELOCITY, velocity->linear.data());
        }

        ALint source_state;
        alGetSourcei(emitter.source, AL_SOURCE_STATE, &source_state);

        if(source_state == AL_STOPPED) {
            alSourceRewind(emitter.source);
            emitter.sound = nullptr;
            continue;
        }
    }
}
