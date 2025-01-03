// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/sound/sound.hh"

#include "mathlib/constexpr.hh"

#include "common/config.hh"

#include "shared/world/voxel_coord.hh"
#include "shared/world/world_coord.hh"

#include "shared/protocol.hh"

#include "client/entity/sound_emitter.hh"

#include "client/gui/settings.hh"

#include "client/resource/sound_effect.hh"

#include "client/world/outline.hh"

#include "client/globals.hh"
#include "client/session.hh"
#include "client/view.hh"


float sound::master_volume = 100.0f;
float sound::effects_volume = 100.0f;
float sound::music_volume = 100.0f;

static ALuint generic_source = {};
static ALuint player_source = {};

static std::shared_ptr<const SoundEffect> generic_sound = {};
static std::shared_ptr<const SoundEffect> player_sound = {};

void sound::init(void)
{
    Config::add(globals::client_config, "sound.master_volume", sound::master_volume);
    Config::add(globals::client_config, "sound.effects_volume", sound::effects_volume);
    Config::add(globals::client_config, "sound.music_volume", sound::music_volume);

    settings::add_slider(1, settings::SOUND, "sound.master_volume", sound::master_volume, 0.0f, 100.0f, false, "%.0f%%");
    settings::add_slider(2, settings::SOUND, "sound.effects_volume", sound::effects_volume, 0.0f, 100.0f, false, "%.0f%%");
    settings::add_slider(3, settings::SOUND, "sound.music_volume", sound::music_volume, 0.0f, 100.0f, false, "%.0f%%");

    alGenSources(1, &generic_source);
    alSourcei(generic_source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(generic_source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(generic_source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    alGenSources(1, &player_source);
    alSourcei(player_source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(player_source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(player_source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    generic_sound = nullptr;
    player_sound = nullptr;
}

void sound::init_late(void)
{

}

void sound::deinit(void)
{
    generic_sound = nullptr;
    player_sound = nullptr;

    alDeleteSources(1, &generic_source);
    alDeleteSources(1, &player_source);
}

void sound::update(void)
{
    sound::master_volume = cxpr::clamp(sound::master_volume, 0.0f, 100.0f);
    sound::effects_volume = cxpr::clamp(sound::effects_volume, 0.0f, 100.0f);
    sound::music_volume = cxpr::clamp(sound::music_volume, 0.0f, 100.0f);
}

void sound::render(void)
{

}

void sound::play_generic(const std::string &sound, bool looping, float pitch, float gain)
{
    if(sound.empty()) {
        alSourceRewind(generic_source);
        generic_sound = nullptr;
        return;
    }

    generic_sound = resource::load<SoundEffect>(sound);

    if(generic_sound == nullptr) {
        alSourceRewind(generic_source);
        return;
    }

    alSourcei(generic_source, AL_BUFFER, generic_sound->buffer);
    alSourcei(generic_source, AL_LOOPING, looping);
    alSourcef(generic_source, AL_PITCH, cxpr::clamp(pitch, 0.0625f, 10.0f));
    alSourcef(generic_source, AL_GAIN, cxpr::clamp(gain, 0.0625f, 1.0f));
    alSourcePlay(generic_source);
}

void sound::play_entity(entt::entity entity, const std::string &sound, bool looping, float pitch, float gain)
{
    if(globals::registry.valid(entity)) {
        if(auto emitter = globals::registry.try_get<SoundEmitterComponent>(entity)) {
            if(sound.empty()) {
                alSourceRewind(emitter->source);
                emitter->sound = nullptr;
                return;
            }

            emitter->sound = resource::load<SoundEffect>(sound);

            if(emitter->sound == nullptr) {
                alSourceRewind(emitter->source);
                return;
            }

            alSourcei(emitter->source, AL_BUFFER, emitter->sound->buffer);
            alSourcei(emitter->source, AL_LOOPING, looping);
            alSourcef(emitter->source, AL_PITCH, cxpr::clamp(pitch, 0.0625f, 10.0f));
            alSourcef(emitter->source, AL_GAIN, cxpr::clamp(gain, 0.0625f, 1.0f));
            alSourcePlay(emitter->source);
        }
    }
}

void sound::play_player(const std::string &sound, bool looping, float pitch, float gain)
{
    if(session::peer && globals::registry.valid(globals::player)) {
        protocol::EntitySound packet = {};
        packet.entity = globals::player;
        packet.sound = std::string(sound);
        packet.looping = looping;
        packet.pitch = pitch;
        packet.gain = gain;

        protocol::send(session::peer, nullptr, packet);
    }

    if(sound.empty()) {
        alSourceRewind(generic_source);
        generic_sound = nullptr;
        return;
    }

    generic_sound = resource::load<SoundEffect>(sound);

    if(generic_sound == nullptr) {
        alSourceRewind(generic_source);
        return;
    }

    alSourcei(generic_source, AL_BUFFER, generic_sound->buffer);
    alSourcei(generic_source, AL_LOOPING, looping);
    alSourcef(generic_source, AL_PITCH, cxpr::clamp(pitch, 0.0625f, 10.0f));
    alSourcef(generic_source, AL_GAIN, cxpr::clamp(gain, 0.0625f, 1.0f));
    alSourcePlay(generic_source);
}
