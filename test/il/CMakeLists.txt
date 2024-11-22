cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME basic_il_test TEST_NAME BasicILTest SOURCE_FILE basic_il_test.c DEPENDENCIES il_shared)
tildeathc_add_test(TARGET_NAME il_empty_loop TEST_NAME ILEmptyLoop SOURCE_FILE il_empty_loop.c DEPENDENCIES il_shared)
tildeathc_add_test(TARGET_NAME il_bifurcate TEST_NAME ILBifurcate SOURCE_FILE il_bifurcate.c DEPENDENCIES il_shared)
tildeathc_add_test(TARGET_NAME il_die TEST_NAME ILDie SOURCE_FILE il_die.c DEPENDENCIES il_shared)
tildeathc_add_test(TARGET_NAME il_abstract TEST_NAME ILAbstract SOURCE_FILE il_abstract.c DEPENDENCIES il_shared)
