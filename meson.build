project('saa', 'c', 'cpp',
        default_options: ['c_std=c11', 'cpp_std=c++14'])

add_project_arguments('-Wall',
                      '-Wextra',
                      '-Werror',
                      '-pedantic',
                      '-pedantic-errors',
                      '-Wmissing-declarations',
                      '-Wmissing-include-dirs',
                      '-Wodr',
                      '-Wpointer-arith',
                      '-Wswitch-default',
                      '-Wswitch-enum',
                      '-flto',
                      '-Wno-error=unused-but-set-variable',
                      '-Wno-error=unused-function',
                      '-Wno-error=unused-parameter',
                      '-Wno-error=unused-variable',
                      language: 'cpp')

cxx = meson.get_compiler('cpp')

eigen = dependency('eigen3')
gl = dependency('gl')
glfw = dependency('glfw3')
libdl = cxx.find_library('dl', required: false)
threads = dependency('threads')
x11 = dependency('x11')

glad_incdir = include_directories('include/saa')
incdir = include_directories('include')

saa_conf = configuration_data()
saa_conf.set('SAA_SHADER_PATH', get_option('prefix') + '/' + get_option('datadir') + '/saa/shaders')
configure_file(
    input: 'src/config.hpp.in',
    output: 'config.hpp',
    configuration: saa_conf)
conf_incdir = include_directories('.')

saa_srcs = [
    'src/camera.cpp',
    'src/drawing/circles.cpp',
    'src/drawing/points.cpp',
    'src/glad.c',
    'src/math.cpp',
    'src/projection.cpp',
    'src/shader.cpp',
    'src/window.cpp',
]
libsaa = shared_library('saa', saa_srcs,
                        include_directories: [incdir, glad_incdir, conf_incdir],
                        dependencies: [eigen, gl, glfw, libdl, threads],
                        install: true,
                        cpp_args: '-DEIGEN_MAX_ALIGN_BYTES=0',
)
                        

executable('saa_main', 'src/saa_main.cpp',
           include_directories: incdir,
           link_with: [libsaa],
           dependencies: [eigen, gl],
           install: true,
           cpp_args: '-DEIGEN_MAX_ALIGN_BYTES=0',
)

shaders = [
    'src/shaders/circles.vs.glsl',
    'src/shaders/circles.fs.glsl',
    'src/shaders/points.vs.glsl',
    'src/shaders/points.fs.glsl',
]


install_subdir('include/saa', install_dir: 'include')
install_data(shaders, install_dir: 'share/saa/shaders')
