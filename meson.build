project(
    'telefonkonyv3d',
    'cpp',
    default_options: [
        'c_std=c23',
        'cpp_std=c++23',
    ],
)

cpp = meson.get_compiler('cpp')
add_project_arguments('-DMEMTRACE', language: 'cpp')

sdl = dependency('SDL2', required: true)
sdl_image = dependency('SDL2_image', required: true)
sdl_gfx = dependency('SDL2_gfx', required: true)

includes = include_directories('./')

sources = files(
    'gamelogic.cpp',
    'graphics.cpp',
    'matrix.cpp',
    'raycast.cpp',
    'telefonkonyv.cpp',
    'texture.cpp',
    'vector2.cpp',
    'memtrace.cpp',
)

executable(
    'telefonkonyv3d',
    sources,
    dependencies: [sdl, sdl_image, sdl_gfx],
    include_directories: includes,
)