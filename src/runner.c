/* SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Karel Kočí <cynerd@email.cz>
 */
#include <check.h>
#include <stdio.h>
#include <stdbool.h>


SRunner *check_suite_runner = NULL;


__attribute__((weak)) void check_suite_setup_cb(void) {
	/* Empty */
}

int main(void) {
	/* There are no tests collected but initialize so we provide valid output */
	if (check_suite_runner == NULL)
		check_suite_runner = srunner_create(NULL);

	/* We have to fork to catch signals. User can change this in the callback */
	srunner_set_fork_status(check_suite_runner, CK_FORK);

	/* Callback that can be overriden by the user */
	check_suite_setup_cb();

	/* Run tests and get the result */
	srunner_run_all(check_suite_runner, CK_ENV);
	int failed = srunner_ntests_failed(check_suite_runner);

	srunner_free(check_suite_runner);
	return (bool)failed;
}
