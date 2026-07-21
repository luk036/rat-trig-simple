#pragma once

#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rattrig {

// ---------------------------------------------------------------------------
// Detail helpers — generic constant generation for any numeric type
// ---------------------------------------------------------------------------

namespace detail {

// Detect whether T has static zero() / one() members (e.g. Fraction<T>)
template <typename T, typename = void>
struct has_static_zero : std::false_type {};
template <typename T>
struct has_static_zero<T, std::void_t<decltype(T::zero())>> : std::true_type {};

template <typename T, typename = void>
struct has_static_one : std::false_type {};
template <typename T>
struct has_static_one<T, std::void_t<decltype(T::one())>> : std::true_type {};

/// @brief Return the zero constant for type T.
template <typename T>
constexpr T zero() noexcept {
    if constexpr (has_static_zero<T>::value) {
        return T::zero();
    } else {
        return T(0);
    }
}

/// @brief Return the one constant for type T.
template <typename T>
constexpr T one() noexcept {
    if constexpr (has_static_one<T>::value) {
        return T::one();
    } else {
        return T(1);
    }
}

/// @brief Return the constant 4 for type T.
template <typename T>
constexpr T four() noexcept {
    return one<T>() + one<T>() + one<T>() + one<T>();
}

/// @brief Return a pair of zeros.
template <typename T>
constexpr std::pair<T, T> zero_pair() noexcept {
    return std::pair<T, T>(zero<T>(), zero<T>());
}

/// @brief Return a tuple of three zeros.
template <typename T>
constexpr std::tuple<T, T, T> zero_triple() noexcept {
    return std::tuple<T, T, T>(zero<T>(), zero<T>(), zero<T>());
}

}  // namespace detail

// ===========================================================================
// archimedes: quadrea from three side quadrances
// ===========================================================================

/// @brief Compute the quadrea (4 x area^2) of a triangle using Archimedes' formula.
///
/// @f[
///     \text{quadrea}(q_1, q_2, q_3) = 4 q_1 q_2 - (q_1 + q_2 - q_3)^2
/// @f]
///
/// @tparam T A numeric type
/// @param[in] q_1 Quadrance of side 1
/// @param[in] q_2 Quadrance of side 2
/// @param[in] q_3 Quadrance of side 3
/// @return The quadrea
template <typename T>
constexpr T archimedes(const T& q_1, const T& q_2, const T& q_3) noexcept {
    T temp = q_1 + q_2 - q_3;
    return detail::four<T>() * q_1 * q_2 - temp * temp;
}

// ===========================================================================
// quadrance: squared distance between 2D points
// ===========================================================================

/// @brief Compute the quadrance (squared distance) between two 2D points.
///
/// @f[
///     Q(p_1, p_2) = (x_2 - x_1)^2 + (y_2 - y_1)^2
/// @f]
///
/// @tparam T A numeric type
/// @param[in] p_1 First point as (x, y)
/// @param[in] p_2 Second point as (x, y)
/// @return The quadrance
template <typename T>
constexpr T quadrance(const std::pair<T, T>& p_1,
                      const std::pair<T, T>& p_2) noexcept {
    T dx = p_1.first - p_2.first;
    T dy = p_1.second - p_2.second;
    return dx * dx + dy * dy;
}

// ===========================================================================
// spread: squared sine between 2D vectors
// ===========================================================================

/// @brief Compute the spread (square of sine) between two 2D vectors.
///
/// @f[
///     s(v_1, v_2) = 1 - \frac{(v_1 \cdot v_2)^2}{|v_1|^2 |v_2|^2}
/// @f]
///
/// @tparam T A numeric type (must support division)
/// @param[in] v_1 First vector as (x, y)
/// @param[in] v_2 Second vector as (x, y)
/// @return The spread
template <typename T>
constexpr T spread(const std::pair<T, T>& v_1,
                   const std::pair<T, T>& v_2) noexcept {
    T dot = v_1.first * v_2.first + v_1.second * v_2.second;
    T q_1 = quadrance(v_1, detail::zero_pair<T>());
    T q_2 = quadrance(v_2, detail::zero_pair<T>());
    return detail::one<T>() - dot * dot / (q_1 * q_2);
}

// ===========================================================================
// cross: 2D cross product
// ===========================================================================

/// @brief Compute the 2D cross product (signed area) of two vectors.
///
/// @f[
///     \text{cross}(v_1, v_2) = x_1 y_2 - y_1 x_2
/// @f]
///
/// @tparam T A numeric type
/// @param[in] v_1 First vector as (x, y)
/// @param[in] v_2 Second vector as (x, y)
/// @return The cross product
template <typename T>
constexpr T cross(const std::pair<T, T>& v_1,
                  const std::pair<T, T>& v_2) noexcept {
    return v_1.first * v_2.second - v_1.second * v_2.first;
}

// ===========================================================================
// quadrance_from_line
// ===========================================================================

/// @brief Compute quadrance from a point to a line.
///
/// @f[
///     Q(p, l) = \frac{(a x + b y + c)^2}{a^2 + b^2}
/// @f]
///
/// @tparam T A numeric type (must support division)
/// @param[in] p Point as (x, y)
/// @param[in] l Line coefficients as (a, b, c) where ax + by + c = 0
/// @return The quadrance
template <typename T>
constexpr T quadrance_from_line(const std::pair<T, T>& p,
                                const std::tuple<T, T, T>& l) noexcept {
    T temp = std::get<0>(l) * p.first + std::get<1>(l) * p.second + std::get<2>(l);
    return temp * temp / quadrance(
        std::pair<T, T>(std::get<0>(l), std::get<1>(l)),
        detail::zero_pair<T>());
}

// ===========================================================================
// spread_from_line
// ===========================================================================

/// @brief Compute the spread between two lines.
///
/// @f[
///     s(l_1, l_2) = \frac{(a_1 b_2 - a_2 b_1)^2}{(a_1^2 + b_1^2)(a_2^2 + b_2^2)}
/// @f]
///
/// @tparam T A numeric type (must support division)
/// @param[in] l_1 First line as (a, b, c)
/// @param[in] l_2 Second line as (a, b, c)
/// @return The spread
template <typename T>
constexpr T spread_from_line(const std::tuple<T, T, T>& l_1,
                             const std::tuple<T, T, T>& l_2) noexcept {
    T temp = cross(std::pair<T, T>(std::get<0>(l_1), std::get<1>(l_1)),
                   std::pair<T, T>(std::get<0>(l_2), std::get<1>(l_2)));
    return temp * temp / (
        quadrance(std::pair<T, T>(std::get<0>(l_1), std::get<1>(l_1)),
                  detail::zero_pair<T>()) *
        quadrance(std::pair<T, T>(std::get<0>(l_2), std::get<1>(l_2)),
                  detail::zero_pair<T>()));
}

// ===========================================================================
// cross_from_line
// ===========================================================================

/// @brief Compute the cross product of two lines.
///
/// @f[
///     \text{cross}(l_1, l_2) = a_1 b_2 - a_2 b_1
/// @f]
///
/// @tparam T A numeric type
/// @param[in] l_1 First line as (a, b, c)
/// @param[in] l_2 Second line as (a, b, c)
/// @return The cross product
template <typename T>
constexpr T cross_from_line(const std::tuple<T, T, T>& l_1,
                            const std::tuple<T, T, T>& l_2) noexcept {
    return cross(std::pair<T, T>(std::get<0>(l_1), std::get<1>(l_1)),
                 std::pair<T, T>(std::get<0>(l_2), std::get<1>(l_2)));
}

// ===========================================================================
// quadrance_from_three_points
// ===========================================================================

/// @brief Compute the three quadrances of a triangle from its vertices.
///
/// Returns (q1, q2, q3) where q1 is opposite p1 (between p2 and p3).
///
/// @tparam T A numeric type
/// @return Tuple of three quadrances
template <typename T>
constexpr std::tuple<T, T, T> quadrance_from_three_points(
    const std::pair<T, T>& p_1,
    const std::pair<T, T>& p_2,
    const std::pair<T, T>& p_3) noexcept {
    return std::make_tuple(
        quadrance(p_2, p_3),
        quadrance(p_1, p_3),
        quadrance(p_1, p_2));
}

// ===========================================================================
// spread_from_three_points
// ===========================================================================

/// @brief Compute the three spreads of a triangle from its vertices.
///
/// Returns (s1, s2, s3) where s1 is the spread at vertex p1.
///
/// @tparam T A numeric type (must support division)
/// @return Tuple of three spreads
template <typename T>
constexpr std::tuple<T, T, T> spread_from_three_points(
    const std::pair<T, T>& p_1,
    const std::pair<T, T>& p_2,
    const std::pair<T, T>& p_3) noexcept {
    T q_1 = quadrance(p_2, p_3);
    T q_2 = quadrance(p_1, p_3);
    T q_3 = quadrance(p_1, p_2);
    T f = detail::four<T>();
    T s_1 = detail::one<T>() - (q_2 + q_3 - q_1) * (q_2 + q_3 - q_1) / (f * q_2 * q_3);
    T s_2 = detail::one<T>() - (q_1 + q_3 - q_2) * (q_1 + q_3 - q_2) / (f * q_1 * q_3);
    T s_3 = detail::one<T>() - (q_1 + q_2 - q_3) * (q_1 + q_2 - q_3) / (f * q_1 * q_2);
    return std::make_tuple(s_1, s_2, s_3);
}

// ===========================================================================
// cross_from_three_points
// ===========================================================================

/// @brief Compute the cross product (twice signed area) from three points.
///
/// @f[
///     \text{cross}(p_1, p_2, p_3) = (x_2 - x_1)(y_3 - y_1) - (y_2 - y_1)(x_3 - x_1)
/// @f]
///
/// @tparam T A numeric type
/// @return The cross product
template <typename T>
constexpr T cross_from_three_points(const std::pair<T, T>& p_1,
                                    const std::pair<T, T>& p_2,
                                    const std::pair<T, T>& p_3) noexcept {
    return cross(
        std::pair<T, T>(p_2.first - p_1.first, p_2.second - p_1.second),
        std::pair<T, T>(p_3.first - p_1.first, p_3.second - p_1.second));
}

// ===========================================================================
// 3D operations
// ===========================================================================

/// @brief Compute the quadrance between two 3D points.
template <typename T>
constexpr T quadrance3d(const std::tuple<T, T, T>& p_1,
                        const std::tuple<T, T, T>& p_2) noexcept {
    T dx = std::get<0>(p_1) - std::get<0>(p_2);
    T dy = std::get<1>(p_1) - std::get<1>(p_2);
    T dz = std::get<2>(p_1) - std::get<2>(p_2);
    return dx * dx + dy * dy + dz * dz;
}

/// @brief Compute the 3D cross product.
template <typename T>
constexpr std::tuple<T, T, T> cross3d(const std::tuple<T, T, T>& v_1,
                                      const std::tuple<T, T, T>& v_2) noexcept {
    return std::make_tuple(
        std::get<1>(v_1) * std::get<2>(v_2) - std::get<2>(v_1) * std::get<1>(v_2),
        std::get<2>(v_1) * std::get<0>(v_2) - std::get<0>(v_1) * std::get<2>(v_2),
        std::get<0>(v_1) * std::get<1>(v_2) - std::get<1>(v_1) * std::get<0>(v_2));
}

/// @brief Compute the spread between two 3D vectors.
template <typename T>
constexpr T spread3d(const std::tuple<T, T, T>& v_1,
                     const std::tuple<T, T, T>& v_2) noexcept {
    T dot = std::get<0>(v_1) * std::get<0>(v_2)
          + std::get<1>(v_1) * std::get<1>(v_2)
          + std::get<2>(v_1) * std::get<2>(v_2);
    T q_1 = quadrance3d(v_1, detail::zero_triple<T>());
    T q_2 = quadrance3d(v_2, detail::zero_triple<T>());
    return detail::one<T>() - dot * dot / (q_1 * q_2);
}

// ===========================================================================
// twist (twice signed area)
// ===========================================================================

/// @brief Compute the twist (twice signed area) of a triangle.
///
/// Equivalent to cross_from_three_points.
template <typename T>
constexpr T twist(const std::pair<T, T>& p_1,
                  const std::pair<T, T>& p_2,
                  const std::pair<T, T>& p_3) noexcept {
    return cross_from_three_points(p_1, p_2, p_3);
}

// ===========================================================================
// turn (oriented angle measure)
// ===========================================================================

/// @brief Compute the turn (oriented angle measure) between three points.
///
/// Returns (spread, sign) where sign indicates orientation.
template <typename T>
constexpr std::pair<T, bool> turn(const std::pair<T, T>& p_1,
                                  const std::pair<T, T>& p_2,
                                  const std::pair<T, T>& p_3) noexcept {
    auto v1 = std::pair<T, T>(p_2.first - p_1.first, p_2.second - p_1.second);
    auto v2 = std::pair<T, T>(p_3.first - p_2.first, p_3.second - p_2.second);
    T s = spread(v1, v2);
    bool sign = cross(v1, v2) >= detail::zero<T>();
    return std::make_pair(s, sign);
}

// ===========================================================================
// dilatation
// ===========================================================================

/// @brief Compute the dilatation (ratio of squared lengths) between two vectors.
template <typename T>
constexpr T dilatation(const std::pair<T, T>& v_1,
                       const std::pair<T, T>& v_2) noexcept {
    T q_1 = quadrance(v_1, detail::zero_pair<T>());
    T q_2 = quadrance(v_2, detail::zero_pair<T>());
    return q_2 / q_1;
}

// ===========================================================================
// sine_law_product
// ===========================================================================

/// @brief Sine law equivalent: q * s.
template <typename T>
constexpr T sine_law_product(const T& q, const T& s) noexcept {
    return q * s;
}

// ===========================================================================
// cosine_law
// ===========================================================================

/// @brief Cosine law equivalent.
///
/// @f[
///     s_1 = 1 - \frac{(q_2 + q_3 - q_1)^2}{4 q_2 q_3}
/// @f]
template <typename T>
constexpr T cosine_law(const T& q_1, const T& q_2, const T& q_3) noexcept {
    T f = detail::four<T>();
    return detail::one<T>() - (q_2 + q_3 - q_1) * (q_2 + q_3 - q_1) / (f * q_2 * q_3);
}

// ===========================================================================
// Concrete type functions (i32, i64)
// ===========================================================================

/// @brief Quadrance for i32 coordinates.
constexpr int quadrance_i32(std::pair<int, int> p_1,
                            std::pair<int, int> p_2) noexcept {
    int dx = p_1.first - p_2.first;
    int dy = p_1.second - p_2.second;
    return dx * dx + dy * dy;
}

/// @brief Archimedes for i32 quadrances.
constexpr int archimedes_i32(int q_1, int q_2, int q_3) noexcept {
    int temp = q_1 + q_2 - q_3;
    return 4 * q_1 * q_2 - temp * temp;
}

/// @brief Spread for i32 vectors (returns numerator).
constexpr int spread_i32(std::pair<int, int> v_1,
                         std::pair<int, int> v_2) noexcept {
    int dot = v_1.first * v_2.first + v_1.second * v_2.second;
    int q_1 = quadrance_i32(v_1, {0, 0});
    int q_2 = quadrance_i32(v_2, {0, 0});
    return q_1 * q_2 - dot * dot / (q_1 * q_2);
}

/// @brief Cross product for i32 vectors.
constexpr int cross_i32(std::pair<int, int> v_1,
                        std::pair<int, int> v_2) noexcept {
    return v_1.first * v_2.second - v_1.second * v_2.first;
}

/// @brief Quadrance for i64 coordinates.
constexpr long long quadrance_i64(std::pair<long long, long long> p_1,
                                  std::pair<long long, long long> p_2) noexcept {
    long long dx = p_1.first - p_2.first;
    long long dy = p_1.second - p_2.second;
    return dx * dx + dy * dy;
}

/// @brief Archimedes for i64 quadrances.
constexpr long long archimedes_i64(long long q_1, long long q_2,
                                   long long q_3) noexcept {
    long long temp = q_1 + q_2 - q_3;
    return 4 * q_1 * q_2 - temp * temp;
}

/// @brief Spread for i64 vectors (returns numerator).
constexpr long long spread_i64(std::pair<long long, long long> v_1,
                               std::pair<long long, long long> v_2) noexcept {
    long long dot = v_1.first * v_2.first + v_1.second * v_2.second;
    long long q_1 = quadrance_i64(v_1, {0, 0});
    long long q_2 = quadrance_i64(v_2, {0, 0});
    return q_1 * q_2 - dot * dot / (q_1 * q_2);
}

/// @brief Cross product for i64 vectors.
constexpr long long cross_i64(std::pair<long long, long long> v_1,
                              std::pair<long long, long long> v_2) noexcept {
    return v_1.first * v_2.second - v_1.second * v_2.first;
}

}  // namespace rattrig
