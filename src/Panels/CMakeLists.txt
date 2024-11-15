target_sources(Box2D_Colliders
        PUBLIC
        bottom.cpp
        center.cpp
        menu.cpp
        top.cpp
        Center/center.cpp
        Center/left.cpp
        Center/right.cpp
)

target_include_directories(Box2D_Colliders PUBLIC ${CMAKE_CURRENT_LIST_DIR})