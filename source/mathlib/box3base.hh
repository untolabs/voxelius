// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/vec3base.hh"

template<typename T>
class Box3base {
public:
    Vec3base<T> min {};
    Vec3base<T> max {};

public:
    constexpr static Box3base<T> from_bounds(const Vec3base<T> &min, const Vec3base<T> &max);
    constexpr static Box3base<T> from_offset(const Vec3base<T> &offset, const Vec3base<T> &size);

    constexpr static bool contains(const Box3base<T> &box, const Vec3base<T> &point);
    constexpr static bool intersect(const Box3base<T> &abox, const Box3base<T> &bbox);

    constexpr static Box3base<T> combine(const Box3base<T> &abox, const Box3base<T> &bbox);
    constexpr static Box3base<T> multiply(const Box3base<T> &abox, const Box3base<T> &bbox);
};

template<typename T>
constexpr Box3base<T> Box3base<T>::from_bounds(const Vec3base<T> &min, const Vec3base<T> &max)
{
    Box3base<T> result;
    result.min = min;
    result.max = max;
    return std::move(result);
}

template<typename T>
constexpr Box3base<T> Box3base<T>::from_offset(const Vec3base<T> &offset, const Vec3base<T> &size)
{
    Box3base<T> result;
    result.min = offset;
    result.max = offset + size;
    return std::move(result);
}

template<typename T>
constexpr bool Box3base<T>::contains(const Box3base<T> &box, const Vec3base<T> &point)
{
    if((point[0] < box.min[0]) || (point[0] > box.max[0]))
        return false;
    if((point[1] < box.min[1]) || (point[1] > box.max[1]))
        return false;
    if((point[2] < box.min[2]) || (point[2] > box.max[2]))
        return false;
    return true;
}

template<typename T>
constexpr bool Box3base<T>::intersect(const Box3base<T> &abox, const Box3base<T> &bbox)
{
    if((abox.min[0] > bbox.max[0]) || (abox.max[0] < bbox.min[0]))
        return false;
    if((abox.min[1] > bbox.max[1]) || (abox.max[1] < bbox.min[1]))
        return false;
    if((abox.min[2] > bbox.max[2]) || (abox.max[2] < bbox.min[2]))
        return false;
    return true;
}

template<typename T>
constexpr Box3base<T> Box3base<T>::combine(const Box3base<T> &abox, const Box3base<T> &bbox)
{
    Box3base<T> result;
    result.min = abox.min;
    result.max = bbox.max;
    return std::move(result);
}

template<typename T>
constexpr Box3base<T> Box3base<T>::multiply(const Box3base<T> &abox, const Box3base<T> &bbox)
{
    Box3base<T> result;
    result.min = bbox.min;
    result.max = abox.max;
    return std::move(result);
}
