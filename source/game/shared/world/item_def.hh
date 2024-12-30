// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "shared/world/item_id.hh"
#include "shared/world/voxel_id.hh"

// This resource is defined only
// client-side and its pointers in
// the shared code shall remain null
struct Texture2D;

struct ItemInfo final {
    std::string name {};
    std::string texture {};

    // A voxel ID to pass into world::set_voxel
    // whenever the item in question is selected
    // in the client-side hotbar; this has no checks inside!
    VoxelID place_voxel {};

    // NOTE: this is only to be used client-side
    // to retain a cached resource handle for that
    // specific item; anywhere else (eg. server-side)
    // this handle must be nullptr and not to be used
    std::shared_ptr<const Texture2D> cached_texture {};
};

class ItemInfoBuilder final {
public:
    explicit ItemInfoBuilder(const std::string &name);
    virtual ~ItemInfoBuilder(void) = default;

public:
    ItemInfoBuilder &set_texture(const std::string &texture);
    ItemInfoBuilder &set_place_voxel(VoxelID place_voxel);

public:
    ItemID build(void) const;

private:
    ItemInfo prototype;
};

namespace item_def
{
extern std::unordered_map<std::string, ItemInfoBuilder> builders;
extern std::unordered_map<std::string, ItemID> names;
extern std::vector<std::shared_ptr<ItemInfo>> items;
} // namespace item_def

namespace item_def
{
ItemInfoBuilder &construct(const std::string &name);
ItemInfo *find(const std::string &name);
ItemInfo *find(const ItemID item);
} // namespace item_def

namespace item_def
{
void purge(void);
} // namespace item_def
