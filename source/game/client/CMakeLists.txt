if(BUILD_CLIENT)
    add_executable(vclient
        "${CMAKE_CURRENT_LIST_DIR}/entity/interpolation.cc"
        "${CMAKE_CURRENT_LIST_DIR}/entity/interpolation.hh"
        "${CMAKE_CURRENT_LIST_DIR}/entity/player_look.cc"
        "${CMAKE_CURRENT_LIST_DIR}/entity/player_look.hh"
        "${CMAKE_CURRENT_LIST_DIR}/entity/player_move.cc"
        "${CMAKE_CURRENT_LIST_DIR}/entity/player_move.hh"
        "${CMAKE_CURRENT_LIST_DIR}/entity/player_target.cc"
        "${CMAKE_CURRENT_LIST_DIR}/entity/player_target.hh"
        "${CMAKE_CURRENT_LIST_DIR}/event/glfw_cursor_pos.hh"
        "${CMAKE_CURRENT_LIST_DIR}/event/glfw_framebuffer_size.hh"
        "${CMAKE_CURRENT_LIST_DIR}/event/glfw_key.hh"
        "${CMAKE_CURRENT_LIST_DIR}/event/glfw_mouse_button.hh"
        "${CMAKE_CURRENT_LIST_DIR}/event/glfw_scroll.hh"
        "${CMAKE_CURRENT_LIST_DIR}/event/language_set.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/background.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/background.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/chat.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/chat.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/gui_screen.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/imdraw_ext.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/imdraw_ext.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/language.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/language.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/main_menu.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/main_menu.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/message_box.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/message_box.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/play_menu.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/play_menu.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/player_list.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/player_list.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/progress.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/progress.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/settings.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/settings.hh"
        "${CMAKE_CURRENT_LIST_DIR}/gui/splash.cc"
        "${CMAKE_CURRENT_LIST_DIR}/gui/splash.hh"
        "${CMAKE_CURRENT_LIST_DIR}/hud/crosshair.cc"
        "${CMAKE_CURRENT_LIST_DIR}/hud/crosshair.hh"
        "${CMAKE_CURRENT_LIST_DIR}/hud/hotbar.cc"
        "${CMAKE_CURRENT_LIST_DIR}/hud/hotbar.hh"
        "${CMAKE_CURRENT_LIST_DIR}/hud/metrics.cc"
        "${CMAKE_CURRENT_LIST_DIR}/hud/metrics.hh"
        "${CMAKE_CURRENT_LIST_DIR}/hud/status_lines.cc"
        "${CMAKE_CURRENT_LIST_DIR}/hud/status_lines.hh"
        "${CMAKE_CURRENT_LIST_DIR}/resource/texture2D.cc"
        "${CMAKE_CURRENT_LIST_DIR}/resource/texture2D.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_mesher.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_mesher.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_quad.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_renderer.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_renderer.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_vbo.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_visibility.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/chunk_visibility.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/outline.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/outline.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/skybox.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/skybox.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/voxel_anims.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/voxel_anims.hh"
        "${CMAKE_CURRENT_LIST_DIR}/world/voxel_atlas.cc"
        "${CMAKE_CURRENT_LIST_DIR}/world/voxel_atlas.hh"
        "${CMAKE_CURRENT_LIST_DIR}/const.hh"
        "${CMAKE_CURRENT_LIST_DIR}/experiments.cc"
        "${CMAKE_CURRENT_LIST_DIR}/experiments.hh"
        "${CMAKE_CURRENT_LIST_DIR}/game.cc"
        "${CMAKE_CURRENT_LIST_DIR}/game.hh"
        "${CMAKE_CURRENT_LIST_DIR}/globals.cc"
        "${CMAKE_CURRENT_LIST_DIR}/globals.hh"
        "${CMAKE_CURRENT_LIST_DIR}/keyboard.cc"
        "${CMAKE_CURRENT_LIST_DIR}/keyboard.hh"
        "${CMAKE_CURRENT_LIST_DIR}/keynames.cc"
        "${CMAKE_CURRENT_LIST_DIR}/keynames.hh"
        "${CMAKE_CURRENT_LIST_DIR}/main.cc"
        "${CMAKE_CURRENT_LIST_DIR}/mouse.cc"
        "${CMAKE_CURRENT_LIST_DIR}/mouse.hh"
        "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh"
        "${CMAKE_CURRENT_LIST_DIR}/receive.cc"
        "${CMAKE_CURRENT_LIST_DIR}/receive.hh"
        "${CMAKE_CURRENT_LIST_DIR}/screenshot.cc"
        "${CMAKE_CURRENT_LIST_DIR}/screenshot.hh"
        "${CMAKE_CURRENT_LIST_DIR}/session.cc"
        "${CMAKE_CURRENT_LIST_DIR}/session.hh"
        "${CMAKE_CURRENT_LIST_DIR}/toggles.cc"
        "${CMAKE_CURRENT_LIST_DIR}/toggles.hh"
        "${CMAKE_CURRENT_LIST_DIR}/varied_program.cc"
        "${CMAKE_CURRENT_LIST_DIR}/varied_program.hh"
        "${CMAKE_CURRENT_LIST_DIR}/view.cc"
        "${CMAKE_CURRENT_LIST_DIR}/view.hh")
    target_compile_definitions(vclient PUBLIC GLFW_INCLUDE_NONE)
    target_include_directories(vclient PRIVATE "${PROJECT_SOURCE_DIR}/source")
    target_include_directories(vclient PRIVATE "${PROJECT_SOURCE_DIR}/source/game")
    target_precompile_headers(vclient PRIVATE "${CMAKE_CURRENT_LIST_DIR}/precompiled.hh")
    target_link_libraries(vclient PUBLIC shared)

    target_include_directories(vclient PUBLIC "${EXTERNAL_INCLUDE_DIR}")
    target_link_directories(vclient PUBLIC "${EXTERNAL_LIBRARY_DIR}")
    target_link_libraries(vclient PUBLIC glad glfw3 imgui imgui_glfw imgui_opengl3)

    if(WIN32 AND MSVC)
        # GLFW defines APIENTRY and ENet includes
        # Windows API headers which also define APIENTRY
        target_compile_options(vclient PRIVATE /wd4005)
    endif()

    if(WIN32)
        enable_language(RC)
        target_sources(vclient PRIVATE "${CMAKE_CURRENT_LIST_DIR}/vclient.rc")
    endif()

    install(TARGETS vclient RUNTIME DESTINATION ".")
endif()
