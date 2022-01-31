src_third_party_lzss = files('BitIO.c', 'LZSS.c')
inc_src_third_party_lzss = include_directories('.')

lib_lzss = static_library('hw_lzss',
                          src_third_party_lzss,
                          c_args: c_args)
