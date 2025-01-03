// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/resource/sound_effect.hh"

#include "common/fstools.hh"


static emhash8::HashMap<std::string, std::shared_ptr<const SoundEffect>> sfx_map = {};

static std::size_t drwav_read_physfs(void *file, void *output, std::size_t count)
{
    return static_cast<std::size_t>(PHYSFS_readBytes(reinterpret_cast<PHYSFS_File *>(file), output, count));
}

static drwav_bool32 drwav_seek_physfs(void *file, int offset, drwav_seek_origin origin)
{
    if(origin == drwav_seek_origin_current)
        return PHYSFS_seek(reinterpret_cast<PHYSFS_File *>(file), PHYSFS_tell(reinterpret_cast<PHYSFS_File *>(file)) + offset);
    return PHYSFS_seek(reinterpret_cast<PHYSFS_File *>(file), offset);
}

template<>
std::shared_ptr<const SoundEffect> resource::load<SoundEffect>(const std::string &name, unsigned int load_flags)
{
    const auto it = sfx_map.find(name);

    if(it != sfx_map.cend()) {
        return it->second;
    }

    PHYSFS_File *file = PHYSFS_openRead(name.c_str());

    if(file == nullptr) {
        spdlog::warn("sound_effect: {}: {}", name, fstools::error());
        return nullptr;
    }

    drwav wav_info;

    if(!drwav_init(&wav_info, &drwav_read_physfs, &drwav_seek_physfs, file, nullptr)) {
        spdlog::warn("sound_effect: {}: drwav_init failed", name);
        PHYSFS_close(file);
        return nullptr;
    }

    if(wav_info.channels != 1) {
        spdlog::warn("sound_effect: {}: only mono sound files are allowed", name);
        drwav_uninit(&wav_info);
        PHYSFS_close(file);
        return nullptr;
    }

    auto data = new ALshort[wav_info.totalPCMFrameCount];
    auto count = drwav_read_pcm_frames_s16(&wav_info, wav_info.totalPCMFrameCount, reinterpret_cast<drwav_int16 *>(data));
    auto sample_rate = static_cast<ALsizei>(wav_info.sampleRate);
    auto length = static_cast<ALsizei>(count * sizeof(ALshort));

    drwav_uninit(&wav_info);
    PHYSFS_close(file);

    auto new_sfx = std::make_shared<SoundEffect>();
    new_sfx->name = std::string(name);

    alGenBuffers(1, &new_sfx->buffer);
    alBufferData(new_sfx->buffer, AL_FORMAT_MONO16, data, length, sample_rate);

    delete[] data;

    return sfx_map.insert_or_assign(name, new_sfx).first->second;
}

template<>
void resource::hard_cleanup<SoundEffect>(void)
{
    for(const auto &sfx : sfx_map) {
        if(sfx.second.use_count() > 1L)
            spdlog::warn("sound_effect: hard_cleanup: zombie resource {} [use_count={}]", sfx.first, sfx.second.use_count());
        spdlog::debug("sound_effect: hard_cleanup: purging {}", sfx.first);
        alDeleteBuffers(1, &sfx.second->buffer);
    }

    sfx_map.clear();
}

template<>
void resource::soft_cleanup<SoundEffect>(void)
{
    auto sfx = sfx_map.cbegin();

    while(sfx != sfx_map.cend()) {
        if(sfx->second.use_count() == 1L) {
            spdlog::debug("sound_effect: soft_cleanup: unloading {}", sfx->first);
            alDeleteBuffers(1, &sfx->second->buffer);
            sfx = sfx_map.erase(sfx);
            continue;
        }

        sfx++;
    }
}
