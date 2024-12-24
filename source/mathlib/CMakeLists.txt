add_library(mathlib STATIC
    "${CMAKE_CURRENT_LIST_DIR}/constexpr.hh"
    "${CMAKE_CURRENT_LIST_DIR}/floathacks.hh"
    "${CMAKE_CURRENT_LIST_DIR}/mat4x4base.hh"
    "${CMAKE_CURRENT_LIST_DIR}/mat4x4f.cc"
    "${CMAKE_CURRENT_LIST_DIR}/mat4x4f.hh"
    "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec2base.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec2f.cc"
    "${CMAKE_CURRENT_LIST_DIR}/vec2f.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec3angles.cc"
    "${CMAKE_CURRENT_LIST_DIR}/vec3angles.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec3base.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec3f.cc"
    "${CMAKE_CURRENT_LIST_DIR}/vec3f.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec4base.hh"
    "${CMAKE_CURRENT_LIST_DIR}/vec4f.cc"
    "${CMAKE_CURRENT_LIST_DIR}/vec4f.hh")
target_include_directories(mathlib PRIVATE "${PROJECT_SOURCE_DIR}/source")
target_precompile_headers(mathlib PRIVATE "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh")

target_include_directories(common PUBLIC "${EXTERNAL_INCLUDE_DIR}")
target_link_directories(common PUBLIC "${EXTERNAL_LIBRARY_DIR}")

if(WIN32)
    target_compile_definitions(mathlib PUBLIC _USE_MATH_DEFINES)
    target_compile_definitions(mathlib PUBLIC NOMINMAX)
endif()
