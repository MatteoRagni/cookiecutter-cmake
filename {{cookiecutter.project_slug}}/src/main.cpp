#include <{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h>
#include <docopt/docopt.h>
#include <iostream>

static const char USAGE[] =
    R"({{cookiecutter.project_slug}}_exe

    Usage:
      {{cookiecutter.project_slug}}_exe (-h | --help)
      {{cookiecutter.project_slug}}_exe --version

    Options:
      -h --help        Show this screen.
      --version        Show version.
)";

int main(int argc, char *argv[])
{
    {% if cookiecutter.use_docopts == 'Y' -%}
    std::map<std::string, docopt::value> args =
        docopt::docopt(USAGE,
                       {argv + 1, argv + argc},
                       true,   // show help if requested
                       {{cookiecutter.cpp_namespace_root}}::{{cookiecutter.cpp_namespace_project}}::getLibVersion()); // version string
    std::cout << "Argument Parsed: " << std::endl;
    for (const auto &item : args)
    {
        std::cout << "  " << item.first << ": " << item.second << std::endl;
    }
    {% else %}
    std::cout << "Hello from  {{cookiecutter.project_slug}}_exe!" << std::endl;
    {% endif %}
}
