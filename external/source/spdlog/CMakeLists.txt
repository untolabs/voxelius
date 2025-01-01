add_library(spdlog STATIC
    "${CMAKE_CURRENT_LIST_DIR}/async.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/bundled_fmtlib_format.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/cfg.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/color_sinks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/file_sinks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/spdlog.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/stdout_sinks.cpp")
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)
target_include_directories(spdlog PUBLIC "${EXTERNAL_INCLUDE_DIR}")
set_target_properties(spdlog PROPERTIES FOLDER External)

find_package(Threads REQUIRED)
target_link_libraries(spdlog PUBLIC Threads::Threads)

if(MSVC)
    # There seems to be a compile-time check merged into the
    # master fmtlib branch that has been pulled into spdlog as well
    # that seems to fix an MSVC bug (https://github.com/fmtlib/fmt/pull/2297)
    # FIXME: should I bundle fmtlib as a separate dependency?
    target_compile_options(spdlog PUBLIC /utf-8)
endif()
