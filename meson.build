project('homeworld',
        'c',
        version: '1.2.0',
        default_options: ['warning_level=3', 'c_std=c17', 'b_sanitize=address,undefined'])

dep_x11 = dependency('x11')
dep_gl = dependency('gl')
dep_sdl = dependency('sdl2')
dep_sdl_img = dependency('SDL2_image')
dep_math = meson.get_compiler('c').find_library('m', required: false)

c_base_args = ['-DHW_GAME_HOMEWORLD']
c_base_args += ['-DHAVE_CONFIG_H', '-D_REENTRANT']
#c_base_args += '-Wno-unused-variable'
#c_base_args += '-Wno-unused-parameter'
c_base_args += '-w'

if build_machine.cpu_family() == 'x86_64'
	c_base_args += ['-D_X86_64', '-D_X86_64_FIX_ME', '-DGENERIC_ETGCALLFUNCTION']
elif build_machine.cpu_family() == 'x86'
	c_base_args += ['-malign-double', '-D_X86', '-msse']
elif build_machine.cpu_family() in ['arm', 'aarch64']
	c_base_args += ['-DARM', '-DGENERIC_ETGCALLFUNCTION']
endif

if build_machine.system() == 'linux'
	c_base_args += ['-D_LINUX_FIX_ME', '-D_GNU_SOURCE']
elif build_machine.system() == 'darwin'
	c_base_args += ['-D__APPLE___FIX_ANIM', '-D__APPLE___FIX_LAN', '-D__APPLE___FIX_MISC', '-DGENERIC_ETGCALLFUNCTION']

	if build_machine.cpu_family() == 'x86_64'
		c_base_args += ['-D__APPLE___64', '-D__APPLE___FIX_64']
	elif build_machine.cpu_family() == 'x86'
		c_base_args += ['-D__APPLE___86', '-D__APPLE___FIX_86']
	endif
elif build_machine.system() == 'windows'

endif

if get_option('debug')
	c_args = c_base_args + '-DHW_BUILD_FOR_DEBUGGING'
else
	c_args = c_base_args + '-DHW_BUILD_FOR_DISTRIBUTION'
endif

subdir('tools')
subdir('src')
subdir('tools/biggie')
subdir('tools/monochrome-btg')

if build_machine.cpu_family() == 'x86_64'
	subdir('tools/x86_64')
endif

executable(meson.project_name(),
           src,
           include_directories: inc_src,
           link_with: [lib_sdl],
           dependencies: [dep_x11, dep_gl, dep_sdl, dep_math, dep_sdl_img],
           c_args: c_args,
           install: true)
