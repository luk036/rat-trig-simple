add_rules("mode.debug", "mode.release")
set_languages("c++17")

if is_plat("windows") then
    add_cxflags("/utf-8", "/W4", "/WX", "/wd4819")
else
    add_cxflags("-Wall", "-Wextra", "-Wpedantic", "-Werror")
end

if not is_plat("windows") then
    add_rules("mode.coverage")
end

add_requires("doctest")

target("rattrig")
    set_kind("headeronly")
    add_headerfiles("include/(rattrig/**.hpp)")
    add_includedirs("include", {public = true})

target("test_rattrig")
    set_kind("binary")
    add_deps("rattrig")
    add_files("tests/test_rattrig.cpp")
    add_includedirs("include")
    add_includedirs("../fractions-simple/include")
    add_packages("doctest")
    add_tests("default")
