tutorial_01_pp = custom_target('Tutorial1.kp',
                               input: 'Tutorial1.kas',
                               output: 'Tutorial1.kp',
                               capture: true,
                               command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

src_missions_tutorials = []

src_missions_tutorials += generator(exe_tools_kas2c,
                                    depends: exe_tools_kas2c,
                                    output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                    arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(tutorial_01_pp)

inc_src_missions_tutorials = include_directories('.')
