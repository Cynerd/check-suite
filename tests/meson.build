valgrind = find_program('valgrind', required: false)
if valgrind.found()
  bash = find_program('bash')
  valgrind_wrapper = [bash, meson.current_source_dir() + '/valgrind-wrapper.sh', valgrind]
  add_test_setup(
    'memcheck',
    exe_wrapper: valgrind_wrapper + 'memcheck',
    timeout_multiplier: 3,
  )
  add_test_setup(
    'helgrind',
    exe_wrapper: valgrind_wrapper + 'helgrind',
    timeout_multiplier: 3,
  )
  add_test_setup(
    'drd',
    exe_wrapper: valgrind_wrapper + 'drd',
    timeout_multiplier: 4,
  )
endif

unittests_env = [
  'CK_TAP_LOG_FILE_NAME=/dev/stdout',
  'CK_VERBOSITY=silent',
]

tests = [
  'example',
  'empty',
]

foreach name : tests
  test(
    name,
    executable(
      'test-' + name,
      files(name + '.c'),
      dependencies: [libcheck_suite_dep],
      include_directories: includes,
    ),
    env: unittests_env,
    protocol: 'tap',
  )
endforeach
