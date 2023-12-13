
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

    if (string.find(get_config("arch"), "x64") ~= nil or string.find(get_config("arch"), "x86_64") ~= nil or string.find(get_config("arch"), "amd64") ~= nil) 
    then
        local xxComp = get_config("cxx")
        local ccComp = get_config("cc")
        if ((xxComp ~= nil and string.find(xxComp, "g++") ~= nil) or (ccComp ~= nil and string.find(ccComp, "gcc") ~= nil)) 
        then
            add_cxflags("-fasm-blocks")
            add_ldflags("-fasm-blocks")
        end
    end
