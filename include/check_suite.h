/* SPDX-License-Identifier: MIT
 * Copyright 2021-2022, CZ.NIC z.s.p.o. (http://www.nic.cz/)
 * Copyright (c) 2022 Karel Kočí <cynerd@email.cz>
 */
#ifndef _CHECK_SUITE_H_
#define _CHECK_SUITE_H_
#include <check.h>
#include <stdbool.h>
#include <math.h>

extern SRunner *check_suite_runner;


#ifndef SUITE
#error Missing SUITE definition to name suite
#endif
[[gnu::unused]]
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


struct _tcase_generic {
	size_t cnt; /* 0 for u_tcase and number of tcases for u_tcases */
	union {
		TCase *u_tcase;
		struct _tcase_generic **u_tcases;
	};
};


#ifndef DEFAULT_SETUP
#define DEFAULT_SETUP NULL
#endif
#ifndef DEFAULT_TEARDOWN
#define DEFAULT_TEARDOWN NULL
#endif

#define _TEST_CASE(name, setup, teardown, timeout) \
	static void _tcase_##name(TCase *tcase); \
	static struct _tcase_generic *check_suite_tcase_##name() { \
		static struct _tcase_generic tcg = {}; \
		if (tcg.u_tcase == NULL) { \
			tcg.cnt = 0; \
			tcg.u_tcase = tcase_create(#name); \
			tcase_add_checked_fixture(tcg.u_tcase, setup, teardown); \
			if (!isnan(timeout)) \
				tcase_set_timeout(tcg.u_tcase, timeout); \
			check_suite_add_tcase(tcg.u_tcase); \
			_tcase_##name(tcg.u_tcase); \
		} \
		return &tcg; \
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

#define __test_cases_n(...) \
	__test_cases_n_( \
		__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __test_cases_n_( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) \
	N
#define __test_cases_c(x, tcase) tcases[x] = check_suite_tcase_##tcase();
#define __test_cases_15(tcase, ...) __test_cases_c(15, tcase)
#define __test_cases_14(tcase, ...) \
	__test_cases_c(14, tcase) __VA_OPT__(__test_cases_15(__VA_ARGS__))
#define __test_cases_13(tcase, ...) \
	__test_cases_c(13, tcase) __VA_OPT__(__test_cases_14(__VA_ARGS__))
#define __test_cases_12(tcase, ...) \
	__test_cases_c(12, tcase) __VA_OPT__(__test_cases_13(__VA_ARGS__))
#define __test_cases_11(tcase, ...) \
	__test_cases_c(11, tcase) __VA_OPT__(__test_cases_12(__VA_ARGS__))
#define __test_cases_10(tcase, ...) \
	__test_cases_c(10, tcase) __VA_OPT__(__test_cases_11(__VA_ARGS__))
#define __test_cases_09(tcase, ...) \
	__test_cases_c(9, tcase) __VA_OPT__(__test_cases_10(__VA_ARGS__))
#define __test_cases_08(tcase, ...) \
	__test_cases_c(8, tcase) __VA_OPT__(__test_cases_09(__VA_ARGS__))
#define __test_cases_07(tcase, ...) \
	__test_cases_c(7, tcase) __VA_OPT__(__test_cases_08(__VA_ARGS__))
#define __test_cases_06(tcase, ...) \
	__test_cases_c(6, tcase) __VA_OPT__(__test_cases_07(__VA_ARGS__))
#define __test_cases_05(tcase, ...) \
	__test_cases_c(5, tcase) __VA_OPT__(__test_cases_06(__VA_ARGS__))
#define __test_cases_05(tcase, ...) \
	__test_cases_c(5, tcase) __VA_OPT__(__test_cases_06(__VA_ARGS__))
#define __test_cases_04(tcase, ...) \
	__test_cases_c(4, tcase) __VA_OPT__(__test_cases_05(__VA_ARGS__))
#define __test_cases_03(tcase, ...) \
	__test_cases_c(3, tcase) __VA_OPT__(__test_cases_04(__VA_ARGS__))
#define __test_cases_02(tcase, ...) \
	__test_cases_c(2, tcase) __VA_OPT__(__test_cases_03(__VA_ARGS__))
#define __test_cases_01(tcase, ...) \
	__test_cases_c(1, tcase) __VA_OPT__(__test_cases_02(__VA_ARGS__))
#define __test_cases_00(tcase, ...) \
	__test_cases_c(0, tcase) __VA_OPT__(__test_cases_01(__VA_ARGS__))
#define TEST_CASES(name, tcase, ...) \
	static struct _tcase_generic *check_suite_tcase_##name() { \
		static struct _tcase_generic tcg = {}; \
		static struct _tcase_generic *tcases[__test_cases_n(__VA_ARGS__)]; \
		if (tcg.u_tcases == NULL) { \
			tcg.cnt = __test_cases_n(__VA_ARGS__); \
			tcg.u_tcases = tcases; \
			__test_cases_00(tcase __VA_OPT__(, __VA_ARGS__)) \
		} \
		return &tcg; \
	}


#define _TEST(tcase_add, tcase, name, ...) \
	static const TTest *tcase##_##name; \
	static void _test_##tcase##_##name##_register(struct _tcase_generic *tcg) { \
		if (tcg->cnt) { \
			for (size_t i = 0; i < tcg->cnt; i++) \
				_test_##tcase##_##name##_register(tcg->u_tcases[i]); \
		} else { \
			tcase_add(tcg->u_tcase, tcase##_##name __VA_OPT__(, ) __VA_ARGS__); \
		} \
	} \
	__attribute__((constructor)) static void _test_##tcase##_##name() { \
		_test_##tcase##_##name##_register(check_suite_tcase_##tcase()); \
	} \
	START_TEST(tcase##_##name)


#define TEST(tcase, name) _TEST(tcase_add_test, tcase, name)
#define TEST_RAISE_SIGNAL(tcase, name, signal) \
	_TEST(tcase_add_test_raise_signal, tcase, name, signal)
#define TEST_EXIT(tcase, name, exit_value) \
	_TEST(tcase_add_exit_test, tcase, name, exit_value)


#define LOOP_TEST(tcase, name, s, e) \
	_TEST(tcase_add_loop_test, tcase, name, s, e)
#define LOOP_TEST_RAISE_SIGNAL(tcase, name, signal, s, e) \
	_TEST(tcase_add_test_raise_signal, tcase, name, signal, s, e)
#define LOOP_TEST_EXIT(tcase, name, exit_value, s, e) \
	_TEST(tcase_add_exit_test, tcase, name, exit_value, s, e)


#define _ARRAY_TEST(tcase_add, tcase, name, array, ...) \
	static void tcase##_##name##_array_fn( \
		int _i, __typeof__(*array) _d CK_ATTRIBUTE_UNUSED); \
	_TEST(tcase_add, tcase, name __VA_OPT__(, ) __VA_ARGS__, 0, \
		sizeof(array) / sizeof(*(array))) { \
		tcase##_##name##_array_fn(_i, array[_i]); \
	} \
	static void tcase##_##name##_array_fn( \
		int _i, __typeof__(*array) _d CK_ATTRIBUTE_UNUSED)

#define __array_test3(tcase, name, array) \
	_ARRAY_TEST(tcase_add_loop_test, tcase, name, array)
#define __array_test2(tcase, name) \
	__array_test3(tcase, name, tcase##_##name##_d)
#define __array_test_select(_1, _2, _3, X, ...) X
#define ARRAY_TEST(...) \
	__array_test_select(__VA_ARGS__, __array_test3, __array_test2)(__VA_ARGS__)

#define __array_test_raise_signal4(tcase, name, signal, array) \
	_ARRAY_TEST(tcase_add_loop_test_raise_signal, tcase, name, array, signal)
#define __array_test_raise_signal3(tcase, name, signal) \
	__array_test_raise_signal4(tcase, name, signal, name##_d)
#define __array_test_raise_signal_select(_1, _2, _3, _4, X, ...) X
#define ARRAY_TEST_RAISE_SIGNAL(...) \
	__array_test_select(__VA_ARGS__, __array_test_raise_signal4, \
		__array_test_raise_signal3)(__VA_ARGS__)

#define __array_test_exit5(tcase, name, exit_value, array) \
	_ARRAY_TEST(tcase_add_loop_exit_test, tcase, name, array, exit_value)
#define __array_test_exit4(tcase, name, exit_value) \
	__array_test_exit5(tcase, name, exit_value, name##_d)
#define __array_test_exit_select(_1, _2, _3, _4, _5, X, ...) X
#define ARRAY_TEST_EXIT(...) \
	__array_test_exit_select( \
		__VA_ARGS__, __array_test_exit5, __array_test_exit4)(__VA_ARGS__)

#endif
