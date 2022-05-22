include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Geometry.Triangle  Triangle.cppm  )
precompile ( Vk.HostAllocator   Allocator.cppm  )

set(sources ${sources} PARENT_SCOPE)