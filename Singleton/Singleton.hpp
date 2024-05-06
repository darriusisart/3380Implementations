#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

template <typename T, typename U>
    requires(std::semiregular<T> || std::regular<T> || std::totally_ordered<T> ||
             std::semiregular<U> || std::regular<U> || std::totally_ordered<U>)
struct pair_singleton final
{
    using first_type = T;
    using second_type = U;

    std::pair<T, U> value;

    // Write conversions from std::pair<T, U> to pair_singleton<T, U> and vice versa.
    explicit pair_singleton(std::pair<T, U> const& p)
      : value(p)
    {
    }

    explicit operator std::pair<T, U>() const
    {
        return value;
    }

    // Semiregular:
    pair_singleton() = default;
    ~pair_singleton() = default;
    pair_singleton(pair_singleton const& x) = default;
    pair_singleton& operator=(pair_singleton const& x) = default;

    // Regular
    friend bool operator==(pair_singleton const& x, pair_singleton const& y)
    {
        return x.value == y.value;
    }
    friend bool operator!=(pair_singleton const& x, pair_singleton const& y)
    {
        return !(x == y);
    }

    // TotallyOrdered
    friend bool operator<(pair_singleton const& x, pair_singleton const& y)
    {
        return x.value < y.value;
    }
    friend bool operator>(pair_singleton const& x, pair_singleton const& y)
    {
        return y < x;
    }
    friend bool operator<=(pair_singleton const& x, pair_singleton const& y)
    {
        return !(y < x);
    }
    friend bool operator>=(pair_singleton const& x, pair_singleton const& y)
    {
        return !(x < y);
    }
};
