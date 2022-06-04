function(precompile module)
    add_custom_target(${module} 
    COMMAND 
      ${CMAKE_CXX_COMPILER} 
      -cc1 ${3RDPARTY_DIR}/module.modulemap -o ${PRECOMPILED_MODULES_DIR}/${module}.pcm 
      -emit-module -fmodules -fmodule-name=${module} -DVK_VERSION_1_0
      -internal-isystem /usr/lib/llvm-14/lib/clang/14.0.5/include 
      -internal-externc-isystem /usr/include 
      -internal-externc-isystem /usr/include/x86_64-linux-gnu 
      -Wall
      -Wextra
      -O3
      -ffast-math
      -std=c++${CMAKE_CXX_STANDARD} 
      -fgnuc-version=4.2.1 
      -fno-implicit-modules 
      -stdlib=libc++
      -fcxx-exceptions 
      -fexceptions 
      -x c++
    )
endfunction(precompile)