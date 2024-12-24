add_library(miniz STATIC "${CMAKE_CURRENT_LIST_DIR}/miniz.c")
target_compile_definitions(miniz PUBLIC MINIZ_NO_STDIO MINIZ_NO_ZLIB_COMPATIBLE_NAMES)
target_include_directories(miniz PUBLIC "${EXTERNAL_INCLUDE_DIR}")
set_target_properties(miniz PROPERTIES FOLDER External)
