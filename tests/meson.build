test_driver = find_program('test-driver.sh', dirs: meson.current_source_dir())
unittests_env = [
  'CK_TAP_LOG_FILE_NAME=/dev/stdout',
  'CK_VERBOSITY=silent',
]


unittest_libcheck_suite = executable(
  'unittest-libcheck_suite',
  files('example.c'),
  dependencies: [libcheck_suite_dep],
  include_directories: includes,
)
test(
  'unittest-libcheck_suite',
  test_driver,
  args: [unittest_libcheck_suite.full_path()],
  env: unittests_env,
  protocol: 'tap',
)
