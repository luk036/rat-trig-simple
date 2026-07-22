#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

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

static void bench_archimedes(ankerl::nanobench::Bench& b) {
    // Integer archimedes
    b.run("archimedes(int) 3-4-5", [&] {
        auto result = rattrig::archimedes(3, 4, 5);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    // int64_t
    b.run("archimedes(int64_t) 3-4-5", [&] {
        auto result = rattrig::archimedes(int64_t(3), int64_t(4), int64_t(5));
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    // Fraction<int>
    auto fa1 = Fraction<int>::from(1, 2);
    auto fa2 = Fraction<int>::from(1, 4);
    auto fa3 = Fraction<int>::from(1, 6);
    b.run("archimedes(Fraction<int>) 1/2 1/4 1/6", [&] {
        auto result = rattrig::archimedes(fa1, fa2, fa3);
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// quadrance
// ===========================================================================

static void bench_quadrance(ankerl::nanobench::Bench& b) {
    // int
    auto p1_i = std::make_pair(0, 0);
    auto p2_i = std::make_pair(3, 4);
    b.run("quadrance(int) (0,0)-(3,4)", [&] {
        auto result = quadrance(p1_i, p2_i);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    // Fraction<int>
    using F = Fraction<int>;
    auto p1_f = std::make_pair(F::zero(), F::zero());
    auto p2_f = std::make_pair(F::from(3, 1), F::from(4, 1));
    b.run("quadrance(Fraction<int>) (0,0)-(3,4)", [&] {
        auto result = quadrance(p1_f, p2_f);
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// spread
// ===========================================================================

static void bench_spread(ankerl::nanobench::Bench& b) {
    // Fraction<int>
    using F = Fraction<int>;
    auto v1 = std::make_pair(F::one(), F::one());
    auto v2 = std::make_pair(F::one(), F::zero());
    b.run("spread(Fraction<int>) (1,1)-(1,0)", [&] {
        auto result = spread(v1, v2);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    // int
    auto v1_i = std::make_pair(1, 1);
    auto v2_i = std::make_pair(1, 0);
    b.run("spread(int) (1,1)-(1,0)", [&] {
        auto result = spread(v1_i, v2_i);
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// cross
// ===========================================================================

static void bench_cross(ankerl::nanobench::Bench& b) {
    // int
    auto v1_i = std::make_pair(1, 1);
    auto v2_i = std::make_pair(1, 0);
    b.run("cross(int) (1,1)-(1,0)", [&] {
        auto result = cross(v1_i, v2_i);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    // Fraction<int>
    using F = Fraction<int>;
    auto v1_f = std::make_pair(F::one(), F::one());
    auto v2_f = std::make_pair(F::one(), F::zero());
    b.run("cross(Fraction<int>) (1,1)-(1,0)", [&] {
        auto result = cross(v1_f, v2_f);
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// 3D operations
// ===========================================================================

static void bench_3d(ankerl::nanobench::Bench& b) {
    auto p1 = std::make_tuple(0, 0, 0);
    auto p2 = std::make_tuple(1, 2, 2);

    b.run("quadrance3d(int) (0,0,0)-(1,2,2)", [&] {
        auto result = quadrance3d(p1, p2);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    auto v1 = std::make_tuple(1, 0, 0);
    auto v2 = std::make_tuple(0, 1, 0);

    b.run("cross3d(int) (1,0,0)-(0,1,0)", [&] {
        auto result = cross3d(v1, v2);
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// Geometry: Triangle2D
// ===========================================================================

static void bench_triangle2d(ankerl::nanobench::Bench& b) {
    auto p1 = Point2D<int>::new_(0, 0);
    auto p2 = Point2D<int>::new_(3, 0);
    auto p3 = Point2D<int>::new_(0, 4);
    auto t = Triangle2D<int>(p1, p2, p3);

    b.run("Triangle2D<int> quadrances", [&] {
        auto result = t.quadrances();
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    b.run("Triangle2D<int> area", [&] {
        auto result = t.area();
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    b.run("Triangle2D<int> twist", [&] {
        auto result = t.twist();
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// Validation
// ===========================================================================

static void bench_validation(ankerl::nanobench::Bench& b) {
    auto p1 = std::make_pair(0, 0);
    auto p2 = std::make_pair(1, 1);
    auto p3 = std::make_pair(2, 2);

    b.run("are_collinear(int) collinear", [&] {
        auto result = are_collinear(p1, p2, p3);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    auto p4 = std::make_pair(0, 1);
    b.run("are_collinear(int) not collinear", [&] {
        auto result = are_collinear(p1, p2, p4);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    using F = Fraction<int>;
    auto l1 = std::make_tuple(F::one(), F::one(), F::zero());
    auto l2 = std::make_tuple(F::from(2, 1), F::from(2, 1), F::one());
    b.run("are_lines_parallel(Fraction<int>)", [&] {
        auto result = are_lines_parallel(l1, l2);
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// cosine_law / sine_law_product
// ===========================================================================

static void bench_trigonometry(ankerl::nanobench::Bench& b) {
    using F = Fraction<int>;
    auto q1 = F::from(2, 1);
    auto q2 = F::from(1, 1);
    auto q3 = F::from(1, 1);

    b.run("cosine_law(Fraction<int>) right triangle", [&] {
        auto result = rattrig::cosine_law(q1, q2, q3);
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    b.run("sine_law_product(Fraction<int>)", [&] {
        auto result = sine_law_product(F::from(4, 1), F::from(1, 2));
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ===========================================================================
// Main
// ===========================================================================

int main() {
    ankerl::nanobench::Bench bench;
    bench.title("rat-trig-simple benchmarks")
        .unit("iteration")
        .warmup(100)
        .minEpochIterations(5000);

    bench_archimedes(bench);
    bench_quadrance(bench);
    bench_spread(bench);
    bench_cross(bench);
    bench_3d(bench);
    bench_triangle2d(bench);
    bench_validation(bench);
    bench_trigonometry(bench);
}
