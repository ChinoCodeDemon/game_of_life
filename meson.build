project('game_of_life', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

libs = [
  dependency('sdl2')
]

src = [
  'main.cpp',
  'GoL-Simulator.cpp',
  'GridColorTexturer.cpp'
]

executable('game_of_life',
           src,
           install : true,
           dependencies: libs)
