# Builds the plugin shared library used by mmpp_unittest to exercise
# import_from_dll / export_into_dll across a real module boundary.
#
# The plugin is built from this subdirectory as a child target of
# mmpp_unittest, and emitted into the same directory as the test executables
# so the loader finds it without extra path juggling.

add_library(mmpp_dll_plugin SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/plugin.cpp
)

target_compile_features(mmpp_dll_plugin PRIVATE cxx_std_17)

target_include_directories(mmpp_dll_plugin PRIVATE
  ${MMUT_PARENT_HEADER_DIR}
)

target_link_libraries(mmpp_dll_plugin PRIVATE
  ${MMUT_PARENT_LINKER_FILE}
)

# Keep the name free of the platform default "lib" prefix so dlsym/LoadLibrary
# lookups are identical on every platform.
set_target_properties(mmpp_dll_plugin PROPERTIES
  PREFIX ""
  OUTPUT_NAME "mmpp_dll_plugin"
)
