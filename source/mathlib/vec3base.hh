// SPDX-License-Identifier: BSD-2-Clause
#pragma once

template<typename T>
class Vec3base : public std::array<T, 3> {
    static_assert(std::is_arithmetic_v<T>);

public:
    Vec3base(void) = default;
    constexpr Vec3base(T filler);
    constexpr Vec3base(T x, T y, T z);
    constexpr Vec3base(const Vec3base<T> &other);
    using std::array<T, 3>::array;

public:
    constexpr inline T get_x(void) const { return this[0][0]; }
    constexpr inline T get_y(void) const { return this[0][1]; }
    constexpr inline T get_z(void) const { return this[0][2]; }

public:
    constexpr inline void set_x(T value) { this[0][0] = value; }
    constexpr inline void set_y(T value) { this[0][1] = value; }
    constexpr inline void set_z(T value) { this[0][2] = value; }

public:
    constexpr Vec3base<T> operator+(const Vec3base<T> &vector) const;
    constexpr Vec3base<T> operator-(const Vec3base<T> &vector) const;
    constexpr Vec3base<T> operator+(const T scalar) const;
    constexpr Vec3base<T> operator-(const T scalar) const;
    constexpr Vec3base<T> operator*(const T scalar) const;
    constexpr Vec3base<T> operator/(const T scalar) const;
    
public:
    constexpr Vec3base<T> &operator+=(const Vec3base<T> &vector);
    constexpr Vec3base<T> &operator-=(const Vec3base<T> &vector);
    constexpr Vec3base<T> &operator+=(const T scalar);
    constexpr Vec3base<T> &operator-=(const T scalar);
    constexpr Vec3base<T> &operator*=(const T scalar);
    constexpr Vec3base<T> &operator/=(const T scalar);

public:
    constexpr static Vec3base<T> dir_forward(void);
    constexpr static Vec3base<T> dir_back(void);
    constexpr static Vec3base<T> dir_left(void);
    constexpr static Vec3base<T> dir_right(void);
    constexpr static Vec3base<T> dir_down(void);
    constexpr static Vec3base<T> dir_up(void);

public:
    constexpr static Vec3base<T> dir_north(void);
    constexpr static Vec3base<T> dir_south(void);
    constexpr static Vec3base<T> dir_east(void);
    constexpr static Vec3base<T> dir_west(void);

public:
    constexpr static Vec3base<T> zero(void);

    template<typename DT>
    constexpr static Vec3base<T> zero_except(const DT dim, const T value);
};

template<typename T>
constexpr inline Vec3base<T>::Vec3base(T filler)
{
    this[0][0] = filler;
    this[0][1] = filler;
    this[0][2] = filler;
}
template<typename T>
constexpr inline Vec3base<T>::Vec3base(T x, T y, T z)
{
    this[0][0] = x;
    this[0][1] = y;
    this[0][2] = z;
}
template<typename T>
constexpr inline Vec3base<T>::Vec3base(const Vec3base<T> &other)
{
    this[0][0] = other[0];
    this[0][1] = other[1];
    this[0][2] = other[2];
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::operator+(const Vec3base<T> &vector) const
{
    Vec3base<T> result = {};
    result[0] = this[0][0] + vector[0];
    result[1] = this[0][1] + vector[1];
    result[2] = this[0][2] + vector[2];
    return result;
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::operator-(const Vec3base<T> &vector) const
{
    Vec3base<T> result = {};
    result[0] = this[0][0] - vector[0];
    result[1] = this[0][1] - vector[1];
    result[2] = this[0][2] - vector[2];
    return result;
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::operator+(const T scalar) const
{
    Vec3base<T> result = {};
    result[0] = this[0][0] + scalar;
    result[1] = this[0][1] + scalar;
    result[2] = this[0][2] + scalar;
    return result;
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::operator-(const T scalar) const
{
    Vec3base<T> result = {};
    result[0] = this[0][0] - scalar;
    result[1] = this[0][1] - scalar;
    result[2] = this[0][2] - scalar;
    return result;
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::operator*(const T scalar) const
{
    Vec3base<T> result = {};
    result[0] = this[0][0] * scalar;
    result[1] = this[0][1] * scalar;
    result[2] = this[0][2] * scalar;
    return result;
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::operator/(const T scalar) const
{
    Vec3base<T> result = {};
    result[0] = this[0][0] / scalar;
    result[1] = this[0][1] / scalar;
    result[2] = this[0][2] / scalar;
    return result;
}

template<typename T>
constexpr inline Vec3base<T> &Vec3base<T>::operator+=(const Vec3base<T> &vector)
{
    this[0][0] += vector[0];
    this[0][1] += vector[1];
    this[0][2] += vector[2];
    return this[0];
}

template<typename T>
constexpr inline Vec3base<T> &Vec3base<T>::operator-=(const Vec3base<T> &vector)
{
    this[0][0] -= vector[0];
    this[0][1] -= vector[1];
    this[0][2] -= vector[2];
    return this[0];
}

template<typename T>
constexpr inline Vec3base<T> &Vec3base<T>::operator+=(const T scalar)
{
    this[0][0] += scalar;
    this[0][1] += scalar;
    this[0][2] += scalar;
    return this[0];
}

template<typename T>
constexpr inline Vec3base<T> &Vec3base<T>::operator-=(const T scalar)
{
    this[0][0] -= scalar;
    this[0][1] -= scalar;
    this[0][2] -= scalar;
    return this[0];
}

template<typename T>
constexpr inline Vec3base<T> &Vec3base<T>::operator*=(const T scalar)
{
    this[0][0] *= scalar;
    this[0][1] *= scalar;
    this[0][2] *= scalar;
    return this[0];
}

template<typename T>
constexpr inline Vec3base<T> &Vec3base<T>::operator/=(const T scalar)
{
    this[0][0] /= scalar;
    this[0][1] /= scalar;
    this[0][2] /= scalar;
    return this[0];
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_forward(void)
{
    return Vec3base<T>(T(0), T(0), T(1));
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_back(void)
{
    return Vec3base<T>(T(0), T(0), T(-1));
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_left(void)
{
    return Vec3base<T>(T(-1), T(0), T(0));
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_right(void)
{
    return Vec3base<T>(T(1), T(0), T(0));
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_down(void)
{
    return Vec3base<T>(T(0), T(-1), T(0));
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_up(void)
{
    return Vec3base<T>(T(0), T(1), T(0));
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_north(void)
{
    return Vec3base<T>::dir_forward();
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_south(void)
{
    return Vec3base<T>::dir_back();
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_east(void)
{
    return Vec3base<T>::dir_right();
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::dir_west(void)
{
    return Vec3base<T>::dir_left();
}

template<typename T>
constexpr inline Vec3base<T> Vec3base<T>::zero(void)
{
    return Vec3base<T>(T(0), T(0), T(0));
}

template<typename T>
template<typename DT>
constexpr inline Vec3base<T> Vec3base<T>::zero_except(const DT dim, const T value)
{
    static_assert(std::is_integral_v<DT>);
    Vec3base<T> result = Vec3base<T>::zero();
    result[dim] = value;
    return result;
}
