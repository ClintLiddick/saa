project('saa', 'c', 'cpp',
        default_options: ['c_std=c11', 'cpp_std=c++14'])

add_project_arguments('-Wall',
                      '-Wextra',
                      '-Werror',
                      '-pedantic',
                      '-pedantic-errors',
                      # '-Wfloat-equal',
                      '-Wmissing-declarations',
                      '-Wmissing-include-dirs',
                      '-Wodr',
                      '-Wpointer-arith',
                      # '-Wshadow',
                      '-Wswitch-default',
                      '-Wswitch-enum',
                      '-flto',
                      language: 'cpp')

cxx = meson.get_compiler('cpp')

eigen = dependency('eigen3')
gl = dependency('gl')
glfw = dependency('glfw3')
libdl = cxx.find_library('dl', required: false)
threads = dependency('threads')

incdir = include_directories('include')

libglad = shared_library('glad',
                         'src/glad.c',
                         include_directories: incdir,
                         dependencies: [gl, libdl],
                         cpp_args: '-DEIGEN_MAX_ALIGN_BYTES=0',
)

saa_srcs = [
    'src/shader.cpp',
    'src/window.cpp',
    'src/drawing/points.cpp',
]

libsaa = shared_library('saa', saa_srcs,
                        include_directories: incdir,
                        link_with: [libglad],
                        dependencies: [eigen, glfw, threads],
)
                        

executable('saa_main', 'src/saa_main.cpp',
           include_directories: incdir,
           link_with: [libsaa],
           dependencies: [eigen],
)
