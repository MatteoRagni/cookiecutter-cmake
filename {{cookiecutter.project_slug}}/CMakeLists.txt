cmake_minimum_required(VERSION 3.8)

project({{cookiecutter.project_slug}} VERSION {{cookiecutter.version}} LANGUAGES CXX)

set(LIB_NAMESPACE {{cookiecutter.cpp_namespace_root}})
set(LIB_NAME {{cookiecutter.cpp_namespace_project}})
set(LIB_TARGET_NAME ${PROJECT_NAME})

# LIB_NAME_CAP is used to procude version.h
string(TOUPPER ${LIB_NAMESPACE}_${LIB_NAME} LIB_NAME_CAP)


SET(CMAKE_USE_CLANG_TIDY OFF)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

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

# local dependencies path
LIST(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/../${PROJECT_NAME}_dependencies)

# cmake path for local files
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
include(cmake/clang-cxx-dev-tools.cmake)

# extract version from GIT
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_COMMIT_HASH)
git_local_changes(GIT_LOCAL_DIRTY)


# generate version header
configure_file(
        cmake/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_TARGET_NAME}/version.h
        @ONLY)

##############################################

# Declare dependencies

{% if cookiecutter.use_docopts == 'Y' -%}
find_package(docopt 0.6.2 REQUIRED)
{% endif %}


##############################################
# Create executable

add_executable(${LIB_TARGET_NAME}_exe
        src/main.cpp
        )

target_link_libraries(${LIB_TARGET_NAME}_exe
        ${LIB_TARGET_NAME}
{% if cookiecutter.use_docopts == 'Y' -%}
        docopt
{% endif %}
        )

# Create library
SET(${LIB_TARGET_NAME}_SOURCES
        src/${LIB_TARGET_NAME}.cpp

        )

add_library(${LIB_TARGET_NAME} SHARED
        ${${LIB_TARGET_NAME}_SOURCES}
        )

# Add an alias so that library can be used inside install treea and the build tree with the same name
add_library(${LIB_NAMESPACE}::${LIB_NAME} ALIAS ${LIB_TARGET_NAME})

target_compile_definitions(${LIB_TARGET_NAME}
        PUBLIC
        PRIVATE
        )

# Set target includes properties
# please note also variable LIB_TARGET_INCLUDE_DIRS used later on, concerning PUBLIC/INTERFACE include locations
target_include_directories(${LIB_TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>

        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_include_directories(${LIB_TARGET_NAME}
        SYSTEM PUBLIC
        $<INSTALL_INTERFACE:include>

        )

# set the PERFORMANCE_TIMERS so we can test them
target_compile_definitions(${LIB_TARGET_NAME}
        PUBLIC
        )

target_compile_features(${LIB_TARGET_NAME}
        PRIVATE
        cxx_std_11
        PUBLIC
        )

target_compile_options(${LIB_TARGET_NAME}
        PRIVATE
        -Wall
        )

# public libraries
SET(${LIB_TARGET_NAME}_PUB_LIBS
        )

#public librarues has to be added manually to cmake/Config.cmake.in so the downstream project will pick it up correctly
SET(${LIB_TARGET_NAME}_PRIVATE_LIBS
        )

# Link dependent libraries
# make sure the public one are declared in cmake/Config.cmake.in as find_dependency
target_link_libraries(${LIB_TARGET_NAME}
        PUBLIC
        ${${LIB_TARGET_NAME}_PUB_LIBS}
        PRIVATE
        ${${LIB_TARGET_NAME}_PRIVATE_LIBS}
        )

# declaring public header list
SET(${LIB_TARGET_NAME}_PUBLIC_HDRS
        include/${LIB_TARGET_NAME}/${LIB_TARGET_NAME}.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_TARGET_NAME}/version.h
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
    if (NOT CLANG_TIDY_EXE)
        message(ERROR "clang-tidy not found.")
    else ()
        SET(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        set_target_properties(${LIB_TARGET_NAME} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
                )
    endif ()
endif (CMAKE_USE_CLANG_TIDY)

# set properties
set_target_properties(${LIB_TARGET_NAME} PROPERTIES
        # link target to export name
        EXPORT_NAME ${LIB_NAME}

        CMAKE_CXX_STANDARD 11

        # enable framework (MAC OS framework suport)
        FRAMEWORK TRUE

        # declare public headers
        PUBLIC_HEADER "${${LIB_TARGET_NAME}_PUBLIC_HDRS}"
        )


##############################################
# Installation instructions


include(GNUInstallDirs)
SET(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME})

# install libratries and headers
install(TARGETS ${LIB_TARGET_NAME}
        EXPORT ${LIB_TARGET_NAME}-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_TARGET_NAME}
        )


# install exported the targets to a script
install(EXPORT ${LIB_TARGET_NAME}-targets
        FILE ${LIB_NAME}Targets.cmake
        NAMESPACE ${LIB_NAMESPACE}::
        DESTINATION ${INSTALL_CONFIGDIR}
        )

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# build configuration
SET(LIB_TARGET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_BINARY_DIR}")
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# install configuration
SET(LIB_TARGET_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/install/${LIB_NAME}Config.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/install/${LIB_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
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
export(EXPORT ${LIB_TARGET_NAME}-targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Targets.cmake
        NAMESPACE ${LIB_NAMESPACE}::
        )

#Register package in user's package registry
export(PACKAGE ${LIB_NAME})

# include test project
enable_testing()
add_subdirectory(test)
