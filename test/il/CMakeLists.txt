cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME basic_il_test TEST_NAME BasicILTest SOURCE_FILE basic_il_test.c DEPENDENCIES il)
tildeathc_add_test(TARGET_NAME il_empty_loop TEST_NAME ILEmptyLoop SOURCE_FILE il_empty_loop.c DEPENDENCIES il)
