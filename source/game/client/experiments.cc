// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/experiments.hh"

#if ENABLE_EXPERIMENTS

#include "shared/world/game_items.hh"
#include "shared/world/game_voxels.hh"
#include "shared/world/item_def.hh"
#include "shared/world/world.hh"

#include "client/entity/player_target.hh"

#include "client/event/glfw_mouse_button.hh"
#include "client/event/glfw_scroll.hh"

#include "client/gui/chat.hh"

#include "client/hud/hotbar.hh"
#include "client/hud/status_lines.hh"

#include "client/globals.hh"


static void do_remove_voxel(void)
{
    world::set_voxel(NULL_VOXEL, player_target::vvec);
}

static void do_place_voxel(void)
{
    if(auto info = item_def::find(hotbar::slots[hotbar::active_slot])) {
        if(info->place_voxel != NULL_VOXEL)
            world::set_voxel(info->place_voxel, player_target::vvec + player_target::vnormal);
        return;
    }
}

static void on_glfw_mouse_button(const GlfwMouseButtonEvent &event)
{
    if(!globals::gui_screen && globals::registry.valid(globals::player)) {
        if((event.action == GLFW_PRESS) && (player_target::voxel != NULL_VOXEL)) {
            if(event.button == GLFW_MOUSE_BUTTON_LEFT) {
                do_remove_voxel();
                return;
            }

            if(event.button == GLFW_MOUSE_BUTTON_RIGHT) {
                do_place_voxel();
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
    hotbar::slots[0] = game_items::cobblestone;
    hotbar::slots[1] = game_items::stone;
    hotbar::slots[2] = game_items::dirt;
    hotbar::slots[3] = game_items::grass;
    hotbar::slots[4] = game_items::oak_leaves;
    hotbar::slots[5] = game_items::oak_planks;
    hotbar::slots[6] = game_items::oak_log;
    hotbar::slots[7] = game_items::glass;
    hotbar::slots[8] = game_items::slime;
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
