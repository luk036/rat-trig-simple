[![Actions Status](https://github.com/luk036/rat-trig-simple/workflows/CMake/badge.svg)](https://github.com/luk036/rat-trig-simple/actions)
[![Actions Status](https://github.com/luk036/rat-trig-simple/workflows/xmake/badge.svg)](https://github.com/luk036/rat-trig-simple/actions)

# 🧭 rat-trig-simple

Rational Trigonometry in C++ — a header-only library implementing the framework
developed by Norman Wildberger. It replaces classical angle/distance trigonometry
with **quadrance** (squared distance) and **spread** (squared sine), enabling
exact computation over rational numbers.

Simplified C++ port of [rat-trig-rs](https://github.com/luk036/rat-trig-rs).

## ✨ Features

- Header-only, C++17
- Exact rational arithmetic via `Fraction<T>` (from
  [fractions-simple](https://github.com/luk036/fractions-simple))
- Quadrance, spread, and triple-quad law computations
- Geometry primitives and validation utilities
- constexpr-friendly where possible

## Requirements

- C++17 compiler
- CMake 3.14+ (or xmake)

## Usage

### Build and test

```bash
# CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure

# xmake
xmake f -m release
xmake
xmake run test_rattrig
```

### Include in your code

```cpp
#include <rattrig/rattrig.hpp>

const auto q = rattrig::quad(std::vector<int>{3, 4});
// q == 25
```

## API

| Header | Purpose |
|--------|---------|
| `rattrig/rattrig.hpp` | Core rational trigonometry functions |
| `rattrig/geometry.hpp` | Geometry primitive types |
| `rattrig/validation.hpp` | Geometric validation utilities |

## Related

- [rat-trig-cpp](https://github.com/luk036/rat-trig-cpp) — full-featured rational
  trigonometry library (doctest + RapidCheck, Fractions dependency)
- [fractions-simple](https://github.com/luk036/fractions-simple) — exact rational arithmetic

## CI

This simplified variant runs a lighter CI set (`cmake.yml`, `xmake.yml`, `release.yml`)
than the full-featured [rat-trig-cpp](https://github.com/luk036/rat-trig-cpp), which additionally
runs `ubuntu/macos/windows/install/documentation` workflows.

## License

MIT
