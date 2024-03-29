
set_project("meme_string")

set_xmakever("2.6.0")

set_version("0.0.1", {build = "%Y%m%d%H%M"})

add_rules("mode.debug", "mode.release")

stdc = "c11"
set_languages(stdc)

add_includedirs(
    "include_private",
    "3rdparty/cvector/include"
)
add_includedirs("include", {public = true})

target("meme_string")
    add_defines("MEME_OPTION__BUILD_SHARED")
    add_files("src/*.c")
    add_syslinks("pthread", "dl", "rt")
    set_kind("shared")
    set_symbols("hidden")
    add_rpathdirs("$ORIGIN")
    -- if is_plat("cross", "linux") then
    --     add_ldflags("-Wl,-rpath,$ORIGIN", {force = true})
    -- end
