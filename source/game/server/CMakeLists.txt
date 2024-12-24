if(BUILD_SERVER)
    add_executable(vserver
        "${CMAKE_CURRENT_LIST_DIR}/chat.cc"
        "${CMAKE_CURRENT_LIST_DIR}/chat.hh"
        "${CMAKE_CURRENT_LIST_DIR}/game.cc"
        "${CMAKE_CURRENT_LIST_DIR}/game.hh"
        "${CMAKE_CURRENT_LIST_DIR}/globals.cc"
        "${CMAKE_CURRENT_LIST_DIR}/globals.hh"
        "${CMAKE_CURRENT_LIST_DIR}/main.cc"
        "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh"
        "${CMAKE_CURRENT_LIST_DIR}/receive.cc"
        "${CMAKE_CURRENT_LIST_DIR}/receive.hh"
        "${CMAKE_CURRENT_LIST_DIR}/sessions.cc"
        "${CMAKE_CURRENT_LIST_DIR}/sessions.hh"
        "${CMAKE_CURRENT_LIST_DIR}/status.cc"
        "${CMAKE_CURRENT_LIST_DIR}/status.hh"
        "${CMAKE_CURRENT_LIST_DIR}/whitelist.cc"
        "${CMAKE_CURRENT_LIST_DIR}/whitelist.hh")
    target_include_directories(vserver PRIVATE "${PROJECT_SOURCE_DIR}/source")
    target_include_directories(vserver PRIVATE "${PROJECT_SOURCE_DIR}/source/game")
    target_precompile_headers(vserver PRIVATE "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh")
    target_link_libraries(vserver PUBLIC shared)
    
    target_include_directories(vserver PUBLIC "${EXTERNAL_INCLUDE_DIR}")
    target_link_directories(vserver PUBLIC "${EXTERNAL_LIBRARY_DIR}")

    if(WIN32)
        enable_language(RC)
        target_sources(vserver PRIVATE "${CMAKE_CURRENT_LIST_DIR}/vserver.rc")
    endif()

    install(TARGETS vserver RUNTIME DESTINATION ".")
endif()
