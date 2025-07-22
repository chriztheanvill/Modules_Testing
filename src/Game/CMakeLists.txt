add_library(Game)
target_sources(Game
        PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES

        ## Base
        Game.cppm

        ## Scenes
        Scenes/Scenes.cppm ##
        Scenes/Scene.cppm
        Scenes/SceneManager.cppm
        Scenes/SceneManager.impl.cpp
)

# Agrega fuentes de implementación por separado
#target_sources(Game PRIVATE
#        Scenes/SceneManager.impl.cpp
#)

#target_link_libraries(Game PUBLIC Engine)


# Configuración de dependencias
target_link_libraries(
        Game PUBLIC
        #        EnginePCH
        imgui::imgui
        fmt::fmt
        nlohmann_json::nlohmann_json
        SQLiteCpp
        ${SDL2_LIBRARIES}
        ${LUA_LIBRARY}
        ${THREAD_LIBRARIES}
        ${OPENGL_VIDEO_LIBRARIES}
)


target_compile_options(Game PUBLIC
        ${BASE_FLAGS}
        $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
        $<$<CONFIG:Release>:${RELEASE_FLAGS}>
        $<$<CONFIG:RelWithDebInfo>:${RELWITHDEBINFO_FLAGS}>
        $<$<CONFIG:MinSizeRel>:${MINSIZEREL_FLAGS}>
)

target_compile_features(Game PRIVATE cxx_std_26)