# CMakeList.txt : CMake project for Task06, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

project ("Task06")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
add_compile_options (/W4 /permissive- /Zc:externConstexpr /Zc:inline)
endif ()


# Add source to this project's executable.
add_executable (Task06 "Task06.cpp" )

# TODO: Add tests and install targets if needed.
