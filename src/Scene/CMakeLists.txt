include(${CMAKE_SCRIPTS_DIR}/precompile.cmake)

precompile ( Scene          Scene.cppm  )
precompile ( Scene.Entity   Entity.cppm )
precompile ( Scene.Camera   Camera.cppm )
precompile ( Scene.Object   Object.cppm )

set(sources ${sources} PARENT_SCOPE)