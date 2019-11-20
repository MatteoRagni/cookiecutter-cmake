
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_executable({{cookiecutter.project_slug|lower}}_tests
        src/main.cpp
       )

target_compile_features({{cookiecutter.project_slug|lower}}_tests
        PRIVATE
        cxx_auto_type
        )

target_link_libraries({{cookiecutter.project_slug|lower}}_tests
        ${Boost_LIBRARIES}
        {{cookiecutter.cpp_namespace_root|lower}}::{{cookiecutter.cpp_namespace_project|lower}}
        )

target_compile_definitions({{cookiecutter.project_slug|lower}}_tests
        PRIVATE
        BOOST_TEST_DYN_LINK
        )

add_test(
        NAME {{cookiecutter.project_slug|lower}}_tests
        COMMAND {{cookiecutter.project_slug|lower}}_tests --logger=HRF,all --color_output=true --report_format=XML --report_sink=test_report.xml
)
