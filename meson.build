project(
    'scratch2c',
    'c',
    default_options: [
        'c_std=c99',
        'warning_level=1',
        'default_library=static'
    ]
)

compiler = meson.get_compiler('c')
python = find_program('python3', 'python')

c_args = []
link_args = []
if compiler.get_id() == 'msvc'
    c_args += ['/arch:AVX2', '/D_CRT_SECURE_NO_WARNINGS']
else
    # -march=mavx2 ?
    c_args += ['-march=native']
    link_args += ['-lm']
endif


src = [
    'external/glad/gl.c',
    'src_c/core/array.c',
    'src_c/engine/engine.c',
    'src_c/engine/renderer.c',
    'generated.c'
]

includes = ['include', 'external']

r = run_command(python, 'scripts/install_wraps.py')
if r.returncode() != 0
    error('install_wraps.py script failed.\n', r.stderr())
endif

sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')
sdl2_ttf_dep = dependency('sdl2_ttf')
freetype2 = dependency('freetype2')
opengl_dep = dependency('gl')

if compiler.get_id() == 'msvc'
    sdl2_main_dep = dependency('sdl2main')
    deps = [sdl2_dep, sdl2_main_dep, sdl2_image_dep, sdl2_ttf_dep, freetype2, opengl_dep]
else
    deps = [sdl2_dep, sdl2_image_dep, sdl2_ttf_dep, freetype2, opengl_dep]
endif

executable(
    'project',
    sources: src,
    include_directories: includes,
    c_args: c_args,
    link_args: link_args,
    dependencies: deps
)