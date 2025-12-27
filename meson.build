project('pingpong', 'cpp')

subdir('PingPong')

x11_dep = dependency('x11')

if get_option('build_type') == 'debug'
    add_global_arguments(['-g'], language: 'cpp')
elif get_option('build_type') == 'release'
    add_global_arguments(['-O2', '-flto', '-Werror', '-Wall'], language: 'cpp')
    add_global_link_arguments(['-flto'], language: 'cpp')
endif

executable('pingpong', sources,
    cpp_args: ['-std=c++23'],
    dependencies: [x11_dep]
)