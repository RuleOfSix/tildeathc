cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME validate_il_undeclared TEST_NAME ValidateILUndeclared SOURCE_FILE undeclared.c DEPENDENCIES validate_il_shared REGEX "passed")
tildeathc_add_test(TARGET_NAME validate_il_one_declaration TEST_NAME ValidateILOneDeclaration SOURCE_FILE one_declaration.c DEPENDENCIES validate_il_shared)
tildeathc_add_test(TARGET_NAME validate_il_multiple_declarations TEST_NAME ValidateILMultipleDeclarations SOURCE_FILE multiple_declarations.c DEPENDENCIES validate_il_shared)
tildeathc_add_test(TARGET_NAME validate_il_bifurcation TEST_NAME ValidateILBifurcation SOURCE_FILE bifurcation.c DEPENDENCIES validate_il_shared)
tildeathc_add_test(TARGET_NAME validate_il_kill_universe TEST_NAME ValidateILKillUniverse SOURCE_FILE kill_universe.c DEPENDENCIES validate_il_shared REGEX "Error:.*universe.*line [0-9]*.*")
tildeathc_add_test(TARGET_NAME validate_il_kill_universe_when_multiple TEST_NAME ValidateILKillUniverseWhenMultiple SOURCE_FILE kill_universe_when_multiple.c DEPENDENCIES validate_il_shared REGEX "Error:.*universe.*line [0-9]*.*")
tildeathc_add_test(TARGET_NAME validate_il_bifurcate_universe TEST_NAME ValidateILBifurcateUniverse SOURCE_FILE bifurcate_universe.c DEPENDENCIES validate_il_shared REGEX "Error:.*universe.*line [0-9]*.*")
tildeathc_add_test(TARGET_NAME validate_il_overwrite_kill_universe TEST_NAME ValidateILOverwriteKillUniverse SOURCE_FILE overwrite_kill_universe.c DEPENDENCIES validate_il_shared REGEX "Error:.*universe.*line [0-9]*.*")
tildeathc_add_test(TARGET_NAME validate_il_overbifurcate_kill_universe TEST_NAME ValidateILOverbifurcateKillUniverse SOURCE_FILE overbifurcate_kill_universe.c DEPENDENCIES validate_il_shared)
tildeathc_add_test(TARGET_NAME validate_il_this_dot_die TEST_NAME ValidateILThisDotDie SOURCE_FILE this_dot_die.c DEPENDENCIES validate_il_shared)
tildeathc_add_test(TARGET_NAME validate_il_scope_test_1 TEST_NAME ValidateIlScopeTest1 SOURCE_FILE scope_test_1.c DEPENDENCIES validate_il_shared REGEX "error.*line 8.*[uU]nrecognized token.*BUTTERFLY")
tildeathc_add_test(TARGET_NAME validate_il_scope_test_2 TEST_NAME ValidateIlScopeTest2 SOURCE_FILE scope_test_2.c DEPENDENCIES validate_il_shared REGEX "error.*line 1.*[uU]nrecognized token.*BUTTERFLY")
tildeathc_add_test(TARGET_NAME validate_il_scope_test_3 TEST_NAME ValidateIlScopeTest3 SOURCE_FILE scope_test_3.c DEPENDENCIES validate_il_shared REGEX "error.*line 6.*[uU]nrecognized token.*BUTTERFLY")
tildeathc_add_test(TARGET_NAME validate_il_scope_test_4 TEST_NAME ValidateIlScopeTest4 SOURCE_FILE scope_test_4.c DEPENDENCIES validate_il_shared REGEX "error.*line 9.*[uU]nrecognized token.*BUTTERFLY")
tildeathc_add_test(TARGET_NAME validate_il_scope_test_5 TEST_NAME ValidateIlScopeTest5 SOURCE_FILE scope_test_5.c DEPENDENCIES validate_il_shared)
