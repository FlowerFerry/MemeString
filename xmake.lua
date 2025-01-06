
set_project("meme_string")

set_xmakever("2.6.0")

set_version("0.0.1", {build = "%Y%m%d%H%M"})

add_rules("mode.debug", "mode.release")

option("memestr_test_enable")
    set_default(false)
    set_showmenu(true)
    set_description("Enable unit test of meme library")
option_end()

option("memestr_benchmark_enable")
    set_default(false)
    set_showmenu(true)
    set_description("Enable benchmark of meme library")
option_end()

function check_c_compiler_flag(flag)
    import("core.tool.compiler")

    local test_file = path.join(os.tmpdir(), "check_c_compiler_flag_test.c")
    local test_prog = path.join(os.tmpdir(), "check_c_compiler_flag_test")

    io.writefile(test_file, "int main(int argc, char **argv) { return 0; }")

    local ok = os.runv(compiler.features("cc"):binary(), {flag, "-o", testprog, testfile})

    os.rm(test_file)
    os.rm(test_prog)

    return ok
end

-- stdc = "c11"
-- set_languages(stdc)

add_includedirs("include", {public = true})

target("meme_string")
    add_defines("MEME_OPTION__BUILD_SHARED")
    set_languages("c11")
    add_includedirs(
        "include_private",
        "3rdparty/cvector/include"
    )
    add_files("src/*.c")
    add_syslinks("pthread", "dl")
    if is_os("windows") ~= true then
        add_syslinks("rt")
        add_cxxflags("-fPIC", "-fexceptions")
        add_ldflags ("-fPIC", "-fexceptions")
    end
    import("core.project.config")    
    if config.get("cc") ~= "cl" then
        local supports_avx2 = check_c_compiler_flag("-mavx2")
        local supports_neon = check_c_compiler_flag("-mfpu=neon")

        if supports_avx2 then
            add_cflags("-mavx2")
        end

        if supports_neon then
            add_cflags("-mfpu=neon")
        end
    end
    set_kind("shared")
    set_symbols("hidden")
    add_rpathdirs("$ORIGIN")
target_end()

target("mmpp_unittest")
    set_kind("binary")
    set_languages("c11", "cxx17")
    add_includedirs(
        "3rdparty/catch2/include"
    )
    add_files("unit_test/mmpp_unittest/*.cpp")
    add_syslinks("pthread", "dl")
    if is_os("windows") ~= true then
        add_syslinks("rt")
    end
    if has_config("test_enable") then
        add_deps("meme_string")
        add_links("meme_string")
    end
    on_load(function (target)
        if has_config("test_enable") then
            target:set("enabled", true)
        else
            target:set("enabled", false)
        end
    end)
    add_rpathdirs("$ORIGIN")
target_end()

target("mmpp_benchmark")
    set_kind("binary")
    set_languages("c11", "cxx17")
    add_files("test/mmpp_benchmark/*.cpp")
    add_syslinks("pthread", "dl")
    if is_os("windows") ~= true then
        add_syslinks("rt")
    end
    if has_config("benchmark_enable") then
        add_deps("meme_string")
        add_links("meme_string")
    end
    on_load(function (target)
        if has_config("benchmark_enable") then
            target:set("enabled", true)
        else
            target:set("enabled", false)
        end
    end)
    add_rpathdirs("$ORIGIN")
target_end()
