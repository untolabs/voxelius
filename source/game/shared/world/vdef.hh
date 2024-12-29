#pragma once
#include "mathlib/vec3f.hh"
#include "shared/world/voxel_id.hh"

enum class VoxelFace : unsigned int {
    CubeNorth   = 0x0000,
    CubeSouth   = 0x0001,
    CubeEast    = 0x0002,
    CubeWest    = 0x0003,
    CubeTop     = 0x0004,
    CubeBottom  = 0x0005,
    CubeCount   = 0x0006,

    SlabNorth   = CubeNorth,
    SlabSouth   = CubeSouth,
    SlabEast    = CubeEast,
    SlabWest    = CubeWest,
    SlabTop     = CubeTop,
    SlabBottom  = CubeBottom,
    SlabCount   = CubeCount,

    StairNorth  = CubeNorth,
    StairSouth  = CubeSouth,
    StairEast   = CubeEast,
    StairWest   = CubeWest,
    StairTop    = CubeTop,
    StairBottom = CubeBottom,
    StairCount  = CubeCount,

    CrossNESW   = 0x0000,
    CrossNWSE   = 0x0001,
    CrossCount  = 0x0002,
};

enum class VoxelType : unsigned int {
    Cube        = 0x0000,
    Slab        = 0x0001, // TODO
    Stairs      = 0x0002, // TODO
    Cross       = 0x0003, // TODO
    VModel      = 0x0004, // TODO
    DModel      = 0x0005, // TODO
};

using VoxelFacing = unsigned short;
constexpr static VoxelFacing FACING_NORTH   = 0x0000;
constexpr static VoxelFacing FACING_SOUTH   = 0x0001;
constexpr static VoxelFacing FACING_EAST    = 0x0002;
constexpr static VoxelFacing FACING_WEST    = 0x0003;
constexpr static VoxelFacing FACING_UP      = 0x0004;
constexpr static VoxelFacing FACING_DOWN    = 0x0005;
constexpr static VoxelFacing FACING_NESW    = 0x0006;
constexpr static VoxelFacing FACING_NWSE    = 0x0007;

using VoxelVis = unsigned short;
constexpr static VoxelVis VIS_NORTH = 1 << FACING_NORTH;
constexpr static VoxelVis VIS_SOUTH = 1 << FACING_SOUTH;
constexpr static VoxelVis VIS_EAST  = 1 << FACING_EAST;
constexpr static VoxelVis VIS_WEST  = 1 << FACING_WEST;
constexpr static VoxelVis VIS_UP    = 1 << FACING_UP;
constexpr static VoxelVis VIS_DOWN  = 1 << FACING_DOWN;

using VoxelTouch = unsigned short;
constexpr static VoxelTouch TOUCH_SOLID     = 0x0000;
constexpr static VoxelTouch TOUCH_BOUNCE    = 0x0001;
constexpr static VoxelTouch TOUCH_SINK      = 0x0002;
constexpr static VoxelTouch TOUCH_NOTHING   = 0xFFFF;

struct VoxelTexture final {
    std::vector<std::string> paths {};
    std::size_t cached_offset {};
    std::size_t cached_plane {};
};

struct VoxelInfo final {
    std::vector<VoxelTexture> textures {};
    VoxelTouch touch_type {};
    Vec3f touch_coeffs {};
    std::string name {};
    VoxelType type {};
    bool animated {};
    bool blending {};
    VoxelID base {};
};

class VoxelInfoBuilder final {
public:
    explicit VoxelInfoBuilder(const std::string &name, VoxelType type, bool animated, bool blending);
    virtual ~VoxelInfoBuilder(void) = default;

public:
    VoxelInfoBuilder &add_texture_default(const std::string &texture);
    VoxelInfoBuilder &add_texture(VoxelFace face, const std::string &texture);
    VoxelInfoBuilder &set_touch(VoxelTouch type, const Vec3f &coeffs);

public:
    VoxelID build(void) const;

private:
    VoxelTexture default_texture;
    VoxelInfo prototype;
};

namespace vdef
{
extern std::unordered_map<std::string, VoxelInfoBuilder> builders;
extern std::unordered_map<std::string, VoxelID> names;
extern std::vector<std::shared_ptr<VoxelInfo>> voxels;
} // namespace vdef

namespace vdef
{
VoxelInfoBuilder &construct(const std::string &name, VoxelType type, bool animated, bool blending);
VoxelInfo *find(const std::string &name);
VoxelInfo *find(const VoxelID voxel);
} // namespace vdef

namespace vdef
{
void purge(void);
} // namespace vdef

namespace vdef
{
std::uint64_t calc_checksum(void);
} // namespace vdef
