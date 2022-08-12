add_compile_options(
    -std=c++${CMAKE_CXX_STANDARD}
    -stdlib=libc++
    -fmodules
    -g3
    -Wall
    -Wextra
    -fmath-errno
    -march=native
    -ffast-math
    -fcxx-exceptions
    -fprebuilt-module-path=${PRECOMPILED_MODULES_DIR}
)