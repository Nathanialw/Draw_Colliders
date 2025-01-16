target_sources(Box2D_Colliders
        PUBLIC
        actions.cpp
        keyboard.cpp
        mouse.cpp
        window.cpp
)

target_include_directories(Box2D_Colliders PUBLIC ${CMAKE_CURRENT_LIST_DIR})