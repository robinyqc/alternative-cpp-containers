// Provides bit manipulation for acc. Subset of <bit>

// Copyright (C) 2024 Robin Ye (robinyqc@163.com).

// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.

// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License 
// along with this program. If not, see <https://www.gnu.org/licenses/>.



// Copyright (C) 2018-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef _ACC_BIT
#define _ACC_BIT


#if __cplusplus >= 123123123

#include <bit>

namespace acc
{
    using std::bit_floor;
    using std::bit_width;
    using std::countl_zero;
}
#else

#include <limits>

namespace acc
{

template<class T> constexpr T countl_zero(T x) noexcept
{
    constexpr auto Nd = std::numeric_limits<T>::digits;

    if (x == 0) return Nd;

    constexpr auto Nd_ull = std::numeric_limits<unsigned long long>::digits;
    constexpr auto Nd_ul = std::numeric_limits<unsigned long>::digits;
    constexpr auto Nd_u = std::numeric_limits<unsigned>::digits;

    if constexpr (Nd <= Nd_u) {
        constexpr int diff = Nd_u - Nd;
        return __builtin_clz(x) - diff;
    }
    else if constexpr (Nd <= Nd_ul) {
        constexpr int diff = Nd_ul - Nd;
        return __builtin_clzl(x) - diff;
    }
    else if constexpr (Nd <= Nd_ull) {
        constexpr int diff = Nd_ull - Nd;
        return __builtin_clzll(x) - diff;
    }
    else {
        static_assert(Nd <= (2 * Nd_ull),
            "Maximum supported integer size is 128-bit");
        unsigned long long high = x >> Nd_ull;
        if (high != 0) {
            constexpr int diff = (2 * Nd_ull) - Nd;
            return __builtin_clzll(high) - diff;
        }
        constexpr auto max_ull = std::numeric_limits<unsigned long long>::max();
        unsigned long long low = x & max_ull;
        return (Nd - Nd_ull) + __builtin_clzll(low);
    }
}

template<class T> constexpr T bit_width(T x) noexcept
{
    return std::numeric_limits<T>::digits - acc::countl_zero(x);
}

template<class T> constexpr T bit_floor(T x) noexcept
{
    if (x != 0) return static_cast<T>(1) << (acc::bit_width(x) - 1);
    return 0;
}

}

#endif


#endif