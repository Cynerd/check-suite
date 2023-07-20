/* SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Karel Kočí <cynerd@email.cz>
 */
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/* Every C source file file is a single suite and need to have unique name
 * defined in this macro before you include check_suite.h.
 */
#define SUITE "example"
#include <check_suite.h>


/* Every test is part of some test case that is automatically included in the
 * suite of this file. Suites can have setup and teardown functions. You can
 * also set timeout for tests in this test-case.
 * The part of the definition is also function body that can contain optionally
 * some code that is executed on check framework initialization. This function
 * gets argument `TCase *tcase` which you can use to further modify the check's
 * test case.
 */
TEST_CASE(simple) {}

/* The basic test looks like this. You need to specify test-case it belongs to
 * and its name. The body of the function contains the test code itself.
 */
TEST(simple, numeric) {
	ck_assert_int_eq(42, 42);
}
END_TEST

/* You can also run multiple tests in loop. This effectively is like if you
 * would add `for` loop to the testing code. The iteration variable is available
 * as `int _i`.
 */
LOOP_TEST(simple, multiple_numeric, 8, 42) {
	ck_assert_int_ne(_i, 0);
}
END_TEST

/* The common usage for LOOP_TEST is to iterate over array of tested data. For
 * that we have ARRAY_TEST.
 * The array name is deduced from the test name (just append `_d`). If you want
 * to use a different array name you can specify it as optional third argument
 * to the `ARRAY_TEST`.
 */
static const struct {
	const char *const a;
	const char *const b;
} str_cmp_d[] = {
	{"", ""},
	{"foo", "foo"},
};
ARRAY_TEST(simple, str_cmp) {
	ck_assert_int_eq(strcmp(_d.a, _d.b), 0);
}


TEST_CASE(signals) {}

/* You can write tests that expect to be terminated with some specific signal.
 */
TEST_RAISE_SIGNAL(signals, signal_abort, SIGABRT) {
	abort();
}
END_TEST

/* You can write tests that result in program termination like this.
 * This is possibly only because check is set to perform fork for every test.
 * This is preferred to separate tests between each other.
 */
TEST_EXIT(signals, signal_exit, 42) {
	exit(42);
}
END_TEST
