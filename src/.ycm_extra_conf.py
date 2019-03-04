flags = [
 '-target arm-none-eabi',
 '-mcpu=cortex-m3',
 '-Wall',
 '-Wextra',
 '-Wshadow',
 '-Wimplicit-function-declaration',
 '-Wredundant-decls',
 '-Wmissing-prototypes',
 '-Wundef',
 '-DSTM32F1',
 '-I../libopencm3/include',
 '-I../u8g2/csrc',
 '-isystem', '/usr/lib/arm-none-eabi/include',
 '-std=c99', '-x', 'c'
]

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c',]

def FlagsForFile( filename, **kwargs ):
	return {
		'flags': flags,
		'do_cache': True
	}
