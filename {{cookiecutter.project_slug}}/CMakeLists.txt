cmake_minimum_required(VERSION 3.8)

project({{cookiecutter.project_slug|lower}} VERSION 1.0.0 LANGUAGES CXX)

SET(CMAKE_USE_CLANG_TIDY ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)
#SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# by default use release
if (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release"
            CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            )
endif (NOT CMAKE_BUILD_TYPE)

# default flags based on build type
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

if (NOT CMAKE_USE_CLANG_TIDY)
    SET(CMAKE_USE_CLANG_TIDY OFF
            CACHE STRING "set to true (ON to run clang tidy during build)"
            )
endif (NOT CMAKE_USE_CLANG_TIDY)

# cmake path for local files
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/clang-cxx-dev-tools.cmake)

# extract version from GIT
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_COMMIT_HASH)
git_local_changes(GIT_LOCAL_DIRTY)


# LIB_NAME_CAP is used to procude version.h
SET(LIB_NAME_CAP {{cookiecutter.cpp_namespace_root.upper().replace('-', '_')}}_{{cookiecutter.cpp_namespace_project.upper().replace('-', '_')}})

# generate version header
configure_file(
    cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/{{cookiecutter.project_slug|lower}}/version.h
     @ONLY)

##############################################
# Declare dependencies
find_package(Boost COMPONENTS program_options filesystem REQUIRED)

#Google logging framework
find_package(glog REQUIRED)

#Google command line arguments parser
SET(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags REQUIRED)

##############################################
# Create target and set properties


SET({{cookiecutter.project_slug|lower}}_SOURCES
        src/{{cookiecutter.project_slug|lower}}.cpp
        )

add_library({{cookiecutter.project_slug|lower}} SHARED
        ${{ '{' }}{{cookiecutter.project_slug|lower}}_SOURCES}
        )

# Add an alias so that library can be used inside install treea and the build tree with the same name
add_library({{cookiecutter.cpp_namespace_root|lower}}::{{cookiecutter.cpp_namespace_project|lower}} ALIAS {{cookiecutter.project_slug|lower}})

target_compile_definitions({{cookiecutter.project_slug|lower}}
        PUBLIC
        PRIVATE
        )

# Set target includes properties
# please note also variable LIB_TARGET_INCLUDE_DIRS used later on, concerning PUBLIC/INTERFACE include locations
target_include_directories({{cookiecutter.project_slug|lower}}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>

        PRIVATE
        )

# set the PERFORMANCE_TIMERS so we can test them
target_compile_definitions({{cookiecutter.project_slug|lower}}
        PUBLIC
        )

target_compile_features({{cookiecutter.project_slug|lower}}
        PRIVATE
        cxx_auto_type
        PUBLIC
        )

target_compile_options({{cookiecutter.project_slug|lower}}
        PRIVATE
        -Wall
        )

# public libraries
SET({{cookiecutter.project_slug|lower}}_PUB_LIBS
        ${Boost_LIBRARIES}
        glog::glog
        )

#public librarues has to be added manually to {{cookiecutter.project_slug}}/cmake/Config.cmake.in so the downstream project will pick it up correctly
SET({{cookiecutter.project_slug|lower}}_PRIVATE_LIBS
        glog::glog
        )

# Link dependent libraries
# make sure the public one are declared in cmake/Config.cmake.in as find_dependency
target_link_libraries({{cookiecutter.project_slug|lower}}
        PUBLIC
        ${{ '{' }}{{cookiecutter.project_slug|lower}}_PUB_LIBS}
        PRIVATE
        ${{ '{' }}{{cookiecutter.project_slug|lower}}_PRIVATE_LIBS}
        )

# declaring public header list
SET({{cookiecutter.project_slug|lower}}_PUBLIC_HDRS
        include/{{cookiecutter.project_slug|lower}}/{{cookiecutter.project_slug|lower}}.h
        ${CMAKE_CURRENT_BINARY_DIR}/{{cookiecutter.project_slug|lower}}/version.h
        )

find_program(CLANG_TIDY_EXE
        NAMES
        clang-tidy-9
        clang-tidy-8
        clang-tidy-7
        clang-tidy
        DOC "Path to clang-tidy executable"
        )

if (CMAKE_USE_CLANG_TIDY)
    if(NOT CLANG_TIDY_EXE)
        message(ERROR "clang-tidy not found.")
    else()
        SET(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif (CMAKE_USE_CLANG_TIDY)

# set properties
set_target_properties({{cookiecutter.project_slug|lower}} PROPERTIES
        # link target to export name
        EXPORT_NAME {{cookiecutter.cpp_namespace_project|lower}}

        CMAKE_CXX_STANDARD 11

        # enable framework (MAC OS framework suport)
        FRAMEWORK TRUE

        # declare public headers
        PUBLIC_HEADER "${{ '{' }}{{cookiecutter.project_slug|lower}}_PUBLIC_HDRS}"
        CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
        )


##############################################
# Installation instructions


include(GNUInstallDirs)
SET(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/{{cookiecutter.cpp_namespace_project|lower}})

# install libratries and headers
install(TARGETS {{cookiecutter.project_slug|lower}}
        EXPORT {{cookiecutter.project_slug|lower}}-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/{{cookiecutter.project_slug|lower}}
        )


# install exported the targets to a script
install(EXPORT {{cookiecutter.project_slug|lower}}-targets
        FILE {{cookiecutter.cpp_namespace_project|lower}}Targets.cmake
        NAMESPACE {{cookiecutter.cpp_namespace_root|upper}}::
        DESTINATION ${INSTALL_CONFIGDIR}
        )

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/{{cookiecutter.cpp_namespace_project|lower}}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# build configuration
SET(LIB_TARGET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_BINARY_DIR}")
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/{{cookiecutter.cpp_namespace_project|lower}}Config.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# install configuration
SET(LIB_TARGET_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/install/{{cookiecutter.cpp_namespace_project|lower}}Config.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/install/{{cookiecutter.cpp_namespace_project|lower}}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/{{cookiecutter.cpp_namespace_project|lower}}ConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR}
)

# handy uninstall target
if (NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CmakeUninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/CmakeUninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/CmakeUninstall.cmake)
endif ()

#############################################
# Exporting from the build tree
export(EXPORT {{cookiecutter.project_slug|lower}}-targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/{{cookiecutter.cpp_namespace_project|lower}}Targets.cmake
        NAMESPACE {{cookiecutter.cpp_namespace_root|upper}}::
        )

#Register package in user's package registry
export(PACKAGE {{cookiecutter.cpp_namespace_project|lower}})

# include test project
enable_testing()
add_subdirectory(test)
