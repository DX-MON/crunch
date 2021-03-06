libCrunchppSrc = [
	'argsParser.cxx', 'stringFuncs.cxx', 'logger.cxx', 'tester.cxx', 'core.cxx'
]
crunchppSrc = ['crunch++.cpp']
crunchppSrcDir = meson.current_source_dir()
crunchppInc = include_directories('.')

config = configuration_data()
config.set('AUTOGEN_HEADER', '/* THIS FILE IS AUTOGENERATED, DO NOT EDIT */')
config.set('VERSION', '@VERSION@')
config.set('COMPILER', cc.get_id())
config.set('COMPILER_VERSION', cc.version())
config.set('TARGET_SYS', target_machine.system())
config.set('TARGET_ARCH', target_machine.cpu())

versionHeaderSrc = configure_file(
	configuration: config,
	input: 'version.hxx.in',
	output: 'version.hxx.in'
)

versionHeader = vcs_tag(
	input: versionHeaderSrc,
	output: 'version.hxx',
	replace_string: '@VERSION@',
	fallback: 'v@0@'.format(meson.project_version())
)

libCrunchpp = shared_library(
	'libcrunch++',
	libCrunchppSrc,
	name_prefix: '',
	cpp_args: libArgs,
	dependencies: [threading],
	include_directories: substrate,
	version: meson.project_version(),
	install: true
)

crunchpp = executable(
	'crunch++',
	crunchppSrc + [versionHeader],
	link_with: [libCrunchpp],
	dependencies: [threading, dl],
	include_directories: substrate,
	install: true,
	install_rpath: libdir
)

pkgconfig.generate(
	libCrunchpp,
	name: 'crunch++',
	filebase: 'crunch++',
	version: meson.project_version(),
	description: 'crunch++ is a unit testing framework for C++'
)

install_headers('crunch++.h')
