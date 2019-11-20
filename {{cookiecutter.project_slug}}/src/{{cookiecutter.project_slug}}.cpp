#include "{{cookiecutter.project_slug}}/{{cookiecutter.project_slug}}.h"

#include <boost/log/utility/setup.hpp>
#include <iostream>


namespace {{cookiecutter.cpp_namespace_root}}
{
namespace {{cookiecutter.cpp_namespace_project}}
{
std::string getLibVersion()
{
    return {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}}_VERSION_STRING;
}

void init_logging(const std::string &logFileName, const std::string &sev_file_str, const std::string &sev_console_str)
{
    static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");

    boost::log::register_simple_formatter_factory<
        boost::log::trivial::severity_level, char>("Severity");

    // Output message to console
    auto sink_console = boost::log::add_console_log(std::cout,
                                                    boost::log::keywords::format = COMMON_FMT,
                                                    boost::log::keywords::auto_flush = true);

    // Output message to file, rotates when file reached 1mb or at midnight every
    // day. Each log file is capped at 1mb and total is 20mb
    auto sink_file = boost::log::add_file_log(
        boost::log::keywords::file_name = logFileName,
        boost::log::keywords::rotation_size = 1 * 1024 * 1024,
        boost::log::keywords::max_size = 20 * 1024 * 1024,
        boost::log::keywords::time_based_rotation =
            boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = COMMON_FMT,
        boost::log::keywords::auto_flush = true);

    boost::log::add_common_attributes();

    std::stringstream s;

    boost::log::trivial::severity_level  sev_console;
    boost::log::trivial::severity_level  sev_file;
    std::stringstream(sev_console_str) >> sev_console;
    std::stringstream(sev_file_str) >> sev_file;
    sink_console->set_filter(boost::log::trivial::severity >= sev_console);
    sink_file->set_filter(boost::log::trivial::severity >= sev_file);
}

} // namespace {{cookiecutter.cpp_namespace_project}}
} // namespace {{cookiecutter.cpp_namespace_root}}
