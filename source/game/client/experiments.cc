// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/experiments.hh"

#if ENABLE_EXPERIMENTS

#include "shared/world/game_voxels.hh"
#include "shared/world/world.hh"

#include "client/event/glfw_mouse_button.hh"
#include "client/event/glfw_scroll.hh"

#include "client/gui/chat.hh"

#include "client/hud/hotbar.hh"

#include "client/world/player_target.hh"

#include "client/globals.hh"


static void on_glfw_mouse_button(const GlfwMouseButtonEvent &event)
{
    if(!globals::gui_screen && globals::registry.valid(globals::player)) {
        if((event.action == GLFW_PRESS) && (player_target::voxel != NULL_VOXEL)) {
            if(event.button == GLFW_MOUSE_BUTTON_LEFT) {
                world::set_voxel(NULL_VOXEL, player_target::vvec);
                return;
            }

            if(event.button == GLFW_MOUSE_BUTTON_RIGHT) {
                if(hotbar::slots[hotbar::active_slot] != NULL_VOXEL)
                    world::set_voxel(hotbar::slots[hotbar::active_slot], player_target::vvec + player_target::vnormal);
                return;
            }

            if(event.button == GLFW_MOUSE_BUTTON_MIDDLE) {
                if(player_target::voxel != NULL_VOXEL)
                    hotbar::slots[hotbar::active_slot] = player_target::voxel;
                return;
            }
        }
    }
}

void experiments::init(void)
{
    globals::dispatcher.sink<GlfwMouseButtonEvent>().connect<&on_glfw_mouse_button>();
}

void experiments::init_late(void)
{
    hotbar::slots[0] = game_voxels::cobblestone;
    hotbar::slots[1] = game_voxels::dirt;
    hotbar::slots[2] = game_voxels::grass;
    hotbar::slots[3] = game_voxels::stone;
    hotbar::slots[4] = game_voxels::vtest;
    hotbar::slots[5] = game_voxels::slime;
    hotbar::slots[6] = game_voxels::superslime;
    hotbar::slots[7] = game_voxels::mud;
    hotbar::slots[8] = game_voxels::glass;
}

void experiments::deinit(void)
{

}

void experiments::update(void)
{

}

void experiments::update_late(void)
{

}

#endif /* ENABLE_EXPERIMENTS */
