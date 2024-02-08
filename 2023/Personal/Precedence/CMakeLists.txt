cmake_minimum_required(VERSION 3.10)

project(Precedence)


add_subdirectory(src build)
target_include_directories(Precedence PUBLIC src)
set_target_properties(Precedence
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
)
target_compile_options(Precedence PRIVATE -g)