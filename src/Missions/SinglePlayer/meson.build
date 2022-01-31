mission_01_pp = custom_target('Mission01.kp',
                             input: 'Mission01.kas',
                             output: 'Mission01.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_02_pp = custom_target('Mission02.kp',
                             input: 'Mission02.kas',
                             output: 'Mission02.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_03_pp = custom_target('Mission03.kp',
                             input: 'Mission03.kas',
                             output: 'Mission03.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_04_pp = custom_target('Mission04.kp',
                             input: 'Mission04.kas',
                             output: 'Mission04.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_05_pp = custom_target('Mission05.kp',
                             input: 'Mission05.kas',
                             output: 'Mission05.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_05_OEM_pp = custom_target('Mission05_OEM.kp',
                                 input: 'Mission05_OEM.kas',
                                 output: 'Mission05_OEM.kp',
                                 capture: true,
                                 command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_06_pp = custom_target('Mission06.kp',
                             input: 'Mission06.kas',
                             output: 'Mission06.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_07_pp = custom_target('Mission07.kp',
                             input: 'Mission07.kas',
                             output: 'Mission07.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_08_pp = custom_target('Mission08.kp',
                             input: 'Mission08.kas',
                             output: 'Mission08.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_09_pp = custom_target('Mission09.kp',
                             input: 'Mission09.kas',
                             output: 'Mission09.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_10_pp = custom_target('Mission10.kp',
                             input: 'Mission10.kas',
                             output: 'Mission10.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_11_pp = custom_target('Mission11.kp',
                             input: 'Mission11.kas',
                             output: 'Mission11.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_12_pp = custom_target('Mission12.kp',
                             input: 'Mission12.kas',
                             output: 'Mission12.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_13_pp = custom_target('Mission13.kp',
                             input: 'Mission13.kas',
                             output: 'Mission13.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_14_pp = custom_target('Mission14.kp',
                             input: 'Mission14.kas',
                             output: 'Mission14.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_15_pp = custom_target('Mission15.kp',
                             input: 'Mission15.kas',
                             output: 'Mission15.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

mission_16_pp = custom_target('Mission16.kp',
                             input: 'Mission16.kas',
                             output: 'Mission16.kp',
                             capture: true,
                             command: [meson.get_compiler('c').cmd_array(), '-E', '-x', 'c', '@INPUT@'])

src_missions_single_player = []

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_01_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_02_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_03_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_04_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_05_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_05_OEM_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_06_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_07_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_08_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_09_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_10_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_11_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_12_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_13_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_14_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_15_pp)

src_missions_single_player += generator(exe_tools_kas2c,
                                        depends: exe_tools_kas2c,
                                        output: ['@BASENAME@.c', '@BASENAME@.h', '@BASENAME@.func.c'],
                                        arguments: ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@', '@OUTPUT2@']).process(mission_16_pp)

inc_src_missions_single_player = include_directories('.')
