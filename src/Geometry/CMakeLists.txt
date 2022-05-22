include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)


precompile ( Geometry           Geometry.cppm   )
precompile ( Geometry.Quad      Quad.cppm       )
precompile ( Geometry.Triangle  Triangle.cppm   )
precompile ( Vk.HostAllocator   Allocator.cppm  )

set(sources ${sources} PARENT_SCOPE)