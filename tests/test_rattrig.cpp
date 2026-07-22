#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdint>
#include <fraction/fraction.hpp>
#include <rattrig/geometry.hpp>
#include <rattrig/rattrig.hpp>
#include <rattrig/validation.hpp>

using namespace rattrig;
using fraction::Fraction;

// ===========================================================================
// archimedes
// ===========================================================================

TEST_CASE("archimedes") {
    CHECK(archimedes(1, 2, 3) == 8);
    using F = Fraction<int>;
    auto a1 = F::from(1, 1);
    auto a2 = F::from(2, 1);
    auto a3 = F::from(3, 1);
    auto r = F::from(8, 1);
    CHECK(rattrig::archimedes(a1, a2, a3) == r);
}

TEST_CASE("archimedes i64") {
    int64_t q1 = 1, q2 = 2, q3 = 3;
    CHECK(archimedes(q1, q2, q3) == 8);
}

// ===========================================================================
// quadrance
// ===========================================================================

TEST_CASE("quadrance") {
    auto p1 = std::make_pair(1, 1);
    auto p2 = std::make_pair(4, 5);
    CHECK(quadrance(p1, p2) == 25);
}

TEST_CASE("quadrance3d") {
    auto p1 = std::make_tuple(0, 0, 0);
    auto p2 = std::make_tuple(1, 2, 2);
    CHECK(quadrance3d(p1, p2) == 9);
}

// ===========================================================================
// spread
// ===========================================================================

TEST_CASE("spread") {
    using F = Fraction<int>;
    auto v1 = std::make_pair(F::one(), F::one());
    auto v2 = std::make_pair(F::one(), F::zero());
    auto s = spread(v1, v2);
    CHECK(s.numer() == 1);
    CHECK(s.denom() == 2);
}

TEST_CASE("spread3d") {
    using F = Fraction<int>;
    auto v1 = std::make_tuple(F::one(), F::zero(), F::zero());
    auto v2 = std::make_tuple(F::zero(), F::one(), F::zero());
    auto s = spread3d(v1, v2);
    CHECK(s.numer() == 1);
    CHECK(s.denom() == 1);
}

// ===========================================================================
// cross
// ===========================================================================

TEST_CASE("cross 2d") {
    auto v1 = std::make_pair(1, 1);
    auto v2 = std::make_pair(1, 0);
    CHECK(cross(v1, v2) == -1);
}

TEST_CASE("cross3d") {
    auto v1 = std::make_tuple(1, 0, 0);
    auto v2 = std::make_tuple(0, 1, 0);
    auto result = cross3d(v1, v2);
    CHECK(std::get<0>(result) == 0);
    CHECK(std::get<1>(result) == 0);
    CHECK(std::get<2>(result) == 1);
}

// ===========================================================================
// quadrance from line
// ===========================================================================

TEST_CASE("quadrance from line") {
    using F = Fraction<int>;
    auto p = std::make_pair(F::one(), F::one());
    auto l = std::make_tuple(F::one(), F::one(), F::one());
    auto q = quadrance_from_line(p, l);
    CHECK(q.numer() == 9);
    CHECK(q.denom() == 2);
}

// ===========================================================================
// spread from line
// ===========================================================================

TEST_CASE("spread from line") {
    using F = Fraction<int>;
    auto l1 = std::make_tuple(F::one(), F::one(), F::one());
    auto l2 = std::make_tuple(F::one(), F::zero(), F::zero());
    auto s = spread_from_line(l1, l2);
    CHECK(s.numer() == 1);
    CHECK(s.denom() == 2);
}

// ===========================================================================
// cross from line
// ===========================================================================

TEST_CASE("cross from line") {
    auto l1 = std::make_tuple(1, 1, 1);
    auto l2 = std::make_tuple(1, 0, 0);
    CHECK(cross_from_line(l1, l2) == -1);
}

// ===========================================================================
// quadrance from three points
// ===========================================================================

TEST_CASE("quadrance from three points") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 0);
    auto p3 = std::make_pair(0, 1);
    auto result = quadrance_from_three_points(p1, p2, p3);
    CHECK(std::get<0>(result) == 2);
    CHECK(std::get<1>(result) == 1);
    CHECK(std::get<2>(result) == 1);
}

// ===========================================================================
// spread from three points
// ===========================================================================

TEST_CASE("spread from three points") {
    using F = Fraction<int>;
    auto p1 = std::make_pair(F::zero(), F::zero());
    auto p2 = std::make_pair(F::one(), F::zero());
    auto p3 = std::make_pair(F::zero(), F::one());
    auto s = spread_from_three_points(p1, p2, p3);
    CHECK(std::get<0>(s).numer() == 1);
    CHECK(std::get<1>(s).numer() == 1);
    CHECK(std::get<1>(s).denom() == 2);
    CHECK(std::get<2>(s).numer() == 1);
    CHECK(std::get<2>(s).denom() == 2);
}

// ===========================================================================
// cross from three points
// ===========================================================================

TEST_CASE("cross from three points") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 0);
    auto p3 = std::make_pair(0, 1);
    CHECK(cross_from_three_points(p1, p2, p3) == 1);
}

// ===========================================================================
// twist
// ===========================================================================

TEST_CASE("twist") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 0);
    auto p3 = std::make_pair(0, 1);
    CHECK(twist(p1, p2, p3) == 1);
}

TEST_CASE("twist negative") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 0);
    auto p3 = std::make_pair(0, -1);
    CHECK(twist(p1, p2, p3) == -1);
}

// ===========================================================================
// turn
// ===========================================================================

TEST_CASE("turn") {
    using F = Fraction<int>;
    auto p1 = std::make_pair(F::zero(), F::zero());
    auto p2 = std::make_pair(F::one(), F::zero());
    auto p3 = std::make_pair(F::one(), F::one());
    auto result = turn(p1, p2, p3);
    CHECK(result.first.numer() == 1);
    CHECK(result.first.denom() == 1);
    CHECK(result.second);
}

// ===========================================================================
// dilatation
// ===========================================================================

TEST_CASE("dilatation") {
    using F = Fraction<int>;
    auto v1 = std::make_pair(F::one(), F::zero());
    auto v2 = std::make_pair(F::from(2, 1), F::zero());
    auto d = dilatation(v1, v2);
    CHECK(d.numer() == 4);
    CHECK(d.denom() == 1);
}

// ===========================================================================
// sine / cosine law
// ===========================================================================

TEST_CASE("sine law product") {
    CHECK(sine_law_product(Fraction<int>::from(4, 1), Fraction<int>::from(1, 2))
          == Fraction<int>::from(2, 1));
}

TEST_CASE("cosine law") {
    // Right triangle with quadrances 2, 1, 1 -> spread opposite q1 = 1
    CHECK(
        cosine_law(Fraction<int>::from(2, 1), Fraction<int>::from(1, 1), Fraction<int>::from(1, 1))
        == Fraction<int>::from(1, 1));
}

// ===========================================================================
// const_trigonom concrete types
// ===========================================================================

TEST_CASE("quadrance i32") { CHECK(quadrance_i32({1, 1}, {4, 5}) == 25); }

TEST_CASE("archimedes i32") { CHECK(archimedes_i32(1, 2, 3) == 8); }

TEST_CASE("cross i32") { CHECK(cross_i32({1, 1}, {1, 0}) == -1); }

TEST_CASE("quadrance i64") { CHECK(quadrance_i64({1, 1}, {4, 5}) == 25); }

TEST_CASE("archimedes i64") { CHECK(archimedes_i64(1, 2, 3) == 8); }

TEST_CASE("cross i64") { CHECK(cross_i64({1, 1}, {1, 0}) == -1); }

// ===========================================================================
// Geometry: Point2D
// ===========================================================================

TEST_CASE("Point2D") {
    auto p = Point2D<int>::new_(3, 5);
    CHECK(p.x == 3);
    CHECK(p.y == 5);
}

TEST_CASE("Point2D from_pair") {
    auto p = Point2D<int>::from_pair({1, 2});
    CHECK(p.x == 1);
    CHECK(p.y == 2);
}

// ===========================================================================
// Geometry: Vector2D
// ===========================================================================

TEST_CASE("Vector2D add") {
    auto v1 = Vector2D<int>::new_(1, 2);
    auto v2 = Vector2D<int>::new_(3, 4);
    auto result = v1 + v2;
    CHECK(result.x == 4);
    CHECK(result.y == 6);
}

TEST_CASE("Vector2D sub") {
    auto v1 = Vector2D<int>::new_(3, 4);
    auto v2 = Vector2D<int>::new_(1, 2);
    auto result = v1 - v2;
    CHECK(result.x == 2);
    CHECK(result.y == 2);
}

// ===========================================================================
// Geometry: Triangle2D
// ===========================================================================

TEST_CASE("Triangle2D quadrances") {
    auto p1 = Point2D<int>::new_(0, 0);
    auto p2 = Point2D<int>::new_(1, 0);
    auto p3 = Point2D<int>::new_(0, 1);
    auto t = Triangle2D<int>(p1, p2, p3);
    auto q = t.quadrances();
    CHECK(std::get<0>(q) == 2);
    CHECK(std::get<1>(q) == 1);
    CHECK(std::get<2>(q) == 1);
}

TEST_CASE("Triangle2D area") {
    auto p1 = Point2D<int>::new_(0, 0);
    auto p2 = Point2D<int>::new_(1, 0);
    auto p3 = Point2D<int>::new_(0, 1);
    auto t = Triangle2D<int>(p1, p2, p3);
    CHECK(t.area() == 4);
}

TEST_CASE("Triangle2D twist") {
    auto p1 = Point2D<int>::new_(0, 0);
    auto p2 = Point2D<int>::new_(1, 0);
    auto p3 = Point2D<int>::new_(0, 1);
    auto t = Triangle2D<int>(p1, p2, p3);
    CHECK(t.twist() == 1);
}

TEST_CASE("Triangle2D is_degenerate") {
    auto p1 = Point2D<int>::new_(0, 0);
    auto p2 = Point2D<int>::new_(1, 1);
    auto p3 = Point2D<int>::new_(2, 2);
    auto t = Triangle2D<int>(p1, p2, p3);
    CHECK(t.is_degenerate());
}

// ===========================================================================
// Validation
// ===========================================================================

TEST_CASE("are_collinear") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 1);
    auto p3 = std::make_pair(2, 2);
    CHECK(are_collinear(p1, p2, p3));
}

TEST_CASE("not collinear") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 0);
    auto p3 = std::make_pair(0, 1);
    CHECK(!are_collinear(p1, p2, p3));
}

TEST_CASE("is_valid_triangle") {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 0);
    auto p3 = std::make_pair(0, 1);
    CHECK(is_valid_triangle(p1, p2, p3));
}

TEST_CASE("is_right_triangle") {
    CHECK(is_right_triangle(1, 0, 0));
    CHECK(!is_right_triangle(0, 0, 0));
}

TEST_CASE("are_lines_parallel") {
    auto l1 = std::make_tuple(1, 1, 0);
    auto l2 = std::make_tuple(2, 2, 1);
    CHECK(are_lines_parallel(l1, l2));
}

TEST_CASE("are_lines_perpendicular") {
    auto l1 = std::make_tuple(1, 0, 0);
    auto l2 = std::make_tuple(0, 1, 0);
    CHECK(are_lines_perpendicular(l1, l2));
}

TEST_CASE("point_on_line") {
    auto point = std::make_pair(1, 1);
    auto line = std::make_tuple(1, -1, 0);
    CHECK(point_on_line(point, line));
}

TEST_CASE("point_in_triangle") {
    using F = Fraction<int>;
    auto point = std::make_pair(F::from(1, 2), F::from(1, 4));
    auto p1 = std::make_pair(F::zero(), F::zero());
    auto p2 = std::make_pair(F::one(), F::zero());
    auto p3 = std::make_pair(F::zero(), F::one());
    CHECK(point_in_triangle(point, p1, p2, p3));
}

TEST_CASE("is_valid_quadrance") {
    CHECK(is_valid_quadrance(4));
    CHECK(is_valid_quadrance(0));
    CHECK(!is_valid_quadrance(-1));
}

TEST_CASE("is_valid_spread") {
    using F = Fraction<int>;
    CHECK(is_valid_spread(F::zero()));
    CHECK(is_valid_spread(F::from(1, 2)));
    CHECK(is_valid_spread(F::one()));
    CHECK(!is_valid_spread(F::from(-1, 10)));
    CHECK(!is_valid_spread(F::from(11, 10)));
}

TEST_CASE("is_acute_triangle") {
    using F = Fraction<int>;
    CHECK(is_acute_triangle(F::from(3, 10), F::from(3, 10), F::from(3, 10)));
    CHECK(!is_acute_triangle(F::one(), F::from(3, 10), F::from(3, 10)));
}

TEST_CASE("is_obtuse_triangle") {
    using F = Fraction<int>;
    CHECK(is_obtuse_triangle(F::from(3, 5), F::from(3, 10), F::from(3, 10)));
    CHECK(!is_obtuse_triangle(F::from(2, 5), F::from(2, 5), F::from(2, 5)));
}

// ===========================================================================
// Fraction type support
// ===========================================================================

using Frac = Fraction<int>;

TEST_CASE("archimedes with Fraction<int>") {
    auto q1 = Frac::from(1, 2);
    auto q2 = Frac::from(1, 4);
    auto q3 = Frac::from(1, 6);
    auto result = rattrig::archimedes(q1, q2, q3);
    CHECK(result.numer() == 23);
    CHECK(result.denom() == 144);
}

TEST_CASE("quadrance with Fraction<int>") {
    auto p1 = std::make_pair(Frac::zero(), Frac::zero());
    auto p2 = std::make_pair(Frac::from(3, 1), Frac::from(4, 1));
    auto q = quadrance(p1, p2);
    CHECK(q.numer() == 25);
    CHECK(q.denom() == 1);
}

TEST_CASE("spread with Fraction<int>") {
    auto v1 = std::make_pair(Frac::one(), Frac::one());
    auto v2 = std::make_pair(Frac::one(), Frac::zero());
    auto s = spread(v1, v2);
    // spread = 1 - (1*1+1*0)² / ((1²+1²)*(1²+0²)) = 1 - 1/2 = 1/2
    CHECK(s.numer() == 1);
    CHECK(s.denom() == 2);
}

TEST_CASE("cross with Fraction<int>") {
    auto v1 = std::make_pair(Frac::one(), Frac::one());
    auto v2 = std::make_pair(Frac::one(), Frac::zero());
    auto c = cross(v1, v2);
    CHECK(c.numer() == -1);
    CHECK(c.denom() == 1);
}

TEST_CASE("archimedes with Fraction<int64_t>") {
    using F64 = Fraction<int64_t>;
    auto q1 = F64::from(1, 2);
    auto q2 = F64::from(1, 4);
    auto q3 = F64::from(1, 6);
    auto result = rattrig::archimedes(q1, q2, q3);
    CHECK(result.numer() == 23);
    CHECK(result.denom() == 144);
}

TEST_CASE("quadrance_from_line with Fraction<int>") {
    auto p = std::make_pair(Frac::one(), Frac::one());
    auto l = std::make_tuple(Frac::one(), Frac::one(), Frac::one());
    auto q = quadrance_from_line(p, l);
    // (1+1+1)² / (1²+1²) = 9/2
    CHECK(q.numer() == 9);
    CHECK(q.denom() == 2);
}

TEST_CASE("spread_from_three_points with Fraction<int>") {
    auto p1 = std::make_pair(Frac::zero(), Frac::zero());
    auto p2 = std::make_pair(Frac::one(), Frac::zero());
    auto p3 = std::make_pair(Frac::zero(), Frac::one());
    auto s = spread_from_three_points(p1, p2, p3);
    // Right triangle with sides sqrt(2), 1, 1
    CHECK(std::get<0>(s).numer() == 1);  // s1 = 1 (right angle)
    CHECK(std::get<0>(s).denom() == 1);
    CHECK(std::get<1>(s).numer() == 1);  // s2 = 1/2
    CHECK(std::get<1>(s).denom() == 2);
    CHECK(std::get<2>(s).numer() == 1);  // s3 = 1/2
    CHECK(std::get<2>(s).denom() == 2);
}

TEST_CASE("cross_from_three_points with Fraction<int>") {
    auto p1 = std::make_pair(Frac::zero(), Frac::zero());
    auto p2 = std::make_pair(Frac::one(), Frac::zero());
    auto p3 = std::make_pair(Frac::zero(), Frac::one());
    auto c = cross_from_three_points(p1, p2, p3);
    CHECK(c.numer() == 1);
    CHECK(c.denom() == 1);
}

TEST_CASE("cosine_law with Fraction<int>") {
    // Right triangle with quadrances 2, 1, 1 -> spread opposite q1 = 1
    auto q1 = Frac::from(2, 1);
    auto q2 = Frac::from(1, 1);
    auto q3 = Frac::from(1, 1);
    auto s = rattrig::cosine_law(q1, q2, q3);
    CHECK(s.numer() == 1);
    CHECK(s.denom() == 1);
}

TEST_CASE("Triangle2D with Fraction<int>") {
    auto p1 = Point2D<Frac>::new_(Frac::zero(), Frac::zero());
    auto p2 = Point2D<Frac>::new_(Frac::from(3, 1), Frac::zero());
    auto p3 = Point2D<Frac>::new_(Frac::zero(), Frac::from(4, 1));
    auto t = Triangle2D<Frac>(p1, p2, p3);
    auto q = t.quadrances();
    // 3-4-5 triangle: q1=25 (3²+4²), q2=16 (4²), q3=9 (3²)
    CHECK(std::get<0>(q).numer() == 25);
    CHECK(std::get<1>(q).numer() == 16);
    CHECK(std::get<2>(q).numer() == 9);
    // area = 4 * 9 * 16 - (9+16-25)² = 576 - 0 = 576
    CHECK(t.area().numer() == 576);
    CHECK(!t.is_degenerate());
}

TEST_CASE("are_collinear with Fraction<int>") {
    auto p1 = std::make_pair(Frac::zero(), Frac::zero());
    auto p2 = std::make_pair(Frac::one(), Frac::one());
    auto p3 = std::make_pair(Frac::from(2, 1), Frac::from(2, 1));
    CHECK(are_collinear(p1, p2, p3));

    auto p4 = std::make_pair(Frac::zero(), Frac::one());
    CHECK(!are_collinear(p1, p2, p4));
}

// ===========================================================================
// Mixed integer / Fraction operations (via fractions-simple dependency)
// ===========================================================================
//
// These verify that Fraction<T> mixed arithmetic (T op Fraction and
// Fraction op T) works correctly when included through the rattrig build.

TEST_CASE("mixed Fraction op T arithmetic") {
    using F = Fraction<int>;

    // Addition
    CHECK(F::from(1, 2) + 1 == F::from(3, 2));
    CHECK(F::from(2, 3) + 4 == F::from(14, 3));
    CHECK(F::from(3, 5) + 2 == F::from(13, 5));

    // Subtraction
    CHECK(F::from(1, 2) - 1 == F::from(-1, 2));
    CHECK(F::from(2, 3) - 4 == F::from(-10, 3));

    // Multiplication
    CHECK(F::from(2, 3) * 4 == F::from(8, 3));
    CHECK(F::from(3, 4) * 2 == F::from(3, 2));

    // Division
    CHECK(F::from(1, 2) / 2 == F::from(1, 4));
    CHECK(F::from(2, 3) / 4 == F::from(1, 6));
}

TEST_CASE("mixed T op Fraction arithmetic") {
    using F = Fraction<int>;

    // Addition
    CHECK(1 + F::from(1, 2) == F::from(3, 2));
    CHECK(4 + F::from(2, 3) == F::from(14, 3));

    // Subtraction
    CHECK(1 - F::from(1, 2) == F::from(1, 2));
    CHECK(4 - F::from(2, 3) == F::from(10, 3));

    // Multiplication
    CHECK(2 * F::from(2, 3) == F::from(4, 3));
    CHECK(3 * F::from(1, 2) == F::from(3, 2));

    // Division
    CHECK(2 / F::from(1, 3) == F::from(6, 1));
    CHECK(4 / F::from(2, 3) == F::from(6, 1));
}

TEST_CASE("mixed scalar compound assign") {
    using F = Fraction<int>;

    // += T (regression: gcd(numer_, rhs) > 1)
    auto f1 = F::from(2, 3); f1 += 4;
    CHECK(f1 == F::from(14, 3));

    // -= T
    auto f2 = F::from(2, 3); f2 -= 4;
    CHECK(f2 == F::from(-10, 3));

    // *= T
    auto f3 = F::from(2, 3); f3 *= 4;
    CHECK(f3 == F::from(8, 3));

    // /= T
    auto f4 = F::from(2, 3); f4 /= 4;
    CHECK(f4 == F::from(1, 6));
}

TEST_CASE("mixed operations in context") {
    // Use mixed op results inside rattrig functions (uniform types)
    using F = Fraction<int>;

    // archimedes with mixed op in the computation chain:
    // 4*F(1,3)*F(1,3) = 4/9, F(1,3)+F(1,3)-F(1,3) = 1/3, (1/3)^2 = 1/9
    // result: 4/9 - 1/9 = 3/9 = 1/3
    auto a = rattrig::archimedes(F::from(1, 3), F::from(1, 3), F::from(1, 3));
    CHECK(a.numer() == 1);
    CHECK(a.denom() == 3);

    // spread using mixed op result as input to rattrig function
    // v1=(3,1), v2=(1,1): dot=4, q1=10, q2=2 → spread = 1 - 16/20 = 1/5
    auto s = rattrig::spread(std::make_pair(F::from(3, 1), F::from(1, 1)),
                             std::make_pair(F::from(1, 1), F::from(1, 1)));
    CHECK(s.numer() == 1);
    CHECK(s.denom() == 5);

    // cosine law with mixed op values
    auto c = rattrig::cosine_law(F::from(2, 1), F::from(1, 1) + 0, F::from(1, 1) * 1);
    CHECK(c.numer() == 1);
    CHECK(c.denom() == 1);
}

TEST_CASE("mixed i64 arithmetic via fractions-simple") {
    using F64 = Fraction<int64_t>;
    CHECK(F64::from(2, 3) + 4 == F64::from(14, 3));
    CHECK(4 + F64::from(2, 3) == F64::from(14, 3));
    CHECK(F64::from(2, 3) * 4 == F64::from(8, 3));
    CHECK(2 * F64::from(2, 3) == F64::from(4, 3));
}
