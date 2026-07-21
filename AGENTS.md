# Agent Guidelines for rat-trig-simple

## Project Overview

C++ header-only library for rational trigonometry — a modern approach to classical trigonometry using rational numbers instead of irrational values.
Port of [rat-trig-rs](https://github.com/luk036/rat-trig-rs).

## Build & Test Commands

### Primary — xmake

```bash
xmake f -m release    # configure
xmake                 # build
xmake run test_rattrig  # run all tests
```

### Secondary — CMake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## Project Structure

```
include/
└── rattrig/
    ├── rattrig.hpp       # Core rational trigonometry functions
    ├── geometry.hpp      # Geometry primitive types
    └── validation.hpp    # Geometric validation utilities
tests/
└── test_rattrig.cpp      # doctest test cases
CMakeLists.txt            # CMake build
cmake/
└── CPM.cmake             # CPM dependency manager
xmake.lua                 # xmake build (preferred)
```

## Before Committing

1. Run `xmake f -m release && xmake run test_rattrig` (or CMake equivalent)
2. Run `xmake format` to check formatting
3. Ensure all test cases pass
