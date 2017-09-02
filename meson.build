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

glad_incdir = include_directories('include/saa')
incdir = include_directories('include')

saa_srcs = [
    'src/glad.c',
    'src/shader.cpp',
    'src/window.cpp',
    'src/drawing/points.cpp',
]
libsaa = shared_library('saa', saa_srcs,
                        include_directories: [incdir, glad_incdir],
                        dependencies: [eigen, glfw, libdl, threads],
                        install: true,
                        cpp_args: '-DEIGEN_MAX_ALIGN_BYTES=0',
)
                        

executable('saa_main', 'src/saa_main.cpp',
           include_directories: incdir,
           link_with: [libsaa],
           dependencies: [eigen],
           install: true,
)

shaders = [
    'src/shaders/points.vs.glsl',
    'src/shaders/points.fs.glsl',
]

install_subdir('include/saa', install_dir: 'include')
install_data(shaders, install_dir: 'share/saa/shaders')
