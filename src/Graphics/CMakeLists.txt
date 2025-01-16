target_sources(Box2D_Colliders
        PUBLIC
        graphics.cpp
        render.cpp
        text.cpp
)

target_include_directories(Box2D_Colliders PUBLIC ${CMAKE_CURRENT_LIST_DIR})