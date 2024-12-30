// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/voxel_def.hh"

#include "common/crc64.hh"
#include "common/fstools.hh"


std::unordered_map<std::string, VoxelInfoBuilder> voxel_def::builders = {};
std::unordered_map<std::string, VoxelID> voxel_def::names = {};
std::vector<std::shared_ptr<VoxelInfo>> voxel_def::voxels = {};

VoxelInfoBuilder::VoxelInfoBuilder(const std::string &name, VoxelType type, bool animated, bool blending)
{
    prototype.name = name;
    prototype.type = type;
    prototype.animated = animated;
    prototype.blending = blending;

    switch(type) {
        case VoxelType::Cube:
            prototype.textures.resize(static_cast<std::size_t>(VoxelFace::CubeCount));
            break;
        case VoxelType::Slab:
            prototype.textures.resize(static_cast<std::size_t>(VoxelFace::SlabCount));
            break;
        case VoxelType::Stairs:
            prototype.textures.resize(static_cast<std::size_t>(VoxelFace::StairCount));
            break;
        case VoxelType::VModel:
        case VoxelType::DModel:
            // Custom models should use a different texture
            // resource management that is not a voxel atlas
            // TODO: actually implement custom models lol
            prototype.textures.resize(0);
            break;
        default:
            // Something really bad should happen if we end up here.
            // The outside code would static_cast an int to VoxelType
            // and possibly fuck a lot of things up to cause this
            spdlog::critical("voxel_def: {}: unknown voxel type {}", name, static_cast<int>(type));
            std::terminate();
    }

    // Physics properties
    prototype.touch_type = TOUCH_SOLID;
    prototype.touch_multipliers = Vec3f::zero();

    // Things set in future by item_def
    prototype.pick_item_id = NULL_ITEM;
}

VoxelInfoBuilder &VoxelInfoBuilder::add_texture_default(const std::string &texture)
{
    default_texture.paths.push_back(texture);
    return *this;
}

VoxelInfoBuilder &VoxelInfoBuilder::add_texture(VoxelFace face, const std::string &texture)
{
    const auto index = static_cast<std::size_t>(face);
    prototype.textures[index].paths.push_back(texture);
    return *this;
}

VoxelInfoBuilder &VoxelInfoBuilder::set_touch(VoxelTouch type, const Vec3f &multipliers)
{
    prototype.touch_type = type;
    prototype.touch_multipliers = multipliers;
    return *this;
}

VoxelID VoxelInfoBuilder::build(void) const
{
    const auto it = voxel_def::names.find(prototype.name);

    if(it != voxel_def::names.cend()) {
        spdlog::warn("voxel_def: cannot build {}: name already present", prototype.name);
        return it->second;
    }

    std::size_t state_count;

    switch(prototype.type) {
        case VoxelType::Cube:
        case VoxelType::Cross:
        case VoxelType::VModel:
        case VoxelType::DModel:
            state_count = 1;
            break;
        case VoxelType::Slab:
            state_count = 3;
            break;
        case VoxelType::Stairs:
            state_count = 8;
            break;
        default:
            // Something really bad should happen if we end up here.
            // The outside code would static_cast an int to VoxelType
            // and possibly fuck a lot of things up to cause this
            spdlog::critical("voxel_def: {}: unknown voxel type {}", prototype.name, static_cast<int>(prototype.type));
            std::terminate();
    }

    if((voxel_def::voxels.size() + state_count) >= VOXEL_MAX) {
        spdlog::critical("voxel_def: voxel registry overflow");
        std::terminate();
    }

    auto new_info = std::make_shared<VoxelInfo>();
    new_info->name = prototype.name;
    new_info->type = prototype.type;
    new_info->animated = prototype.animated;
    new_info->blending = prototype.blending;

    new_info->textures.resize(prototype.textures.size());

    for(std::size_t i = 0; i < prototype.textures.size(); ++i) {
        if(prototype.textures[i].paths.empty()) {
            new_info->textures[i].paths = default_texture.paths;
            new_info->textures[i].cached_offset = SIZE_MAX;
            new_info->textures[i].cached_plane = SIZE_MAX;
        }
        else {
            new_info->textures[i].paths = prototype.textures[i].paths;
            new_info->textures[i].cached_offset = SIZE_MAX;
            new_info->textures[i].cached_plane = SIZE_MAX;
        }
    }

    // Physics properties
    new_info->touch_type = prototype.touch_type;
    new_info->touch_multipliers = prototype.touch_multipliers;

    // Things set in future by item_def
    new_info->pick_item_id = prototype.pick_item_id;

    // Base voxel identifier offset
    new_info->base = voxel_def::voxels.size() + 1;

    for(std::size_t i = 0; i < state_count; ++i)
        voxel_def::voxels.push_back(new_info);
    voxel_def::names.insert_or_assign(new_info->name, new_info->base);

    return new_info->base;
}

VoxelInfoBuilder &voxel_def::construct(const std::string &name, VoxelType type, bool animated, bool blending)
{
    const auto it = voxel_def::builders.find(name);
    if(it != voxel_def::builders.cend())
        return it->second;
    return voxel_def::builders.emplace(name, VoxelInfoBuilder(name, type, animated, blending)).first->second;
}

VoxelInfo *voxel_def::find(const std::string &name)
{
    const auto it = voxel_def::names.find(name);
    if(it != voxel_def::names.cend())
        return voxel_def::find(it->second);
    return nullptr;
}

VoxelInfo *voxel_def::find(const VoxelID voxel)
{
    if((voxel != NULL_VOXEL) && (voxel <= voxel_def::voxels.size()))
        return voxel_def::voxels[voxel - 1].get();
    return nullptr;
}

void voxel_def::purge(void)
{
    voxel_def::builders.clear();
    voxel_def::names.clear();
    voxel_def::voxels.clear();
}

std::uint64_t voxel_def::calc_checksum(void)
{
    std::uint64_t result = 0;

    for(const std::shared_ptr<VoxelInfo> &info : voxels) {
        result = crc64::get(info->name, result);
        result += static_cast<std::uint64_t>(info->type);
        result += static_cast<std::uint64_t>(info->base);
        result += info->blending ? 256 : 1;
    }

    return result;
}
