#pragma once

/* Uncomment if you want to run the unit tests */

#define RUN_TESTS

void app_main();

#ifndef RUN_TESTS
	#define APP_MAIN() int \
		main() { \
			app_main(); \
			return 0; \
		}
#else
	#define APP_MAIN()
	#define CATCH_CONFIG_MAIN // Run the unit tests instead
	#include "catch.hpp"
#endif