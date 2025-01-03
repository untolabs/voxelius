add_library(salad STATIC "${CMAKE_CURRENT_LIST_DIR}/salad.c")
target_include_directories(salad PUBLIC "${EXTERNAL_INCLUDE_DIR}")
target_compile_definitions(salad PRIVATE SALAD_PARANOID)
target_link_libraries(salad PUBLIC ${CMAKE_DL_LIBS})
