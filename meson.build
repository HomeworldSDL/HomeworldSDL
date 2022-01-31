project('homeworld',
        'c',
        version: '0.1',
        default_options: ['warning_level=3', 'c_std=c17'])

dep_x11 = dependency('x11')
dep_gl = dependency('gl')
dep_sdl = dependency('sdl2')
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

	if build_machine.cpu_family() == 'x86_64'
		c_args += '-DGENERIC_ETGCALLFUNCTION'
	endif
endif

subdir('tools')
subdir('src')
subdir('tools/biggie')
subdir('tools/monochrome-btg')

if build_machine.cpu_family() == 'x86_64'
	subdir('tools/x86_64')
endif

subdir('HomeworldSDL_big')

alias_target('tools', exe_tools)

python = find_program('python', required: true)

asset_homeworldsdl_big = custom_target('HomeworldSDL.big',
                                       output: 'HomeworldSDL.big',
                                       command: [python, script_assets_homeworldsdl_big_generate, exe_tools_biggie, '@OUTPUT@'],
                                       depends: [exe_tools_biggie])

alias_target('assets', asset_homeworldsdl_big)

executable(meson.project_name(),
           src,
           include_directories: inc_src,
           link_with: [lib_jpg, lib_sdl],
           dependencies: [dep_x11, dep_gl, dep_sdl, dep_math],
           c_args: c_args,
           install: true)
