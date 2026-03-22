cmake_minimum_required(VERSION 3.16)
project(Project2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(generate_dataset
  scripts/generate_main.cpp
  scripts/generator.cpp
  scripts/data_common.cpp
)

add_executable(clean_dataset
  scripts/clean_main.cpp
  scripts/cleaner.cpp
  scripts/data_common.cpp
)

target_include_directories(generate_dataset PRIVATE scripts)
target_include_directories(clean_dataset PRIVATE scripts)
