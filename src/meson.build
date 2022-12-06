libcheck_suite_sources = files(
  'runner.c',
)

libcheck_suite = library('check-suite', libcheck_suite_sources,
  version: '0.0.0',
  dependencies: [check],
  include_directories: includes,
  link_args: '-Wl,--version-script=' + join_paths(meson.current_source_dir(), 'libcheck_suite.version'),
  install: true
)
install_headers(libcheck_suite_headers)

libcheck_suite_dep = declare_dependency(
  include_directories: includes,
  dependencies: [check],
  link_with: libcheck_suite
)


pkg_mod = import('pkgconfig')
pkg_mod.generate(libcheck_suite,
  description: 'Unit testing framework for C with implicit suite registration'
)
