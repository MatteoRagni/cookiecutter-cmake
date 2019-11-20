#include "{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h"

#define BOOST_TEST_MODULE {{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}_tests
#include "{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(versionTest)
    {
        auto version = {{cookiecutter.cpp_namespace_root}}::{{cookiecutter.cpp_namespace_project}}::getLibVersion();
    BOOST_CHECK(version == {{cookiecutter.project_slug|upper}}_VERSION_STRING);
    }