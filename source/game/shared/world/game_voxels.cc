// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/game_voxels.hh"

#include "shared/world/vdef.hh"


VoxelID game_voxels::cobblestone = NULL_VOXEL;
VoxelID game_voxels::dirt = NULL_VOXEL;
VoxelID game_voxels::grass = NULL_VOXEL;
VoxelID game_voxels::stone = NULL_VOXEL;
VoxelID game_voxels::vtest = NULL_VOXEL;
VoxelID game_voxels::vtest_ck = NULL_VOXEL;
VoxelID game_voxels::oak_leaves = NULL_VOXEL;
VoxelID game_voxels::oak_planks = NULL_VOXEL;
VoxelID game_voxels::oak_log = NULL_VOXEL;
VoxelID game_voxels::glass = NULL_VOXEL;
VoxelID game_voxels::slime = NULL_VOXEL;
VoxelID game_voxels::superslime = NULL_VOXEL;
VoxelID game_voxels::mud = NULL_VOXEL;

void game_voxels::populate(void)
{
    // Stone; the backbone of the generated world
    game_voxels::stone = vdef::construct("stone", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/stone_01.png")
        .add_texture_default("textures/voxels/stone_02.png")
        .add_texture_default("textures/voxels/stone_03.png")
        .add_texture_default("textures/voxels/stone_04.png")
        .build();

    // Cobblestone; should drop when a stone is broken, might also be present in surface features
    game_voxels::cobblestone = vdef::construct("cobblestone", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/cobblestone_01.png")
        .add_texture_default("textures/voxels/cobblestone_02.png")
        .build();

    // Dirt with a grass layer on top; the top layer of plains biome
    game_voxels::grass = vdef::construct("grass", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/grass_side_01.png")
        .add_texture_default("textures/voxels/grass_side_02.png")
        .add_texture(VoxelFace::CubeBottom, "textures/voxels/dirt_01.png")
        .add_texture(VoxelFace::CubeBottom, "textures/voxels/dirt_02.png")
        .add_texture(VoxelFace::CubeBottom, "textures/voxels/dirt_03.png")
        .add_texture(VoxelFace::CubeBottom, "textures/voxels/dirt_04.png")
        .add_texture(VoxelFace::CubeTop, "textures/voxels/grass_01.png")
        .add_texture(VoxelFace::CubeTop, "textures/voxels/grass_02.png")
        .build();

    // Dirt; the under-surface layer of some biomes
    game_voxels::dirt = vdef::construct("dirt", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/dirt_01.png")
        .add_texture_default("textures/voxels/dirt_02.png")
        .add_texture_default("textures/voxels/dirt_03.png")
        .add_texture_default("textures/voxels/dirt_04.png")
        .build();

    // VTest; a test voxel to ensure animations work
    game_voxels::vtest = vdef::construct("vtest", VoxelType::Cube, true, false)
        .add_texture_default("textures/voxels/vtest_F1.png")
        .add_texture_default("textures/voxels/vtest_F2.png")
        .add_texture_default("textures/voxels/vtest_F3.png")
        .add_texture_default("textures/voxels/vtest_F4.png")
        .build();

    // VTest-CK; a pure blue chromakey I used to make the game's logo
    game_voxels::vtest_ck = vdef::construct("vtest_ck", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/chromakey.png")
        .build();

    // Oak leaves; greenery. TODO: add trees as surface features
    game_voxels::oak_leaves = vdef::construct("oak_leaves", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/oak_leaves.png")
        .build();

    // Oak planks; the thing that comes out of oak logs
    game_voxels::oak_planks = vdef::construct("oak_planks", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/oak_planks_01.png")
        .add_texture_default("textures/voxels/oak_planks_02.png")
        .build();

    // Oak logs; greenery. TODO: add trees as surface features
    game_voxels::oak_log = vdef::construct("oak_log", VoxelType::Cube, false, false)
        .add_texture_default("textures/voxels/oak_wood_01.png")
        .add_texture_default("textures/voxels/oak_wood_02.png")
        .add_texture(VoxelFace::CubeBottom, "textures/voxels/oak_wood_top.png")
        .add_texture(VoxelFace::CubeTop, "textures/voxels/oak_wood_top.png")
        .build();

    // Glass; blend rendering test
    game_voxels::glass = vdef::construct("glass", VoxelType::Cube, false, true)
        .add_texture_default("textures/voxels/glass_01.png")
        .build();

    // Slime; it's bouncy!
    game_voxels::slime = vdef::construct("slime", VoxelType::Cube, false, true)
        .set_touch(TOUCH_BOUNCE, Vec3f(0.00f, 0.60f, 0.00f))
        .add_texture_default("textures/voxels/slime_01.png")
        .build();

    // Super-slime; a positive feedback loop of bounciness!
    game_voxels::superslime = vdef::construct("superslime", VoxelType::Cube, false, true)
        .set_touch(TOUCH_SINK, Vec3f(0.00f, -1.50f, 0.00f))
        .add_texture_default("textures/voxels/superslime_01.png")
        .build();

    // Mud; you sink in it
    game_voxels::mud = vdef::construct("mud", VoxelType::Cube, false, false)
        .set_touch(TOUCH_SINK, Vec3f(0.50f, 0.75f, 0.50f))
        .add_texture_default("textures/voxels/mud_01.png")
        .add_texture_default("textures/voxels/mud_02.png")
        .build();
}
