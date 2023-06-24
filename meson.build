project('software-renderer', 'cpp',
    version: '0.0.1',
    default_options: [
        'cpp_std=c++20',   # Use C++20
        'warning_level=2', # Enable extra warnings
        'werror=true',     # Treat warnings as errors
        'default_library=static',
    ])

# Subprojects
fmt_wrap = subproject('fmt',
    default_options: [
        'default_library=static',
        'warning_level=0', # Disable extra warnings
        'werror=false',    # Don't treat warnings as errors
    ])
spdlog_wrap = subproject('spdlog',
    default_options: [
        'tests=false',
        'default_library=static',
        'compile_library=true',
        'external_fmt=enabled',
        'warning_level=0', # Disable extra warnings
        'werror=false',    # Don't treat warnings as errors
    ])
sdl2_wrap = subproject('sdl2',
    default_options: [
        'test=false',
        'warning_level=0', # Disable extra warnings
        'werror=false',    # Don't treat warnings as errors
    ])
eigen_wrap = subproject('eigen')

# Executable
executable('software-renderer',
    dependencies: [
        spdlog_wrap.get_variable('spdlog_dep'),
        eigen_wrap.get_variable('eigen_dep'),
        sdl2_wrap.get_variable('sdl2_dep'),
    ],
    include_directories: [
        'src/Renderer',
    ],
    cpp_pch: 'src/Renderer/pch.h',
    sources: [
        'src/Renderer/Entity.cpp',
        'src/Renderer/Entrypoint.cpp',
        'src/Renderer/Model.cpp',
        'src/Renderer/Renderer.cpp',
        'src/Renderer/Simulation.cpp',
        'src/Renderer/Utilities.cpp',
    ],
    win_subsystem: 'windows')