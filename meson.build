project('c++ helloworld', 'cpp',
  version : '1.0.0',
  license : 'MIT')
add_global_arguments('-DSOME_TOKEN=value', language : 'cpp')
gtkdep = dependency('gtkmm-3.0')
jackdep = dependency('jack')

incdir = include_directories('include')

executable('eg-amp', ['src/main.cpp', 'src/main_window.cpp'], include_directories : incdir, dependencies : [gtkdep, jackdep])
