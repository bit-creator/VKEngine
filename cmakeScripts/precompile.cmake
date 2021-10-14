function(precompile module source)
add_custom_target(${module})
add_custom_command(TARGET ${module}
  COMMAND
    ${CMAKE_CXX_COMPILER}
    -std=c++${CMAKE_CXX_STANDARD}
    -fmodules
    -stdlib=libc++
    -fimplicit-modules
    -fimplicit-module-maps
    # -Wall
    # -O3
    -fprebuilt-module-path=${PRECOMPILED_MODULES_DIR}
    -c --precompile ${CMAKE_CURRENT_SOURCE_DIR}/${source}
    -o ${PRECOMPILED_MODULES_DIR}/${module}.pcm 
)
set(sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/${source} PARENT_SCOPE)
endfunction(precompile)