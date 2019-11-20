#ifndef {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_{{cookiecutter.project_slug.upper().replace('-', '_')}}_H
#define {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_{{cookiecutter.project_slug.upper().replace('-', '_')}}_H

#include <string>
#include "{{cookiecutter.project_slug}}/version.h"
#include <boost/log/trivial.hpp>


namespace {{cookiecutter.cpp_namespace_root}}
{
namespace {{cookiecutter.cpp_namespace_project}}
{

std::string getLibVersion();
void init_logging(const std::string &logFileName, const std::string &sev_file_str, const std::string &sev_console_str);

} // namespace {{cookiecutter.cpp_namespace_project}}
} // namespace {{cookiecutter.cpp_namespace_root}}

#endif // {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_{{cookiecutter.project_slug.upper().replace('-', '_')}}_H
