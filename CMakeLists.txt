cmake_minimum_required(VERSION 3.25)
project(Box2D_Colliders)

set(CMAKE_CXX_STANDARD 23)

add_executable(Box2D_Colliders
        main.cpp

        lib/nativefiledialog/nfd_common.c
        lib/nativefiledialog/nfd_zenity.c
        lib/SDL_FontCache/SDL_FontCache.c
        lib/SDL2_gxf/SDL2_gfxPrimitives.c
        lib/SDL2_gxf/SDL2_rotozoom.c
        lib/tinyxml2/tinyxml2.cpp
        lib/sqlite/sqlite3.c

        src/Utils/utils.cpp

        src/Graphics/graphics.cpp
        src/Graphics/text.cpp
        src/Graphics/render.cpp

        src/App/core.cpp
        src/App/loop.cpp
        src/App/data.cpp

        src/App/Settings/serialise.cpp
        src/App/Settings/save.cpp

        src/App/Bounding_Boxes/line_segment.cpp
        src/App/Bounding_Boxes/aabb.cpp
        src/App/Bounding_Boxes/circle.cpp
        src/App/Bounding_Boxes/point.cpp
        src/App/Bounding_Boxes/polygon.cpp
        src/App/Bounding_Boxes/shape.cpp

        src/Input/keyboard.cpp
        src/Input/mouse.cpp
        src/Input/window.cpp
        src/Input/actions.cpp

        src/UI/scroll_bar.cpp

        src/Panels/menu.cpp
        src/Panels/top.cpp
        src/Panels/bottom.cpp
        src/Panels/center.cpp
        src/Panels/Center/left.cpp
        src/Panels/Center/right.cpp
        src/Panels/Center/center.cpp

        src/Output/json.cpp
        src/Output/xml.cpp
        src/Output/sqlite.cpp
)

target_link_libraries(Box2D_Colliders -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
