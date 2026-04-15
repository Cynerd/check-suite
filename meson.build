project(
  'check-suite',
  'c',
  version: run_command(find_program('bash'), './.version.sh', check: true).stdout().strip(),
  license : 'MIT',
  default_options: 'c_std=c17',
  meson_version: '>=0.60.0',
)
pkg_mod = import('pkgconfig')

add_project_arguments('-D_GNU_SOURCE', language: 'c')
cc = meson.get_compiler('c')

check = dependency('check', version: '>=0.11')

subdir('include')
subdir('src')

meson.override_dependency(meson.project_name(), libcheck_suite_dep)


test_buildtypes = ['debug', 'debugoptimized']
tests_opt = get_option('tests')
if (
  not meson.is_subproject()
  and (
    tests_opt == 'enabled'
    or (tests_opt == 'auto' and get_option('buildtype') in test_buildtypes)
  )
)
  subdir('tests')
endif
