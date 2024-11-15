cmake_minimum_required(VERSION 3.25)
project(Box2D_Colliders)

set(CMAKE_CXX_STANDARD 23)

include_directories(include/all)

if(UNIX)
    add_executable(Box2D_Colliders
        src/main.cpp
        include/all/nativefiledialog/nfd_zenity.c
        include/all/nativefiledialog/nfd_common.c
        include/all/SDL_FontCache/SDL_FontCache.c
        include/all/SDL2_gxf/SDL2_gfxPrimitives.c
        include/all/SDL2_gxf/SDL2_rotozoom.c
        include/all/tinyxml2/tinyxml2.cpp
        include/all/sqlite/sqlite3.c
    )
    target_link_libraries(Box2D_Colliders -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
elseif(WIN32)
    add_executable(Box2D_Colliders
            src/main.cpp
            include/all/nativefiledialog/nfd_win.cpp
            include/all/nativefiledialog/nfd_common.c
            include/all/SDL_FontCache/SDL_FontCache.c
            include/all/SDL2_gxf/SDL2_gfxPrimitives.c
            include/all/SDL2_gxf/SDL2_rotozoom.c
            include/all/tinyxml2/tinyxml2.cpp
            include/all/sqlite/sqlite3.c
    )
    link_directories(${CMAKE_SOURCE_DIR}/lib/win)
    include_directories(include/win)
    target_link_libraries(Box2D_Colliders
            ${CMAKE_SOURCE_DIR}/lib/win/SDL2.lib
            ${CMAKE_SOURCE_DIR}/lib/win/SDL2_gfx.lib
            ${CMAKE_SOURCE_DIR}/lib/win/SDL2_image.lib
            ${CMAKE_SOURCE_DIR}/lib/win/SDL2_ttf.lib
            ${CMAKE_SOURCE_DIR}/lib/win/SDL2_mixer.lib
    )
    target_compile_definitions(Box2D_Colliders PRIVATE SDL_MAIN_HANDLED)
endif()

add_subdirectory(src)