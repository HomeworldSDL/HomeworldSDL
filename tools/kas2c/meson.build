src_tools_kas2c = files('kas2c.c')

prog_flex = find_program('flex', required: true)
prog_bison = find_program('bison', required: true)

gen_parser = generator(prog_bison,
                      output: ['@BASENAME@.c', '@BASENAME@.h'],
					  arguments: ['-d', '-o', '@OUTPUT0@', '@INPUT@'])

gen_lexer = generator(prog_flex,
                      output: '@BASENAME@.c',
					  arguments: ['-i', '-o', '@OUTPUT@', '@INPUT@'])

gen_output_parser = gen_parser.process('parser.y')
gen_output_lexer = gen_lexer.process('lexer.l')

exe_tools_kas2c = executable('kas2c',
                             src_tools_kas2c,
		                     gen_output_parser,
		                     gen_output_lexer,
		                     implicit_include_directories: true,
		                     dependencies: dep_sdl,
		                     c_args: c_args)
