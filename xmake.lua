
set_project("meme_string")

set_xmakever("2.6.0")

set_version("0.0.1", {build = "%Y%m%d%H%M"})

add_rules("mode.debug", "mode.release")

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
    add_syslinks("pthread", "dl", )
    if is_os("windows") != true then
        add_syslinks("rt")
    end
    set_kind("shared")
    set_symbols("hidden")
    add_rpathdirs("$ORIGIN")
