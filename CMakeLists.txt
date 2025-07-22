cmake_minimum_required(VERSION 3.31.6)
project(Modules_Testing VERSION 1.0.0 LANGUAGES CXX)

# ###############################################################

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# This option tells CMake to export the compile commands for the target.
# This can be useful for debugging and profiling purposes.
# Generate compile_commands.json to make it easier to work with clang based
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This option tells CMake to generate position-independent code (PIC).
# PIC code can be loaded and executed at any address in memory.
# This is necessary when building shared/dynamic libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
#set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

# ###############################################################

# Usar mold como linker (más rápido que ld/gold)
find_program(MOLD_PATH mold)
if (MOLD_PATH)
    set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=mold")
    set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=mold")
    set(CMAKE_MODULE_LINKER_FLAGS "-fuse-ld=mold")
endif ()

# ###############################################################

find_package(fmt REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(imgui REQUIRED)

# ###############################################################

set(BASE_FLAGS
        #        -Wall
        #        -Wextra
        #        -Wpedantic
        -m64
        -march=native
        -ffast-math
        -fno-math-errno ## Este fue un cambio
        -fstrict-aliasing
        -fPIC         # Para libs SHARED
        -Wwrite-strings
        -fno-exceptions
        -fno-rtti ## Probar, no se puede usar dynamic_cast ni typeid
        -ftrivial-auto-var-init=pattern ## Va a estar por default en C++26
        -pipe # Si usas GCC o Clang, este flag evita recompilar archivos que no cambiaron
        -ffunction-sections  # Optimiza código muerto (útil si se combina con linker flags)
        -fdata-sections      # Lo mismo pero para datos
        -fno-threadsafe-statics # Evita código thread-safe para estáticos
        #        -fno-ident          # Ignora identificadores de compilación
        #        -fno-unwind-tables  # Reduce información de unwind
        #        -fno-asynchronous-unwind-tables
)

set(DEBUG_FLAGS
        -g3                           # Máxima información de debug
        -ggdb3                   # Más información específica para GDB
        -gdwarf-5                # Usar DWARF5 (mejor soporte) # Formato moderno de debug info
        -O0                      # Desactivar TODAS las optimizaciones
        -fno-eliminate-unused-debug-types
        -fno-omit-frame-pointer       # CRUCIAL para profiling # Mejor para backtraces
        -fno-inline                   # Evita inline para ver funciones reales (crítico para templates)
        -fno-optimize-sibling-calls   # Preserva call stack
        -D_DEBUG
        -fasynchronous-unwind-tables  # NECESARIO para stack unwinding
        -funwind-tables               # NECESARIO para backtraces
)

set(RELEASE_FLAGS
        -O3
        -DNDEBUG
        #        -g
        #        -O0
        #        -flto                  # Link Time Optimization
        #        -flto=auto
        -fno-stack-protector   # Si seguridad no es crítica
        -fomit-frame-pointer   # Mejor rendimiento, peor debugging
        #        -fvisibility=hidden    # Mejor optimización de símbolos
        -march=native
        -fno-trapping-math
        -funroll-loops
        -fno-semantic-interposition  # Mejor optimización
)

set(RELWITHDEBINFO_FLAGS
        -g
        -O2
        -DNDEBUG
)

set(MINSIZEREL_FLAGS
        -Os
        -DNDEBUG
)


if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Compilando con Clang (${CMAKE_CXX_COMPILER}) ++++++++++++++++++++++++++")
    list(APPEND BASE_FLAGS
            -stdlib=libstdc++ ## Linux, no usar -stdlib=libc++ ## Mac y FreeBSD
            #            -ftime-trace -c
            #            -fmodules
            #            -fimplicit-modules
            #            -fimplicit-module-maps
    )

    list(APPEND RELEASE_FLAGS
            #            -stdlib=libstdc++ ## Linux, no usar -stdlib=libc++ ## Mac y FreeBSD
            #            -ftime-trace -c
            #            -fmodules
            #            -fimplicit-modules
            #            -fimplicit-module-maps
    )

    add_compile_options(
            -fmodules
            -fbuiltin-module-map
            -fmodules-cache-path=${CMAKE_BINARY_DIR}/module.cache
            -fPIC
    )

    add_link_options(-fmodules)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "Compilando con GCC (${CMAKE_CXX_COMPILER}) ++++++++++++++++++++++++++")
    list(APPEND BASE_FLAGS
            #            -fprebuilt-module-path
            #            -static-libgcc
            -static-libstdc++
            -fcoroutines  # Para corrutinas
            -fconcepts    # Para conceptos avanzados
    )
    list(APPEND DEBUG_FLAGS
            -fno-inline-small-functions
            -fkeep-inline-functions
            -rdynamic                     # Exporta símbolos para backtraces
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "Compilando con MSVC")
    # Flags específicos para MSVC podrían ir aquí
else ()
    message(WARNING "Compilador no reconocido: ${CMAKE_CXX_COMPILER_ID} ++++++++++++++++++++++++++")
endif ()

# ###############################################################

#add_subdirectory(src/Engine)
add_subdirectory(src/Game)

add_executable(Modules_Testing)
target_sources(Modules_Testing PRIVATE src/main.cpp)
target_link_libraries(Modules_Testing PRIVATE Game)

target_compile_features(Modules_Testing PRIVATE cxx_std_26)

target_link_libraries(
        Modules_Testing PUBLIC
        #        imgui::imgui
        fmt::fmt
        nlohmann_json::nlohmann_json
        SQLiteCpp
        ${SDL2_LIBRARIES}
        ${LUA_LIBRARY}
        ${THREAD_LIBRARIES}
        ${OPENGL_VIDEO_LIBRARIES}
)

# ###############################################################

target_compile_options(Modules_Testing PUBLIC
        ${BASE_FLAGS}
        $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
        $<$<CONFIG:Release>:${RELEASE_FLAGS}>
        $<$<CONFIG:RelWithDebInfo>:${RELWITHDEBINFO_FLAGS}>
        $<$<CONFIG:MinSizeRel>:${MINSIZEREL_FLAGS}>
)

# ###############################################################

get_target_property(compile_options Modules_Testing COMPILE_OPTIONS)
if (compile_options)
    message(STATUS "Opciones de compilación para mi_ejecutable: ${compile_options}")
else ()
    message(STATUS "No hay opciones de compilación definidas para mi_ejecutable")
endif ()

# ###############################################################

message(STATUS "\n -- MY -- CMake FLAGS --")
message(STATUS " C FLAGS ${CMAKE_C_FLAGS}")
message(STATUS " C++ FLAGS ${CMAKE_CXX_FLAGS} ")
message(STATUS " C++ FLAGS Debug ${CMAKE_CXX_FLAGS_DEBUG} ")
message(STATUS " C++ FLAGS Release ${CMAKE_CXX_FLAGS_RELEASE} ")
message(STATUS " C++ FLAGS Min ${CMAKE_CXX_FLAGS_MINSIZEREL} ")
message(STATUS " C++ FLAGS RelWDebInfo ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ")
message(STATUS "\n -- MY -- CMake FLAGS --\n\n")

# ###############################################################