add_library(stb STATIC "${CMAKE_CURRENT_LIST_DIR}/stb.c")
target_compile_definitions(stb PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(stb PUBLIC "${EXTERNAL_INCLUDE_DIR}")
set_target_properties(stb PROPERTIES FOLDER External)
