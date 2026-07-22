#pragma once

#include <rattrig/rattrig.hpp>
#include <tuple>
#include <utility>

namespace rattrig {

    // ===========================================================================
    // Collinearity
    // ===========================================================================

    /// @brief Check if three 2D points are collinear.
    ///
    /// @f[
    ///     (p_2 - p_1) \times (p_3 - p_1) = 0
    /// @f]
    ///
    /// @return true if the cross product of vectors (p2-p1) and (p3-p1) is zero.
    template <typename T> constexpr bool are_collinear(const std::pair<T, T>& p_1,
                                                       const std::pair<T, T>& p_2,
                                                       const std::pair<T, T>& p_3) noexcept {
        return cross(std::pair<T, T>(p_2.first - p_1.first, p_2.second - p_1.second),
                     std::pair<T, T>(p_3.first - p_1.first, p_3.second - p_1.second))
               == detail::zero<T>();
    }

    /// @brief Check if three points form a valid (non-degenerate) triangle.
    template <typename T> constexpr bool is_valid_triangle(const std::pair<T, T>& p_1,
                                                           const std::pair<T, T>& p_2,
                                                           const std::pair<T, T>& p_3) noexcept {
        return !are_collinear(p_1, p_2, p_3);
    }

    // ===========================================================================
    // Quadrance / spread validation
    // ===========================================================================

    /// @brief Check if a quadrance value is valid (non-negative).
    template <typename T> constexpr bool is_valid_quadrance(const T& q) noexcept {
        return q >= detail::zero<T>();
    }

    /// @brief Check if a spread value is valid (in [0, 1]).
    template <typename T> constexpr bool is_valid_spread(const T& s) noexcept {
        return s >= detail::zero<T>() && s <= detail::one<T>();
    }

    /// @brief Check the triangle inequality for three quadrances.
    template <typename T> constexpr bool satisfies_triangle_inequality(const T& q_1, const T& q_2,
                                                                       const T& q_3) noexcept {
        return q_1 >= detail::zero<T>() && q_2 >= detail::zero<T>() && q_3 >= detail::zero<T>();
    }

    // ===========================================================================
    // Triangle classification by spreads
    // ===========================================================================

    /// @brief Check if a triangle is acute-angled (all spreads < 1).
    template <typename T>
    constexpr bool is_acute_triangle(const T& s_1, const T& s_2, const T& s_3) noexcept {
        return s_1 < detail::one<T>() && s_2 < detail::one<T>() && s_3 < detail::one<T>();
    }

    /// @brief Check if a triangle is right-angled (one spread = 1).
    template <typename T>
    constexpr bool is_right_triangle(const T& s_1, const T& s_2, const T& s_3) noexcept {
        return s_1 == detail::one<T>() || s_2 == detail::one<T>() || s_3 == detail::one<T>();
    }

    /// @brief Check if a triangle is obtuse-angled (one spread > 0.5).
    template <typename T>
    constexpr bool is_obtuse_triangle(const T& s_1, const T& s_2, const T& s_3) noexcept {
        T half = detail::one<T>() / (detail::one<T>() + detail::one<T>());
        return s_1 > half || s_2 > half || s_3 > half;
    }

    // ===========================================================================
    // Line relationships
    // ===========================================================================

    /// @brief Check if two lines are parallel.
    ///
    /// @f[
    ///     a_1 b_2 - a_2 b_1 = 0
    /// @f]
    template <typename T>
    constexpr bool are_lines_parallel(const std::tuple<T, T, T>& l_1,
                                      const std::tuple<T, T, T>& l_2) noexcept {
        return std::get<0>(l_1) * std::get<1>(l_2) - std::get<1>(l_1) * std::get<0>(l_2)
               == detail::zero<T>();
    }

    /// @brief Check if two lines are perpendicular.
    ///
    /// @f[
    ///     a_1 a_2 + b_1 b_2 = 0
    /// @f]
    template <typename T>
    constexpr bool are_lines_perpendicular(const std::tuple<T, T, T>& l_1,
                                           const std::tuple<T, T, T>& l_2) noexcept {
        return std::get<0>(l_1) * std::get<0>(l_2) + std::get<1>(l_1) * std::get<1>(l_2)
               == detail::zero<T>();
    }

    // ===========================================================================
    // Point-on-line / point-in-triangle
    // ===========================================================================

    /// @brief Check if a point lies on a line.
    ///
    /// @f[
    ///     a x + b y + c = 0
    /// @f]
    template <typename T> constexpr bool point_on_line(const std::pair<T, T>& point,
                                                       const std::tuple<T, T, T>& line) noexcept {
        T result = std::get<0>(line) * point.first + std::get<1>(line) * point.second
                   + std::get<2>(line);
        return result == detail::zero<T>();
    }

    /// @brief Check if a point lies inside a triangle using barycentric coordinates.
    template <typename T> constexpr bool point_in_triangle(const std::pair<T, T>& point,
                                                           const std::pair<T, T>& p_1,
                                                           const std::pair<T, T>& p_2,
                                                           const std::pair<T, T>& p_3) noexcept {
        T x = point.first;
        T y = point.second;
        T x1 = p_1.first, y1 = p_1.second;
        T x2 = p_2.first, y2 = p_2.second;
        T x3 = p_3.first, y3 = p_3.second;

        T denom = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
        if (denom == detail::zero<T>()) return false;

        T a = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / denom;
        T b = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / denom;
        T c = detail::one<T>() - a - b;

        return a >= detail::zero<T>() && b >= detail::zero<T>() && c >= detail::zero<T>();
    }

}  // namespace rattrig
