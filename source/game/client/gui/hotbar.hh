// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/voxel.hh"

// TODO: design an inventory system and an item
// registry and integrate the hotbar into that system

constexpr static unsigned int HOTBAR_SIZE = 9U;

namespace hotbar
{
extern unsigned int active_slot;
extern Voxel slots[HOTBAR_SIZE];
} // namespace hotbar

namespace hotbar
{
void init(void);
void layout(void);
} // namespace hotbar
