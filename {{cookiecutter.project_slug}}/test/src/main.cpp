#include "{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h"
#include <boost/filesystem.hpp>
#include <exception>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <string>

namespace fs = boost::filesystem;

fs::path testDataBasePath = "./";
fs::path tempFolderBasePath = "";

TEST({{cookiecutter.project_slug}}, getVersion)
{
    auto version = {{cookiecutter.cpp_namespace_root}}::{{cookiecutter.cpp_namespace_project}}::getLibVersion();
    ASSERT_EQ(version, {{cookiecutter.project_slug|upper}}_VERSION_STRING);
}

int main(int argc, char **argv)
{
    ::google::InitGoogleLogging(argv[0]);

    //init gtest framework
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::FLAGS_gtest_death_test_style = "fast";

    // for debugging you may want to select test to run
    // ::testing::GTEST_FLAG(filter) = "{{cookiecutter.project_slug}}.getVersion";

    int retVal = 0;
    for (int i = 0; i < 1; ++i)
    {
        retVal += RUN_ALL_TESTS();
    }

    return retVal;
}
