cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME runtime_create_object TEST_NAME RuntimeCreateObject SOURCE_FILE create_object.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_bifurcate_abstract TEST_NAME RuntimeBifurcateAbstract SOURCE_FILE bifurcate_abstract.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_bifurcate_input TEST_NAME RuntimeBifurcateInput SOURCE_FILE bifurcate_input.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_bifurcate_universe TEST_NAME RuntimeBifurcateUniverse SOURCE_FILE bifurcate_universe.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_bifurcate_existing_children TEST_NAME RuntimeBifurcateExistingChildren SOURCE_FILE bifurcate_existing_children.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_kill_universe TEST_NAME RuntimeKillUniverse SOURCE_FILE kill_universe.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_kill_object TEST_NAME RuntimeKillObject SOURCE_FILE kill_object.c DEPENDENCIES objects_shared)
tildeathc_add_test(TARGET_NAME runtime_refcount_test TEST_NAME RuntimeRefcountTest SOURCE_FILE refcount_test.c DEPENDENCIES objects_shared)
