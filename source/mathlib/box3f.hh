// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/box3base.hh"

class Box3f final : public Box3base<float> {
public:
    template<typename T>
    constexpr Box3f(const Box3base<T> &other);
    using Box3base<float>::Box3base;
};

template<typename T>
constexpr inline Box3f::Box3f(const Box3base<T> &other)
{
    this->min = Vec3base<float>(other.min);
    this->max = Vec3base<float>(other.max);
}
