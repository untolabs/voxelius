// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/item_def.hh"

#include "shared/world/voxel_def.hh"


std::unordered_map<std::string, ItemInfoBuilder> item_def::builders = {};
std::unordered_map<std::string, ItemID> item_def::names = {};
std::vector<std::shared_ptr<ItemInfo>> item_def::items = {};

ItemInfoBuilder::ItemInfoBuilder(const std::string &name)
{
    prototype.name = name;
    prototype.texture = std::string();
    prototype.place_voxel = NULL_VOXEL;
    prototype.cached_texture = nullptr;
}

ItemInfoBuilder &ItemInfoBuilder::set_texture(const std::string &texture)
{
    prototype.texture = texture;
    prototype.cached_texture = nullptr;
    return *this;
}

ItemInfoBuilder &ItemInfoBuilder::set_place_voxel(VoxelID place_voxel)
{
    prototype.place_voxel = place_voxel;
    return *this;
}

ItemID ItemInfoBuilder::build(void) const
{
    const auto it = item_def::names.find(prototype.name);

    if(it != item_def::names.cend()) {
        spdlog::warn("item_def: cannot build {}: name already present", prototype.name);
        return it->second;
    }

    auto new_info = std::make_shared<ItemInfo>();
    new_info->name = prototype.name;
    new_info->texture = prototype.texture;
    new_info->place_voxel = prototype.place_voxel;
    new_info->cached_texture = nullptr;

    item_def::items.push_back(new_info);
    item_def::names.insert_or_assign(prototype.name, static_cast<ItemID>(item_def::items.size()));

    return static_cast<ItemID>(item_def::items.size());
}

ItemInfoBuilder &item_def::construct(const std::string &name)
{
    const auto it = item_def::builders.find(name);
    if(it != item_def::builders.cend())
        return it->second;
    return item_def::builders.emplace(name, ItemInfoBuilder(name)).first->second;
}

ItemInfo *item_def::find(const std::string &name)
{
    const auto it = item_def::names.find(name);
    if(it != item_def::names.cend())
        return item_def::find(it->second);
    return nullptr;
}

ItemInfo *item_def::find(const ItemID item)
{
    if((item != NULL_ITEM) && (item <= item_def::items.size()))
        return item_def::items[item - 1].get();
    return nullptr;
}

void item_def::purge(void)
{
    item_def::builders.clear();
    item_def::names.clear();
    item_def::items.clear();
}
