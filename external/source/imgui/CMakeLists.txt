add_library(imgui STATIC
    "${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui_stdlib.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui_internal.h"
    "${CMAKE_CURRENT_LIST_DIR}/imgui_tables.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui.cpp")
target_include_directories(imgui PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(imgui PUBLIC "${EXTERNAL_INCLUDE_DIR}")
set_target_properties(imgui PROPERTIES FOLDER External)

add_library(imgui_glfw STATIC "${CMAKE_CURRENT_LIST_DIR}/imgui_impl_glfw.cpp")
target_link_libraries(imgui_glfw PUBLIC glfw3 imgui)
set_target_properties(imgui_glfw PROPERTIES FOLDER External)

add_library(imgui_opengl3 STATIC "${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp")
target_link_libraries(imgui_opengl3 PUBLIC imgui)
set_target_properties(imgui_opengl3 PROPERTIES FOLDER External)
