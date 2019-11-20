#include <{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h>
{% if cookiecutter.use_docopts == 'Y' -%}#include <docopt.h>{% endif %}
#include <iostream>
#include <boost/log/trivial.hpp>


static const char USAGE[] =
    R"({{cookiecutter.project_slug}}_exe

    Usage:
        company_utilities_exe
        company_utilities_exe [options]

    Options:
      -h --help                    Show this message.
      -v --version                 Show version.
      --log-level-file=<level>     Logging level for file (trace, debug, info, warning, error, fatal) [default: trace]
      --log-level-console=<level>  Logging level for console (trace, debug, info, warning, error, fatal) [default: info]
      --log-file=<file>            Log file name, must contain "%xN" formatting for rollover functionality [default: company_utilities_%3N.log]
)";

{% if cookiecutter.use_docopts == 'Y' -%}int main(int argc, char *argv[]){% else %}int main(int, char *[]){% endif %}
{
    // Parse arguments
    {% if cookiecutter.use_docopts == 'Y' -%}std::map<std::string, docopt::value> args =
        docopt::docopt(USAGE,
                       {argv + 1, argv + argc},
                       true,   // show help if requested
                       {{cookiecutter.cpp_namespace_root}}::{{cookiecutter.cpp_namespace_project}}::getLibVersion()); // version string

    // Print the parsed values
    std::cout << "Argument Parsed: " << std::endl;
    for (const auto &item : args)
    {
        std::cout << " " << item.first << ": " << item.second << std::endl;
    }{% else %}std::cout << "Hello from {{cookiecutter.project_slug}}_exe (lib version: " << {{cookiecutter.cpp_namespace_root}}::{{cookiecutter.cpp_namespace_project}}::getLibVersion() << ") !"<< std::endl;{% endif %}

    //Initialise logging
    {{cookiecutter.cpp_namespace_root}}::{{cookiecutter.cpp_namespace_project}}::init_logging(
        args["--log-file"].asString(),
        args["--log-level-file"].asString(),
        args["--log-level-console"].asString());

    // Log sample messages
    BOOST_LOG_TRIVIAL(fatal) << "An fatal severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(warning) << "An warning severity message";
    BOOST_LOG_TRIVIAL(info) << "An info severity message";
    BOOST_LOG_TRIVIAL(debug) << "An debug severity message";
    BOOST_LOG_TRIVIAL(trace) << "An trace severity message";

}
