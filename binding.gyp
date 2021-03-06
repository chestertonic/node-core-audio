{
  'targets': [
		{
			'target_name': 'NodeCoreAudio',
			'sources': [
				'NodeCoreAudio/AudioEngine.cpp',
				'NodeCoreAudio/NodeCoreAudio.cpp',
				'NodeCoreAudio/WindowFunction.cpp',
			],
			'include_dirs': [
        '<!(node -e "require(\'nan\')")',
				'<(module_root_dir)/NodeCoreAudio/'
			],
			'conditions' : [
				[
					'OS=="linux"', {
						'include_dirs': [
							'/usr/local/include/'
						],
						'libraries' : [
							'/usr/local/lib/libportaudio.so',
							'/usr/local/lib/libfftw3.so'
						],
						'cflags!': [ '-fno-exceptions' ],
						'cflags_cc!': [ '-fno-exceptions' ],
						'cflags_cc': [ '-std=c++0x' ]
					}
				],
				[
					'OS=="mac"', {
						'include_dirs': [
							'/usr/local/include/'
						],
						'libraries' : [
							'/Library/Frameworks/CoreAudio.framework',
							'/Library/Frameworks/AudioToolbox.framework',
							'/Library/Frameworks/AudioUnit.framework',
							'/Library/Frameworks/CoreServices.framework',
							'/Library/Frameworks/Carbon.framework',
							'/usr/local/lib/libportaudio.dylib',
							'/usr/local/lib/libfftw3.dylib'
						],
						'cflags!': [ '-fno-exceptions' ],
						'cflags_cc!': [ '-fno-exceptions' ],
						'cflags_cc': [ '-std=c++0x' ]
					}
				],
				[
					'OS=="win"', {
						"conditions": [
							[
								'target_arch=="ia32"', {
                  'include_dirs': ["D:\\lib\\fftw\\x32"],
									'libraries' : [
										'<(module_root_dir)/gyp/lib/win32/portaudio_x86.lib',
                    '<(module_root_dir)/gyp/lib/win32/libfftw3-3.lib'
									],
									'copies': [
										{
										    'destination': '<(module_root_dir)/build/Release/',
										    'files': [
										 		'<(module_root_dir)/gyp/lib/win32/portaudio_x86.dll',
										 		'<(module_root_dir)/gyp/lib/win32/portaudio_x86.lib',
												'<(module_root_dir)/gyp/lib/win32/libfftw3-3.dll',
                        '<(module_root_dir)/gyp/lib/win32/libfftw3-3.lib'
										  ]
										}
									]
								}
							],
							[
								'target_arch=="x64"', {
                  'include_dirs': ["D:\\lib\\fftw\\x64"],
									'libraries' : [
										'<(module_root_dir)/gyp/lib/win64/portaudio_x64.lib',
                    '<(module_root_dir)/gyp/lib/win64/libfftw3-3.lib'
									],
									'copies': [
										{
										    'destination': '<(module_root_dir)/build/Release/',
										    'files': [
										 		'<(module_root_dir)/gyp/lib/win64/portaudio_x64.dll',
										 		'<(module_root_dir)/gyp/lib/win64/portaudio_x64.lib',
												'<(module_root_dir)/gyp/lib/win64/libfftw3-3.dll',
                        '<(module_root_dir)/gyp/lib/win64/libfftw3-3.lib'
										  ]
										}
									]
								}
							]
						],

						'include_dirs' : [ "gyp/include",
                               "D:\\lib\\portaudio\\include"
             ]
					}
				]
			]
		}
	]
}
