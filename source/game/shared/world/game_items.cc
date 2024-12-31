// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/game_items.hh"

#include "shared/world/game_voxels.hh"
#include "shared/world/item_def.hh"


ItemID game_items::stone = NULL_ITEM;
ItemID game_items::cobblestone = NULL_ITEM;
ItemID game_items::dirt = NULL_ITEM;
ItemID game_items::grass = NULL_ITEM;
ItemID game_items::oak_leaves = NULL_ITEM;
ItemID game_items::oak_planks = NULL_ITEM;
ItemID game_items::oak_log = NULL_ITEM;
ItemID game_items::glass = NULL_ITEM;
ItemID game_items::slime = NULL_ITEM;
ItemID game_items::mud = NULL_ITEM;

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

    // Oak leaves; they're bushy!
    game_items::oak_leaves = item_def::construct("oak_leaves")
        .set_texture("textures/item/oak_leaves.png")
        .set_place_voxel(game_voxels::oak_leaves)
        .build();

    // Oak planks; watch for splinters!
    game_items::oak_planks = item_def::construct("oak_planks")
        .set_texture("textures/item/oak_planks.png")
        .set_place_voxel(game_voxels::oak_planks)
        .build();

    // Oak log; a big wad of wood
    game_items::oak_log = item_def::construct("oak_log")
        .set_texture("textures/item/oak_log.png")
        .set_place_voxel(game_voxels::oak_log)
        .build();

    // Glass; used for windowing
    game_items::glass = item_def::construct("glass")
        .set_texture("textures/item/glass.png")
        .set_place_voxel(game_voxels::glass)
        .build();

    // Slime; it's bouncy!
    game_items::slime = item_def::construct("slime")
        .set_texture("textures/item/slime.png")
        .set_place_voxel(game_voxels::slime)
        .build();

    // Mud; you sink in it!
    game_items::mud = item_def::construct("mud")
        .set_texture("textures/item/mud.png")
        .build();
}
