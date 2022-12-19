project('check-suite', 'c',
  version: '1.0.0',
  license : 'MIT',
  default_options: 'c_std=c17',
)
pkg = import('pkgconfig')

add_project_arguments('-D_GNU_SOURCE', language: 'c')
cc = meson.get_compiler('c')

check = dependency('check', version: '>=0.11')

subdir('include')
subdir('src')

meson.override_dependency(meson.project_name(), libcheck_suite_dep)


test_buildtypes = ['debug', 'debugoptimized']
opt_tests = get_option('tests')
if not meson.is_subproject() and (opt_tests.enabled() or (opt_tests.auto() and get_option('buildtype') in test_buildtypes))
  subdir('tests')
endif


cppcheck = find_program('cppcheck', required: false)
if cppcheck.found()
  run_target('cppcheck', command : [cppcheck, '--project=' + join_paths(meson.build_root(), 'compile_commands.json')])
endif

flawfinder = find_program('flawfinder', required: false)
if flawfinder.found()
  run_target('flawfinder', command : [flawfinder, '--error-level=5', meson.project_source_root()])
endif
