add_library(common STATIC
    "${CMAKE_CURRENT_LIST_DIR}/resource/binary_file.cc"
    "${CMAKE_CURRENT_LIST_DIR}/resource/binary_file.hh"
    "${CMAKE_CURRENT_LIST_DIR}/resource/image.cc"
    "${CMAKE_CURRENT_LIST_DIR}/resource/image.hh"
    "${CMAKE_CURRENT_LIST_DIR}/resource/resource.hh"
    "${CMAKE_CURRENT_LIST_DIR}/cmdline.cc"
    "${CMAKE_CURRENT_LIST_DIR}/cmdline.hh"
    "${CMAKE_CURRENT_LIST_DIR}/config.cc"
    "${CMAKE_CURRENT_LIST_DIR}/config.hh"
    "${CMAKE_CURRENT_LIST_DIR}/crc64.cc"
    "${CMAKE_CURRENT_LIST_DIR}/crc64.hh"
    "${CMAKE_CURRENT_LIST_DIR}/epoch.cc"
    "${CMAKE_CURRENT_LIST_DIR}/epoch.hh"
    "${CMAKE_CURRENT_LIST_DIR}/fstools.cc"
    "${CMAKE_CURRENT_LIST_DIR}/fstools.hh"
    "${CMAKE_CURRENT_LIST_DIR}/packet_buffer.cc"
    "${CMAKE_CURRENT_LIST_DIR}/packet_buffer.hh"
    "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh"
    "${CMAKE_CURRENT_LIST_DIR}/strtools.cc"
    "${CMAKE_CURRENT_LIST_DIR}/strtools.hh")
target_include_directories(common PRIVATE "${PROJECT_SOURCE_DIR}/source")
target_precompile_headers(common PRIVATE "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh")
target_link_libraries(common PUBLIC mathlib)

target_include_directories(common PUBLIC "${EXTERNAL_INCLUDE_DIR}")
target_link_directories(common PUBLIC "${EXTERNAL_LIBRARY_DIR}")
target_link_libraries(common PUBLIC physfs spdlog stb)

if(WIN32)
    target_compile_definitions(common PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    target_compile_options(common PUBLIC /wd4018)
    target_compile_options(common PUBLIC /wd4244)
    target_compile_options(common PUBLIC /wd4267)
endif()
