#include "tests_utils.h"

#include <libbase/runtime_assert.h>

#include <gtest/gtest.h>


std::string getUnitCaseDebugDir() {
    const ::testing::TestInfo* info = ::testing::UnitTest::GetInstance()->current_test_info();

    rassert(info != nullptr, 7382391237189231);

    const char* suite = info->test_suite_name();
    const char* name  = info->name();
    return "debug/unit-tests/" + std::string(suite) + "/" + std::string(name) + "/";
}