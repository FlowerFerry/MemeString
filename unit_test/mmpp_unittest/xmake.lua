
set_project("mmpp_unittest")

set_xmakever("2.6.0")

set_version("0.0.1", {build = "%Y%m%d%H%M"})

add_rules("mode.debug", "mode.release")

include ("../../xmake.lua")

target("mmpp_unittest")
    set_kind("binary")
    set_languages("cxx11")
    add_files("*.cpp")
    add_includedirs(
        "../../3rdparty/include"
    )
    add_deps("meme_string")
    add_syslinks ("pthread", "dl", "rt")
    add_links("meme_string")
    add_rpathdirs("$ORIGIN")
    add_rpathdirs("$ORIGIN/../lib")
