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
  # Fetch fractions-simple from GitHub (provides the fraction target)
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
  CPMAddPackage(
    NAME fractions-simple
    GITHUB_REPOSITORY luk036/fractions-simple
    VERSION 0.1.1
    OPTIONS "FRACTION_BUILD_TESTS OFF"
  )

  # Create doctest target if fractions-simple hasn't already
  if(NOT TARGET doctest)
    set(DOCTEST_VERSION 2.4.11)
    set(DOCTEST_DIR "${CMAKE_BINARY_DIR}/_deps/doctest")
    set(DOCTEST_HEADER "${DOCTEST_DIR}/doctest/doctest.h")
    if(NOT EXISTS "${DOCTEST_HEADER}")
      file(MAKE_DIRECTORY "${DOCTEST_DIR}/doctest")
      file(DOWNLOAD
           "https://raw.githubusercontent.com/doctest/doctest/v${DOCTEST_VERSION}/doctest/doctest.h"
           "${DOCTEST_HEADER}" STATUS download_status)
      list(GET download_status 0 status_code)
      if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download doctest (status ${status_code})")
      endif()
    endif()
    add_library(doctest INTERFACE)
    target_include_directories(doctest SYSTEM INTERFACE "${DOCTEST_DIR}")
    if(MSVC)
      target_compile_options(doctest INTERFACE /wd5285)
    endif()
  endif()

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
