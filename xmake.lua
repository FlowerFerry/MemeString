
set_project("meme_string")

set_xmakever("2.6.0")

set_version("0.0.1", {build = "%Y%m%d%H%M"})

add_rules("mode.debug", "mode.release")

option("test_enable")
    set_default(false)
    set_showmenu(true)
    set_description("Enable unit test")
option_end()

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
    end
    set_kind("shared")
    set_symbols("hidden")
    add_rpathdirs("$ORIGIN")
target_end()

target("mmpp_unittest")
    set_kind("binary")
    set_languages("c11", "cxx11")
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
