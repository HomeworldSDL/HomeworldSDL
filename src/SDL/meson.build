src_sdl = files('avi.c',
                'font.c',
                'main.c',
                'mainrgn.c',
                'mixfft.c',
                'mouse.c',
                'NetworkInterface.c',
                'prim2d.c',
                'prim3d.c',
                'Queue.c',
                'render.c',
                'rglu.c',
                'rinit.c',
                'screenshot.c',
                'smixer.c',
                'soundlow.c',
                'sstream.c',
                'texreg.c',
                'TimeoutTimer.c',
                'Titan.c',
                'TitanInterfaceC.c',
                'utility.c')

src_sdl += files('fqcodec.c',
                 'fqeffect.c',
                 'fquant.c',
                 'dct.c')

lib_sdl = static_library('hw_sdl',
                         src_sdl,
                         c_args: c_args,
                         dependencies: [dep_sdl],
                         include_directories: ['../Game', '../Ships', '../ThirdParty/CRC', '../ThirdParty/JPG'])

inc_src_sdl = include_directories('.')
