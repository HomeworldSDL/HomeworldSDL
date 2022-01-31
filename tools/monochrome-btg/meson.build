src_tools_monochrome_btg = files('monochrome-btg.c')

exe_tools_monochrome_btg = executable('monochrome-btg',
                                      build_by_default: false,
                                      c_args: c_args,
									  include_directories: inc_src,
									  dependencies: dep_sdl,
                                      sources: src_tools_monochrome_btg)

exe_tools += exe_tools_monochrome_btg
