cmake_minimum_required(VERSION 3.16)
project(Project2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(generate_dataset
  scripts/data_pipeline/generate_main.cpp
  scripts/data_pipeline/generator.cpp
  scripts/data_pipeline/data_common.cpp
)

add_executable(clean_dataset
  scripts/data_pipeline/clean_main.cpp
  scripts/data_pipeline/cleaner.cpp
  scripts/data_pipeline/data_common.cpp
)

target_include_directories(generate_dataset PRIVATE scripts/data_pipeline)
target_include_directories(clean_dataset PRIVATE scripts/data_pipeline)

set_target_properties(generate_dataset clean_dataset PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
)
