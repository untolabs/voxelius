// SPDX-License-Identifier: BSD-2-Clause
#pragma once

#include "common/resource/resource.hh"

struct SoundEffect final : public Resource {
    ALuint buffer {};
};
