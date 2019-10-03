cmake_minimum_required(VERSION 3.8)
project({{cookiecutter.project_slug}}_dependencies)

SET(CMAKE_VERBOSE_MAKEFILE OFF)

# directory where all dependencies will resides
# it can not be subfolder of main project so we remove it to parent level
SET(${PROJECT_NAME}_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../${PROJECT_NAME})

add_custom_target(${PROJECT_NAME})
include(ExternalProject)

# build docopts.cpp
ExternalProject_Add(docopt_ext
        GIT_REPOSITORY git@github.com:docopt/docopt.cpp.git
        GIT_TAG origin/master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        PREFIX ${${PROJECT_NAME}_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_SHARED_LIBS=ON
        )

# add docopts.cpp to main target as dependency
add_dependencies(${PROJECT_NAME} docopt_ext)
