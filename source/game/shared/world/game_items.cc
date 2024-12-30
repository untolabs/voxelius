// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/game_items.hh"

#include "shared/world/game_voxels.hh"
#include "shared/world/item_def.hh"


ItemID game_items::stone = NULL_ITEM;
ItemID game_items::cobblestone = NULL_ITEM;
ItemID game_items::dirt = NULL_ITEM;
ItemID game_items::grass = NULL_ITEM;

void game_items::populate(void)
{
    // Stone; a hardened slate rock
    game_items::stone = item_def::construct("stone")
        .set_texture("textures/item/stone.png")
        .set_place_voxel(game_voxels::stone)
        .build();

    // Cobblestone; a bunch of small stones
    game_items::cobblestone = item_def::construct("cobblestone")
        .set_texture("textures/item/cobblestone.png")
        .set_place_voxel(game_voxels::cobblestone)
        .build();

    // Dirt; it's very dirty
    game_items::dirt = item_def::construct("dirt")
        .set_texture("textures/item/dirt.png")
        .set_place_voxel(game_voxels::dirt)
        .build();
    
    // Grass; literally just grassy dirt
    game_items::grass = item_def::construct("grass")
        .set_texture("textures/item/grass.png")
        .set_place_voxel(game_voxels::grass)
        .build();
}
