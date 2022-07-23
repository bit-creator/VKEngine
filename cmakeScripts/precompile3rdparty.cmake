function(precompile module)
    add_custom_target(${module} 
    COMMAND 
      ${CMAKE_CXX_COMPILER} 
      -cc1 ${3RDPARTY_DIR}/module.modulemap -o ${PRECOMPILED_MODULES_DIR}/${module}.pcm 
      -std=c++${CMAKE_CXX_STANDARD} 
      -stdlib=libc++ -emit-module -fmodules -fmodule-name=${module} -DVK_VERSION_1_0
      -internal-isystem /usr/lib/llvm-14/include/c++/v1 
      -internal-isystem /usr/lib/llvm-14/lib/clang/14.0.6/include
      -internal-isystem /usr/include
      -internal-externc-isystem ${CMAKE_SOURCE_DIR}assimp/include/assimp
      -internal-externc-isystem ${CMAKE_SOURCE_DIR}assimp/build64/include/assimp
      -Wall
      -Wextra
      -O3
      -ffast-math
      -fgnuc-version=4.2.1 
      -fno-implicit-modules 
      -fcxx-exceptions 
      -fexceptions 
      -x c++
    )
endfunction(precompile)