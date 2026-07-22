add_rules("mode.debug", "mode.release")
set_languages("c++17")

if is_plat("windows") then
    add_cxflags("/utf-8", "/W4", "/WX", "/wd4819")
else
    add_cxflags("-Wall", "-Wextra", "-Wpedantic", "-Werror")
end

-- mode.coverage adds --coverage which only GCC/Clang support
if not is_plat("windows") then
    add_rules("mode.coverage")
end

add_requires("doctest")
add_requires("nanobench")

target("rattrig")
    set_kind("headeronly")
    add_headerfiles("include/(rattrig/**.hpp)")
    add_includedirs("include", {public = true})

target("test_rattrig")
    set_kind("binary")
    add_deps("rattrig")
    add_files("tests/test_rattrig.cpp")
    add_includedirs("include")
    if os.isdir("fractions-simple/include") then
        add_includedirs("fractions-simple/include")
    else
        add_includedirs("../fractions-simple/include")
    end
    add_packages("doctest")
    add_tests("default")

target("benchmark_rattrig")
    set_kind("binary")
    add_deps("rattrig")
    add_files("tests/benchmark_rattrig.cpp")
    add_includedirs("include")
    if os.isdir("fractions-simple/include") then
        add_includedirs("fractions-simple/include")
    else
        add_includedirs("../fractions-simple/include")
    end
    add_packages("nanobench")

-- ---------------------------------------------------------------------------
-- clang-tidy task
-- ---------------------------------------------------------------------------

task("clang-tidy")
    set_category("format")
    set_menu {
        usage = "xmake clang-tidy [options]",
        description = "Run clang-tidy static analysis",
    }
    on_run(function()
        local ea = {"--extra-arg=-std=c++17", "--extra-arg=-Iinclude"}
        local args = {"clang-tidy", "--quiet"}
        for _, a in ipairs(ea) do
            table.insert(args, a)
        end
        table.insert(args, "include/rattrig/rattrig.hpp")
        if not os.exec(table.concat(args, " ")) then
            os.exit(1)
        end
    end)

-- ---------------------------------------------------------------------------
-- Doxygen task
-- ---------------------------------------------------------------------------

task("doxygen")
    set_category("doc")
    set_menu {
        usage = "xmake doxygen",
        description = "Generate Doxygen HTML documentation",
    }
    on_run(function()
        os.mkdir("docs/doxygen")
        local ok, err = os.iorun("doxygen Doxyfile")
        if ok then
            print("Doxygen documentation generated in docs/doxygen/")
        else
            print("Error: doxygen not found or failed. Install with: pip install doxygen")
            os.exit(1)
        end
    end)

-- ---------------------------------------------------------------------------
-- Format tasks
-- ---------------------------------------------------------------------------

task("fix-format")
    set_category("format")
    set_menu {
        usage = "xmake fix-format",
        description = "Apply clang-format formatting fixes",
    }
    on_run(function()
        local sources = {
            "include/rattrig/rattrig.hpp",
            "include/rattrig/geometry.hpp",
            "include/rattrig/validation.hpp",
            "tests/test_rattrig.cpp",
            "tests/benchmark_rattrig.cpp",
            "xmake.lua",
        }
        for _, src in ipairs(sources) do
            if os.isfile(src) then
                if src:match("%.hpp$") or src:match("%.cpp$") then
                    os.exec("clang-format -i -style=file " .. src)
                    print("  fixed: " .. src)
                end
            end
        end
    end)
