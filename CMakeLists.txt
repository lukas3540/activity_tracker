cmake_minimum_required(VERSION 3.15)

project(activity_tracker)

add_executable(activity_tracker src/main.cpp)

set_property(TARGET activity_tracker PROPERTY CXX_STANDARD 17)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()
