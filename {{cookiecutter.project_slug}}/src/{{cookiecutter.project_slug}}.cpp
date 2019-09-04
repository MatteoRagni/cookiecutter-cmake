#include "{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h"

#include <iostream>

namespace {{cookiecutter.cpp_namespace_root}}
{
namespace {{cookiecutter.cpp_namespace_project}}
{
std::string getLibVersion()
{
    return {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_VERSION_STRING;
}

} // namespace {{cookiecutter.cpp_namespace_project}}
} // namespace {{cookiecutter.cpp_namespace_root}}
