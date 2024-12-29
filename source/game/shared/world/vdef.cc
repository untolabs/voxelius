// SPDX-License-Identifier: BSD-2-Clause
#include "shared/precompiled.hh"
#include "shared/world/vdef.hh"

#include "common/crc64.hh"
#include "common/fstools.hh"


std::unordered_map<std::string, VoxelInfoBuilder> vdef::builders = {};
std::unordered_map<std::string, VoxelID> vdef::names = {};
std::vector<std::shared_ptr<VoxelInfo>> vdef::voxels = {};

VoxelInfoBuilder::VoxelInfoBuilder(const std::string &name, VoxelType type, bool animated, bool blending)
{
    prototype.name = name;
    prototype.type = type;
    prototype.animated = animated;
    prototype.blending = blending;
    prototype.touch_type = TOUCH_SOLID;
    prototype.touch_coeffs = Vec3f(0.0f, 0.0f, 0.0f);

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
            spdlog::critical("vdef: {}: unknown voxel type {}", name, static_cast<int>(type));
            std::terminate();
    }
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

VoxelInfoBuilder &VoxelInfoBuilder::set_touch(VoxelTouch type, const Vec3f &coeffs)
{
    prototype.touch_type = type;
    prototype.touch_coeffs = coeffs;
    return *this;
}

VoxelID VoxelInfoBuilder::build(void) const
{
    const auto it = vdef::names.find(prototype.name);

    if(it != vdef::names.cend()) {
        spdlog::warn("vdef: cannot build {}: name already present", prototype.name);
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
            spdlog::critical("vdef: {}: unknown voxel type {}", prototype.name, static_cast<int>(prototype.type));
            std::terminate();
    }

    if((vdef::voxels.size() + state_count) >= VOXEL_MAX) {
        spdlog::critical("vdef: voxel registry overflow");
        std::terminate();
    }

    auto new_info = std::make_shared<VoxelInfo>();
    new_info->name = prototype.name;
    new_info->type = prototype.type;
    new_info->animated = prototype.animated;
    new_info->blending = prototype.blending;
    new_info->touch_type = prototype.touch_type;
    new_info->touch_coeffs = prototype.touch_coeffs;
    new_info->textures.resize(prototype.textures.size());
    new_info->base = vdef::voxels.size() + 1;

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

    for(std::size_t i = 0; i < state_count; ++i)
        vdef::voxels.push_back(new_info);
    return new_info->base;
}

VoxelInfoBuilder &vdef::construct(const std::string &name, VoxelType type, bool animated, bool blending)
{
    const auto it = vdef::builders.find(name);

    if(it != vdef::builders.cend())
        return it->second;
    return vdef::builders.emplace(name, VoxelInfoBuilder(name, type, animated, blending)).first->second;
}

VoxelInfo *vdef::find(const std::string &name)
{
    const auto it = vdef::names.find(name);
    if(it != vdef::names.cend())
        return vdef::find(it->second);
    return nullptr;
}

VoxelInfo *vdef::find(const VoxelID voxel)
{
    if((voxel != NULL_VOXEL) && (voxel <= vdef::voxels.size()))
        return vdef::voxels[voxel - 1].get();
    return nullptr;
}

void vdef::purge(void)
{
    vdef::builders.clear();
    vdef::names.clear();
    vdef::voxels.clear();
}

std::uint64_t vdef::calc_checksum(void)
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
