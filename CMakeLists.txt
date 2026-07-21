cmake_minimum_required(VERSION 3.14)
project(
  rat-trig-simple
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Compiler warnings (as errors)
# ---------------------------------------------------------------------------

function(target_enable_warnings TARGET)
  if(MSVC)
    target_compile_options(${TARGET} PRIVATE /W4 /WX /wd4819)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror)
  endif()
endfunction()

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(FRACTION_BUILD_TESTS "Build tests" ON)
option(FRACTION_ENABLE_COVERAGE "Enable coverage reporting" OFF)

# ---------------------------------------------------------------------------
# Header-only library target
# ---------------------------------------------------------------------------
add_library(rattrig INTERFACE)
target_include_directories(
  rattrig INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()

if(FRACTION_BUILD_TESTS)
  # Fetch fractions-simple from GitHub (provides doctest target)
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
  CPMAddPackage("gh:luk036/fractions-simple@0.1.1")

  add_executable(test_rattrig tests/test_rattrig.cpp)
  target_link_libraries(test_rattrig PRIVATE rattrig fraction doctest)
  target_enable_warnings(test_rattrig)

  if(MSVC)
    target_compile_options(test_rattrig PRIVATE /utf-8)
  endif()

  if(FRACTION_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
      target_compile_options(test_rattrig PRIVATE --coverage -fno-inline -fno-elide-constructors)
      target_link_options(test_rattrig PRIVATE --coverage)
    endif()
  endif()

  add_test(NAME test_rattrig COMMAND test_rattrig)
endif()

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.hpp"
)
