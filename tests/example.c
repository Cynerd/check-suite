/* SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Karel Kočí <cynerd@email.cz>
 */
#include <string.h>
#define SUITE "example"
#include <check_suite.h>


TEST_CASE(simple) {}

TEST(simple, numeric) {
	ck_assert_int_eq(42, 42);
}
END_TEST
