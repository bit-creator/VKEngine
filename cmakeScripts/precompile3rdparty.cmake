function(precompile module)
    add_custom_target(${module} 
    COMMAND 
      ${CMAKE_CXX_COMPILER} 
      -cc1 ${3RDPARTY_DIR}/module.modulemap -o ${PRECOMPILED_MODULES_DIR}/${module}.pcm 
      -emit-module -fmodules -fmodule-name=${module} -DVK_VERSION_1_0 -fmath-errno
      -internal-isystem /usr/lib/llvm-13/lib/clang/13.0.0/include 
      -internal-externc-isystem /usr/include 
      -internal-externc-isystem /usr/include/x86_64-linux-gnu 
      -O3 -std=c++${CMAKE_CXX_STANDARD} -fgnuc-version=4.2.1 
      -fno-implicit-modules -fcxx-exceptions 
      -fexceptions -x c++
    )
endfunction(precompile)