
include(../cmakeScripts/precompile3rdparty.cmake)

precompile  ( Vulkan )
precompile  ( GLFW   )
precompile  ( Image  )

add_library ( 3rdparty SHARED
    ${3RDPARTY_DIR}/image.cpp
)