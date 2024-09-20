project('homeworld',
        'c',
        version: '1.2.0',
        default_options: ['warning_level=3', 'c_std=gnu11', 'b_sanitize=address,undefined'])

if get_option('b_sanitize') != 'none'
	warning('Building with memory sanitizers on. If linking fails, try disabling them with `meson configure -Db_sanitize=none`.')
endif

dep_sdl = dependency('sdl2')

base_deps = [ ]

if host_machine.system() != 'emscripten'
	base_deps += [
		dependency('gl'),
		dep_sdl,
		meson.get_compiler('c').find_library('m', required: false),
	]
endif

c_base_args = [
	'-DHAVE_CONFIG_H',
	'-D_REENTRANT',

	# Treat all warnings as errors. We want all new code to be as safe & clean as possible
	'-Werror',

	# Ignore existing issues in the codebase
	# TODO: re-enable them one by one to fix them
	'-Wno-address', # 2 occurrences
	'-Wno-cast-function-type',
	'-Wno-discarded-qualifiers',
	'-Wno-enum-conversion', # 1 occurrence
	'-Wno-empty-body',
	'-Wno-format-security',
	'-Wno-format-zero-length',
	'-Wno-format',
	'-Wno-implicit-fallthrough',
	'-Wno-implicit-function-declaration',
	'-Wno-incompatible-pointer-types',
	'-Wno-int-conversion', # 6 occurrences
	'-Wno-int-in-bool-context',
	'-Wno-int-to-pointer-cast',
	'-Wno-logical-not-parentheses',
	'-Wno-maybe-uninitialized',
	'-Wno-missing-field-initializers', # 1 occurrence
	'-Wno-misleading-indentation', # 2 occurrences
	'-Wno-multistatement-macros',
	'-Wno-parentheses',
	'-Wno-pedantic',
	'-Wno-pointer-arith',
	'-Wno-pointer-to-int-cast',
	'-Wno-restrict',
	'-Wno-sign-compare',
	'-Wno-stringop-overflow',
	'-Wno-stringop-overread',
	'-Wno-type-limits',
	'-Wno-unused-but-set-parameter',
	'-Wno-unused-but-set-variable',
	'-Wno-unused-function',
	'-Wno-unused-parameter',
	'-Wno-unused-value',
	'-Wno-unused-variable',

	# TODO: Windows mingw build issues
	'-Wno-array-bounds',
	'-Wno-deprecated-declarations',
	'-Wno-stringop-truncation',

	# TODO: Linux gcc 13.2.0
	'-Wno-unused-result',
]

if host_machine.system() == 'emscripten'
	c_base_args += [
		'-DHW_GAME_DEMO',

		# TODO: fix warnings
		'-Wno-unknown-warning-option',
		'-Wno-constant-logical-operand',
		'-Wno-deprecated-non-prototype',
		'-Wno-pointer-sign',
		'-Wno-invalid-source-encoding',
		'-Wno-uninitialized',
		'-Wno-sometimes-uninitialized',
		'-Wno-non-literal-null-conversion', # 1 occurrence
	]
else
	c_base_args += ['-DHW_GAME_HOMEWORLD']
endif

if host_machine.cpu_family() != 'x86'
	c_base_args += ['-DGENERIC_ETGCALLFUNCTION']
endif

if host_machine.cpu_family() == 'x86_64'
	c_base_args += ['-D_X86_64', '-D_X86_64_FIX_ME']
elif host_machine.cpu_family() == 'x86'
	c_base_args += ['-malign-double', '-D_X86', '-msse']
elif host_machine.cpu_family() in ['arm', 'aarch64']
	c_base_args += ['-DARM']
endif

if host_machine.system() == 'linux'
	base_deps += [dependency('x11')]
	c_base_args += ['-D_LINUX_FIX_ME', '-D_GNU_SOURCE']
elif host_machine.system() == 'darwin'
	base_deps += [dependency('x11')]
	c_base_args += ['-D__APPLE___FIX_ANIM', '-D__APPLE___FIX_LAN', '-D__APPLE___FIX_MISC']

	if host_machine.cpu_family() == 'x86_64'
		c_base_args += ['-D__APPLE___64', '-D__APPLE___FIX_64']
	elif host_machine.cpu_family() == 'x86'
		c_base_args += ['-D__APPLE___86', '-D__APPLE___FIX_86']
	endif
elif host_machine.system() == 'windows'
	c_base_args += ['-D_LINUX_FIX_ME', '-D_GNU_SOURCE', '-D_WIN32', '-DWIN32', '-DWINDOWS']
elif host_machine.system() == 'emscripten'
	base_deps += [subproject('demo-assets').get_variable('demo_assets_dep')]
	c_base_args += ['-D_LINUX_FIX_ME', '-D_GNU_SOURCE']
endif

if get_option('debug') and get_option('debug_asserts')
	c_args = c_base_args + '-DHW_BUILD_FOR_DEBUGGING'
else
	c_args = c_base_args + '-DHW_BUILD_FOR_DISTRIBUTION'
endif

if get_option('movies')
	warning('Building with video cutscenes support. This requires a handful of extra dependencies (usually provided by FFMPEG). Disable with `meson configure -Dmovies=false`')

	base_deps += [
		dependency('libavutil'),
		dependency('libswscale'),
		dependency('libavcodec'),
		dependency('libavformat')
	]
	c_args += ['-DHW_ENABLE_MOVIES']
endif

if host_machine.system() == 'emscripten'
	emscripten_shared_args = [
		'--use-port=sdl2',
		'-Wno-int-conversion',
		'-pthread',
		'-gsource-map',
	]
	
	emscripten_compile_args = emscripten_shared_args
	emscripten_link_args = emscripten_shared_args + [
		'-lidbfs.js',
		'-sASSERTIONS=0',
		'-sWASM=1',
		'-sUSE_PTHREADS=1',
		'-sPTHREAD_POOL_SIZE=4',
		'-sLEGACY_GL_EMULATION=1',
		'-s', 'TOTAL_MEMORY=50331648',
		'-s', 'ALLOW_MEMORY_GROWTH=1',
		'--use-preload-plugins',

		'--preload-file', '../../subprojects/demo-assets-1.05/assets@/',
		'-o', 'homeworld.html',
	]

    add_project_arguments(emscripten_compile_args, language: ['c', 'cpp'])
    add_project_link_arguments(emscripten_link_args, language: ['c', 'cpp'])
endif


subdir('tools')
subdir('src')
subdir('tools/biggie')
subdir('tools/monochrome-btg')

if build_machine.cpu_family() == 'x86_64'
	subdir('tools/x86_64')
endif

if host_machine.system() == 'emscripten'
	fs = import('fs')
	copy = fs.copyfile('wasm/index.html')
endif

if host_machine.system() == 'emscripten'
	executable(meson.project_name(),
			src,
			include_directories: inc_src,
			link_with: [lib_sdl],
			c_args: c_args,
			install: true)
else
	executable(meson.project_name(),
			src,
			include_directories: inc_src,
			link_with: [lib_sdl],
			dependencies: base_deps,
			c_args: c_args,
			install: true)
endif