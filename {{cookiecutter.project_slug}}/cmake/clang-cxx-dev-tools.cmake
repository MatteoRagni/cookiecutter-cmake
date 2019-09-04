# Additional targets to perform .clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
        ALL_CXX_SOURCE_FILES
        src/*.cpp
        src/*.h
        include/*.h
        ${CMAKE_BINARY_DIR}/include/*.h
        )

# Adding .clang-format target if executable is found
find_program(${LIB_TARGET_NAME}_CLANG_FORMAT_EXE
        clang-format-7
        clang-format-8
        clang-format-9)

if(${LIB_TARGET_NAME}_CLANG_FORMAT_EXE)
  add_custom_target(
          ${LIB_TARGET_NAME}_clang-format
          COMMAND
          ${${LIB_TARGET_NAME}_CLANG_FORMAT_EXE}
          -i
          -style=file
          ${ALL_CXX_SOURCE_FILES}
  )
endif()
