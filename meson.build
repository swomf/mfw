project('mfw', 'c', version: '0.2.1', default_options: ['warning_level=2'])

config = configuration_data()
datadir = get_option('datadir')
prefix = get_option('prefix')
config.set('datadir', join_paths(prefix, datadir))

subdir('completions')

executable('mfw', 'mfw.c', install: true)
