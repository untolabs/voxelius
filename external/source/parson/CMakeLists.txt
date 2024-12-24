add_library(parson STATIC "${CMAKE_CURRENT_LIST_DIR}/parson.c")
target_include_directories(parson PUBLIC "${EXTERNAL_INCLUDE_DIR}")
set_target_properties(parson PROPERTIES FOLDER External)
