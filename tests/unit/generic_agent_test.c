#include "test.h"

#include "generic_agent.h"
#include "env_context.h"

void test_load_masterfiles(void **state)
{
    EvalContext *ctx = EvalContextNew();
    GenericAgentConfig *config = GenericAgentConfigNewDefault(AGENT_TYPE_COMMON);

    MINUSF = true;
    GenericAgentConfigSetInputFile(config, "../../masterfiles/promises.cf");

    Policy *masterfiles = GenericAgentLoadPolicy(ctx, config);
    assert_true(masterfiles);

    PolicyDestroy(masterfiles);
    GenericAgentConfigDestroy(config);
    EvalContextDestroy(ctx);
}

void test_resolve_absolute_input_path(void **state)
{
    assert_string_equal("/abs/aux.cf", GenericAgentResolveInputPath("/abs/aux.cf", NULL));
}

void test_resolve_input_path(void **state)
{
    MINUSF = false;

    assert_string_equal("/workdir/inputs/aux.cf", GenericAgentResolveInputPath("aux.cf", NULL));
    assert_string_equal("./aux.cf", GenericAgentResolveInputPath("./aux.cf", NULL));
}

void test_resolve_input_path_specified(void **state)
{
    MINUSF = true;

    assert_string_equal("/workdir/inputs/promises.cf", GenericAgentResolveInputPath("promises.cf", "promises.cf"));
    assert_string_equal("/workdir/inputs/aux.cf", GenericAgentResolveInputPath("aux.cf", "promises.cf"));
    assert_string_equal("./aux.cf", GenericAgentResolveInputPath("./aux.cf", "promises.cf"));

    assert_string_equal("./rel/promises.cf", GenericAgentResolveInputPath("./rel/promises.cf", "./rel/promises.cf"));
    assert_string_equal("./rel/aux.cf", GenericAgentResolveInputPath("aux.cf", "./rel/promises.cf"));
    assert_string_equal("./rel/./rel/aux.cf", GenericAgentResolveInputPath("./rel/aux.cf", "./rel/promises.cf"));
}

int main()
{
    strcpy(CFWORKDIR, "/workdir");

    PRINT_TEST_BANNER();
    const UnitTest tests[] =
    {
        unit_test(test_load_masterfiles),
        unit_test(test_resolve_absolute_input_path),
        unit_test(test_resolve_input_path),
        unit_test(test_resolve_input_path_specified)
    };

    return run_tests(tests);
}
