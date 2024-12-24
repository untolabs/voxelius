add_library(FNL STATIC "${CMAKE_CURRENT_LIST_DIR}/fastnoiselite.c")
target_compile_definitions(FNL PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(FNL PUBLIC "${EXTERNAL_INCLUDE_DIR}")
set_target_properties(FNL PROPERTIES FOLDER External)
