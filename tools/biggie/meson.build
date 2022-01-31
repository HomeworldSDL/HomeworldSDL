src_tools_biggie = files('main.c',
                         'options.c',
						 '../../src/Game/BigFile.c')

exe_tools_biggie = executable('biggie',
                              build_by_default: false,
                              c_args: c_base_args,
							  include_directories: inc_src,
							  link_with: [lib_crc, lib_lzss],
							  dependencies: dep_sdl,
                              sources: src_tools_biggie)

exe_tools += exe_tools_biggie
