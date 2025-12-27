project('pingpong', 'cpp')

subdir('PingPong')

x11_dep = dependency('x11')

if get_option('build_type') == 'debug'
    add_global_arguments(['-g'], language: 'cpp')
elif get_option('build_type') == ''
    add_global_arguments(['-O2'], language: 'cpp')
endif

executable('pingpong', sources,
    cpp_args: ['-std=c++23'],
    dependencies: [x11_dep]
)