function(precompile module source)
add_custom_target(${module})
add_custom_command(TARGET ${module}
  COMMAND
    ${CMAKE_CXX_COMPILER}
    -std=c++${CMAKE_CXX_STANDARD}
    -stdlib=libc++
    -fmodules
    -O3
    -fPIC
    -Wall
    -Wextra
    -fmath-errno
    -march=native
    -ffast-math
    -fcxx-exceptions
    -fmodules-cache-path=/home/user/Code/C++/VKEngine/.build/cache
    -fprebuilt-module-path=${PRECOMPILED_MODULES_DIR}
    -fprebuilt-module-path=Mathon/precompiled
    -c --precompile ${CMAKE_CURRENT_SOURCE_DIR}/${source}
    -o ${PRECOMPILED_MODULES_DIR}/${module}.pcm 
)

set(sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/${source} PARENT_SCOPE)
endfunction(precompile)