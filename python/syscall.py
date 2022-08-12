from enum import Enum, auto
import subprocess

class BuildType(Enum):
    Debug = auto(),
    Release = auto()

compiler = 'clang++-14'
standart = 20
stdlib   = 'libc++'
build    = BuildType.Debug

flags    = [
    '-fmodules',
    '-Wall',
    '-Wextra',
    '-fmath-errno',
    '-march=native',
    '-ffast-math',
    '-fcxx-exceptions'
]

prebuild_directory = [
    'precompiled',
    'Mathon/precompiled'
]

def prebuild(module, module_path):
    dirs=[]
    for path in prebuild_directory:
        dirs.append('-fprebuilt-module-path='+path)

    call = [
        compiler,
        '-std=c++' + str(standart),
        '-stdlib=' + stdlib,
        '-g3' if build == BuildType.Debug else '-O3',
    ]
    call.extend(flags)
    call.extend(dirs)
    call.extend([
        '-c', '--precompile', module_path,
        '-o', 'precompiled/' + module + '.pcm'
    ])

    subprocess.run(call)