// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "client/resource/sound_effect.hh"

class SoundEmitterComponent final {
public:
    // Sound effect resource handle should
    // be held here in case sounds are not precached
    // and are to be nuked by the resource system
    std::shared_ptr<const SoundEffect> sound {};
    ALuint source {};

public:
    explicit SoundEmitterComponent(void);
    virtual ~SoundEmitterComponent(void);

public:
    static void update(void);
};
