#add_library(Engine INTERFACE)
#target_sources(Engine INTERFACE
#        FILE_SET HEADERS
#        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
#        FILES
#
#        Engine.cppm
#        Interfaces.cppm
#        EventBus.cppm
#        ColliderSystem.cppm
#        Components/ColliderComponent.cppm
#        ColliderTool.cppm
#        Registry.cppm
#)

add_library(Engine)
target_sources(Engine
        PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES

        Engine.cppm
        #        Interfaces.cppm
        #        EventBus.cppm
        #        ColliderSystem.cppm
        #        Components/ColliderComponent.cppm
        #        ColliderTool.cppm
        #        Registry.cppm

        PRIVATE
        #        Engine.tpp
        #        Engine.impl.cppm
)


#target_sources(Engine PRIVATE
#        Engine/Events/EventBus.tpp
#        Engine/DOD/SparseSet.tpp
#        Engine/DOD/Registry.tpp
#        Engine/Utils/Logger.tpp
#)

#target_sources(Engine PRIVATE Engine.tpp)

# Configuración de dependencias
target_link_libraries(
        Engine PUBLIC
        #        EnginePCH
        #        ImGuiBackends
        imgui::imgui
        fmt::fmt
        nlohmann_json::nlohmann_json
        SQLiteCpp
        ${SDL2_LIBRARIES}
        ${LUA_LIBRARY}
        ${THREAD_LIBRARIES}
        ${OPENGL_VIDEO_LIBRARIES}
)

target_compile_options(SQLiteCpp INTERFACE -fexceptions)


target_compile_options(Engine PUBLIC
        ${BASE_FLAGS}
        $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
        $<$<CONFIG:Release>:${RELEASE_FLAGS}>
        $<$<CONFIG:RelWithDebInfo>:${RELWITHDEBINFO_FLAGS}>
        $<$<CONFIG:MinSizeRel>:${MINSIZEREL_FLAGS}>
)


target_compile_features(Engine PRIVATE cxx_std_26)