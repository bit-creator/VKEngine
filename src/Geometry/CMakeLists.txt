include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_library ( Geometry STATIC
    Geometry.cppm
    Quad.cppm
    Allocator.cppm
    Triangle.cppm
    Attributes.cppm
)