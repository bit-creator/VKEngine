
include(../cmakeScripts/precompile3rdparty.cmake)

precompile  ( Vulkan    )
precompile  ( GLFW      )
precompile  ( Image     )
precompile  ( Importer  )

add_library ( 3rdparty SHARED
    ${3RDPARTY_DIR}/3rdparty.cpp
)