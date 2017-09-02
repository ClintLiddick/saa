project('saa', 'c', 'cpp',
        default_options: ['c_std=c11', 'cpp_std=c++14'])

add_project_arguments('-Wall',
                      '-Wextra',
                      '-Werror',
                      '-pedantic',
                      '-pedantic-errors',
                      '-Wfloat-equal',
                      '-Wmissing-declarations',
                      '-Wmissing-include-dirs',
                      '-Wodr',
                      '-Wpointer-arith',
                      '-Wshadow',
                      '-Wswitch-default',
                      '-Wswitch-enum',
                      '-flto',
                      language: 'cpp')

cxx = meson.get_compiler('cpp')

gl = dependency('gl')
glfw = dependency('glfw3')
libdl = cxx.find_library('dl', required: false)
threads = dependency('threads')

incdir = include_directories('include')

libglad = shared_library('glad',
                         'src/glad.c',
                         include_directories: incdir,
                         dependencies: [gl, libdl],
)

saa_srcs = [
    'src/window.cpp'
]

libsaa = shared_library('saa', saa_srcs,
                        include_directories: incdir,
                        link_with: [libglad],
                        dependencies: [glfw, threads],
)
                        

executable('saa_main', 'src/saa_main.cpp',
           include_directories: incdir,
           link_with: [libsaa],
)
