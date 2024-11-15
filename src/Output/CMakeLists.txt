target_sources(Box2D_Colliders
        PUBLIC
        json.cpp
        sqlite.cpp
        xml.cpp
)

target_include_directories(Box2D_Colliders PUBLIC ${CMAKE_CURRENT_LIST_DIR})