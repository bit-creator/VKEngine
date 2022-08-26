include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Geometry            Geometry.cppm   )
precompile ( Geometry.Quad       Quad.cppm       )
precompile ( Vk.HostAllocator    Allocator.cppm  )
precompile ( Geometry.Triangle   Triangle.cppm   )
precompile ( Geometry.Attributes Attributes.cppm )

set(sources ${sources} PARENT_SCOPE)