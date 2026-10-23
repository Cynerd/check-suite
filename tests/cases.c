/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Karel Kočí <cynerd@email.cz>
 */

/* Every C source file file is a single suite and need to have unique name
 * defined in this macro before you include check_suite.h.
 */
#define SUITE "cases"
#include <check_suite.h>


/* The global variable can be set by setup function differently for different
 * test cases.
 */
static int case_var;
static const char *case_string;

static void setup_zero() {
	case_var = 0;
	case_string = "zero";
}
TEST_CASE(zero, setup_zero) {}

static void setup_answer() {
	case_var = 42;
	case_string = "42";
}
TEST_CASE(answer, setup_answer) {}

/* Multiple test cases (up to 16) can be grouped to new test case. Tests defined
 * for this test cases group will be run multiple times per test case.
 */
TEST_CASES(all, zero, answer)

/* This test is part of both zero and answer case. */
TEST(all, cases) {
	const char *expected;
	switch (case_var) {
		case 0:
			expected = "zero";
			break;
		case 42:
			expected = "42";
			break;
		default:
			ck_abort();
	}
	ck_assert_str_eq(expected, case_string);
}


/* The test of all 16 slots follows.
 * If you even need more you have two options:
 * - Either use multiple TEST_CASES as they can be chained
 * - Or request reasonable extension as it is just an arbitrary selected limit.
 */
TEST_CASE(_0) {}
TEST_CASE(_1) {}
TEST_CASE(_2) {}
TEST_CASE(_3) {}
TEST_CASE(_4) {}
TEST_CASE(_5) {}
TEST_CASE(_6) {}
TEST_CASE(_7) {}
TEST_CASE(_8) {}
TEST_CASE(_9) {}
TEST_CASE(_10) {}
TEST_CASE(_11) {}
TEST_CASE(_12) {}
TEST_CASE(_13) {}
TEST_CASE(_14) {}
TEST_CASE(_15) {}
TEST_CASES(upto16, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,
	_14, _15)
TEST_CASE(_16) {}
TEST_CASE(_17) {}
TEST_CASE(_18) {}
TEST_CASE(_19) {}
TEST_CASE(_20) {}
TEST_CASE(_21) {}
TEST_CASE(_22) {}
TEST_CASE(_23) {}
TEST_CASES(upto24, upto16, _16, _17, _18, _19, _20, _21, _22, _23)
TEST(upto24, empty) {}
