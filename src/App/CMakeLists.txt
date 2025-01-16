target_sources(Box2D_Colliders
        PUBLIC
        core.cpp
        data.cpp
        loop.cpp
        Settings/save.cpp
        Settings/serialise.cpp
        Bounding_Boxes/aabb.cpp
        Bounding_Boxes/circle.cpp
        Bounding_Boxes/line_segment.cpp
        Bounding_Boxes/point.cpp
        Bounding_Boxes/polygon.cpp
        Bounding_Boxes/e_shape.cpp
)

target_include_directories(Box2D_Colliders PUBLIC ${CMAKE_CURRENT_LIST_DIR})