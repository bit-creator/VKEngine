function(precompile module source)
add_custom_target(${module}
  COMMAND
    ${CMAKE_CXX_COMPILER}
    -std=c++${CMAKE_CXX_STANDARD}
    -fmodules
    -stdlib=libc++
    -fimplicit-modules
    -fimplicit-module-maps
    -fprebuilt-module-path=${CMAKE_CURRENT_SOURCE_DIR}/precompiled
    -c --precompile ${CMAKE_CURRENT_SOURCE_DIR}/${source}
    -o ${PRECOMPILED_MODULES_DIR}/${module}.pcm 
)
set(sources ${sources} ${source} PARENT_SCOPE)
endfunction(precompile)