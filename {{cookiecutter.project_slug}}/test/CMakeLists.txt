find_package(GTest REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(glog REQUIRED)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable({{cookiecutter.project_slug|lower}}_tests
        src/main.cpp
       )

target_compile_features({{cookiecutter.project_slug|lower}}_tests
        PRIVATE
        cxx_auto_type
        )

target_link_libraries({{cookiecutter.project_slug|lower}}_tests
        ${Boost_LIBRARIES}
        ${GTEST_LIBRARIES}
        glog::glog
        Threads::Threads
        {{cookiecutter.cpp_namespace_root|lower}}::{{cookiecutter.cpp_namespace_project|lower}}
        )

add_test(
        NAME {{cookiecutter.project_slug|lower}}_tests
        COMMAND {{cookiecutter.project_slug|lower}}_tests --gtest_output=xml:test_report.xml
)
