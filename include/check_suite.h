/* SPDX-License-Identifier: MIT
 * Copyright 2021-2022, CZ.NIC z.s.p.o. (http://www.nic.cz/)
 * Copyright (c) 2022 Karel Kočí <cynerd@email.cz>
 */
#ifndef _CHECK_SUITE_H_
#define _CHECK_SUITE_H_
#include <check.h>
#include <math.h>

extern SRunner *check_suite_runner;


#ifndef SUITE
#error Missing SUITE definition to name suite
#endif
static void check_suite_add_tcase(TCase *tcase) {
	static Suite *suite = NULL;
	if (suite == NULL) {
		suite = suite_create(SUITE);
		if (check_suite_runner == NULL)
			check_suite_runner = srunner_create(NULL);
		srunner_add_suite(check_suite_runner, suite);
	}
	suite_add_tcase(suite, tcase);
}


#ifndef DEFAULT_SETUP
#define DEFAULT_SETUP NULL
#endif
#ifndef DEFAULT_TEARDOWN
#define DEFAULT_TEARDOWN NULL
#endif

#define _TEST_CASE(name, setup, teardown, timeout) \
	static void _tcase_##name(TCase *tcase); \
	static TCase *check_suite_tcase_##name() { \
		static TCase *tcase = NULL; \
		if (tcase == NULL) { \
			tcase = tcase_create(#name); \
			tcase_add_checked_fixture(tcase, setup, teardown); \
			if (!isnan(timeout)) \
				tcase_set_timeout(tcase, timeout); \
			check_suite_add_tcase(tcase); \
			_tcase_##name(tcase); \
		} \
		return tcase; \
	} \
	static void _tcase_##name(TCase *tcase)
#define __test_case1(name) \
	_TEST_CASE(name, DEFAULT_SETUP, DEFAULT_TEARDOWN, NAN)
#define __test_case2(name, setup) _TEST_CASE(name, setup, DEFAULT_TEARDOWN, NAN)
#define __test_case3(name, setup, teardown) \
	_TEST_CASE(name, setup, teardown, NAN)
#define __test_case_select(_1, _2, _3, _4, X, ...) X
#define TEST_CASE(...) \
	__test_case_select(__VA_ARGS__, _TEST_CASE, __test_case3, __test_case2, \
		__test_case1)(__VA_ARGS__)


#define _TEST(tcase_add, tcase, name, ...) \
	static const TTest *name; \
	__attribute__((constructor)) static void _test_##name() { \
		tcase_add(check_suite_tcase_##tcase(), __VA_ARGS__); \
	} \
	START_TEST(name)


#define TEST(tcase, name) _TEST(tcase_add_test, tcase, name, name)
#define TEST_RAISE_SIGNAL(tcase, name, signal) \
	_TEST(tcase_add_test_raise_signal, tcase, name, name, signal)
#define TEST_EXIT(tcase, name, exit_value) \
	_TEST(tcase_add_exit_test, tcase, name, name, exit_value)


#define LOOP_TEST(tcase, name, s, e) \
	_TEST(tcase_add_loop_test, tcase, name, name, s, e)
#define LOOP_TEST_RAISE_SIGNAL(tcase, name, signal, s, e) \
	_TEST(tcase_add_test_raise_signal, tcase, name, name, signal, s, e)
#define LOOP_TEST_EXIT(tcase, name, exit_value, s, e) \
	_TEST(tcase_add_exit_test, tcase, name, name, exit_value, s, e)


#define _ARRAY_TEST(tcase_add, tcase, name, array, ...) \
	static void name##_array_fn( \
		int _i, __typeof__(*array) _d CK_ATTRIBUTE_UNUSED); \
	_TEST(tcase_add, tcase, name, __VA_ARGS__, 0, \
		sizeof(array) / sizeof(*(array))) { \
		name##_array_fn(_i, array[_i]); \
	} \
	static void name##_array_fn(int _i, __typeof__(*array) _d CK_ATTRIBUTE_UNUSED)

#define __array_test3(tcase, name, array) \
	_ARRAY_TEST(tcase_add_loop_test, tcase, name, array, name)
#define __array_test2(tcase, name) __array_test3(tcase, name, name##_d)
#define __array_test_select(_1, _2, _3, X, ...) X
#define ARRAY_TEST(...) \
	__array_test_select(__VA_ARGS__, __array_test3, __array_test2)(__VA_ARGS__)

#define __array_test_raise_signal4(tcase, name, signal, array) \
	_ARRAY_TEST(tcase_add_loop_test_raise_signal, tcase, name, array, name, signal)
#define __array_test_raise_signal3(tcase, name, signal) \
	__array_test_raise_signal4(tcase, name, signal, name##_d)
#define __array_test_raise_signal_select(_1, _2, _3, _4, X, ...) X
#define ARRAY_TEST_RAISE_SIGNAL(...) \
	__array_test_select(__VA_ARGS__, __array_test_raise_signal4, \
		__array_test_raise_signal3)(__VA_ARGS__)

#define __array_test_exit5(tcase, name, exit_value, array) \
	_ARRAY_TEST(tcase_add_loop_exit_test, tcase, name, array, name, exit_value)
#define __array_test_exit4(tcase, name, exit_value) \
	__array_test_exit5(tcase, name, exit_value, name##_d)
#define __array_test_exit_select(_1, _2, _3, _4, _5, X, ...) X
#define ARRAY_TEST_EXIT(...) \
	__array_test_exit_select( \
		__VA_ARGS__, __array_test_exit5, __array_test_exit4)(__VA_ARGS__)

#endif
