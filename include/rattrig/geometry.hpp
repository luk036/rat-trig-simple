#pragma once

#include <rattrig/rattrig.hpp>
#include <tuple>
#include <utility>

namespace rattrig {

    // ===========================================================================
    // Point2D
    // ===========================================================================

    /// @brief A 2D point with coordinates of type T.
    template <typename T> struct Point2D {
        T x;
        T y;

        /// @brief Construct a new 2D point.
        constexpr Point2D() noexcept : x(detail::zero<T>()), y(detail::zero<T>()) {}
        constexpr Point2D(const T& x_, const T& y_) noexcept : x(x_), y(y_) {}

        /// @brief Create a point from coordinates.
        static constexpr Point2D new_(const T& x_, const T& y_) noexcept { return Point2D(x_, y_); }

        /// @brief Create from a std::pair.
        static constexpr Point2D from_pair(const std::pair<T, T>& p) noexcept {
            return Point2D(p.first, p.second);
        }
    };

    // ===========================================================================
    // Point3D
    // ===========================================================================

    /// @brief A 3D point with coordinates of type T.
    template <typename T> struct Point3D {
        T x;
        T y;
        T z;

        constexpr Point3D() noexcept
            : x(detail::zero<T>()), y(detail::zero<T>()), z(detail::zero<T>()) {}
        constexpr Point3D(const T& x_, const T& y_, const T& z_) noexcept : x(x_), y(y_), z(z_) {}

        static constexpr Point3D new_(const T& x_, const T& y_, const T& z_) noexcept {
            return Point3D(x_, y_, z_);
        }

        static constexpr Point3D from_tuple(const std::tuple<T, T, T>& t) noexcept {
            return Point3D(std::get<0>(t), std::get<1>(t), std::get<2>(t));
        }
    };

    // ===========================================================================
    // Vector2D
    // ===========================================================================

    /// @brief A 2D vector with components of type T.
    template <typename T> struct Vector2D {
        T x;
        T y;

        constexpr Vector2D() noexcept : x(detail::zero<T>()), y(detail::zero<T>()) {}
        constexpr Vector2D(const T& x_, const T& y_) noexcept : x(x_), y(y_) {}

        static constexpr Vector2D new_(const T& x_, const T& y_) noexcept {
            return Vector2D(x_, y_);
        }

        static constexpr Vector2D from_pair(const std::pair<T, T>& v) noexcept {
            return Vector2D(v.first, v.second);
        }

        static constexpr Vector2D from_point(const Point2D<T>& p) noexcept {
            return Vector2D(p.x, p.y);
        }

        /// @brief Vector addition.
        friend constexpr Vector2D operator+(const Vector2D& a, const Vector2D& b) noexcept {
            return Vector2D(a.x + b.x, a.y + b.y);
        }

        /// @brief Vector subtraction.
        friend constexpr Vector2D operator-(const Vector2D& a, const Vector2D& b) noexcept {
            return Vector2D(a.x - b.x, a.y - b.y);
        }
    };

    // ===========================================================================
    // Vector3D
    // ===========================================================================

    /// @brief A 3D vector with components of type T.
    template <typename T> struct Vector3D {
        T x;
        T y;
        T z;

        constexpr Vector3D() noexcept
            : x(detail::zero<T>()), y(detail::zero<T>()), z(detail::zero<T>()) {}
        constexpr Vector3D(const T& x_, const T& y_, const T& z_) noexcept : x(x_), y(y_), z(z_) {}

        static constexpr Vector3D new_(const T& x_, const T& y_, const T& z_) noexcept {
            return Vector3D(x_, y_, z_);
        }

        static constexpr Vector3D from_tuple(const std::tuple<T, T, T>& v) noexcept {
            return Vector3D(std::get<0>(v), std::get<1>(v), std::get<2>(v));
        }

        static constexpr Vector3D from_point(const Point3D<T>& p) noexcept {
            return Vector3D(p.x, p.y, p.z);
        }

        friend constexpr Vector3D operator+(const Vector3D& a, const Vector3D& b) noexcept {
            return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        friend constexpr Vector3D operator-(const Vector3D& a, const Vector3D& b) noexcept {
            return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
        }
    };

    // ===========================================================================
    // Line2D
    // ===========================================================================

    /// @brief A 2D line represented as ax + by + c = 0.
    template <typename T> struct Line2D {
        T a;
        T b;
        T c;

        constexpr Line2D() noexcept
            : a(detail::zero<T>()), b(detail::zero<T>()), c(detail::zero<T>()) {}
        constexpr Line2D(const T& a_, const T& b_, const T& c_) noexcept : a(a_), b(b_), c(c_) {}

        static constexpr Line2D new_(const T& a_, const T& b_, const T& c_) noexcept {
            return Line2D(a_, b_, c_);
        }

        static constexpr Line2D from_tuple(const std::tuple<T, T, T>& l) noexcept {
            return Line2D(std::get<0>(l), std::get<1>(l), std::get<2>(l));
        }
    };

    // ===========================================================================
    // Triangle2D
    // ===========================================================================

    /// @brief A 2D triangle defined by three points.
    template <typename T> struct Triangle2D {
        Point2D<T> p1;
        Point2D<T> p2;
        Point2D<T> p3;

        constexpr Triangle2D() noexcept = default;
        constexpr Triangle2D(const Point2D<T>& p1_, const Point2D<T>& p2_,
                             const Point2D<T>& p3_) noexcept
            : p1(p1_), p2(p2_), p3(p3_) {}

        /// @brief Calculate the three quadrances of the triangle sides.
        /// @return (q1, q2, q3) where q1 is opposite p1.
        constexpr std::tuple<T, T, T> quadrances() const noexcept {
            T q1 = (p2.x - p3.x) * (p2.x - p3.x) + (p2.y - p3.y) * (p2.y - p3.y);
            T q2 = (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y);
            T q3 = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
            return std::make_tuple(q1, q2, q3);
        }

        /// @brief Compute the quadrea (4 x area^2) using Archimedes' formula.
        constexpr T area() const noexcept {
            auto [q1, q2, q3] = quadrances();
            T temp = q1 + q2 - q3;
            return detail::four<T>() * q1 * q2 - temp * temp;
        }

        /// @brief Compute the twist (twice signed area).
        constexpr T twist() const noexcept {
            return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        }

        /// @brief Check if the triangle is degenerate (all points collinear).
        constexpr bool is_degenerate() const noexcept { return twist() == detail::zero<T>(); }
    };

    // ===========================================================================
    // Triangle3D
    // ===========================================================================

    /// @brief A 3D triangle defined by three points.
    template <typename T> struct Triangle3D {
        Point3D<T> p1;
        Point3D<T> p2;
        Point3D<T> p3;

        constexpr Triangle3D() noexcept = default;
        constexpr Triangle3D(const Point3D<T>& p1_, const Point3D<T>& p2_,
                             const Point3D<T>& p3_) noexcept
            : p1(p1_), p2(p2_), p3(p3_) {}

        /// @brief Calculate the three quadrances of the triangle sides.
        constexpr std::tuple<T, T, T> quadrances() const noexcept {
            T q1 = (p2.x - p3.x) * (p2.x - p3.x) + (p2.y - p3.y) * (p2.y - p3.y)
                   + (p2.z - p3.z) * (p2.z - p3.z);
            T q2 = (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y)
                   + (p1.z - p3.z) * (p1.z - p3.z);
            T q3 = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)
                   + (p1.z - p2.z) * (p1.z - p2.z);
            return std::make_tuple(q1, q2, q3);
        }
    };

}  // namespace rattrig
