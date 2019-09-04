#ifndef {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_{{cookiecutter.project_slug.upper().replace('-', '_')}}_H
#define {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_{{cookiecutter.project_slug.upper().replace('-', '_')}}_H

#include <string>
#include "{{cookiecutter.project_slug}}/version.h"

namespace {{cookiecutter.cpp_namespace_root}}
{
namespace {{cookiecutter.cpp_namespace_project}}
{
std::string getLibVersion();

} // namespace {{cookiecutter.cpp_namespace_project}}
} // namespace {{cookiecutter.cpp_namespace_root}}

#endif // {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_{{cookiecutter.project_slug.upper().replace('-', '_')}}_H
