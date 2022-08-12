include(${CMAKE_SCRIPTS_DIR}/settings.cmake)

add_library ( Scene STATIC
    Scene.cppm
    Entity.cppm
    Camera.cppm
    Object.cppm
)