cmake_minimum_required(VERSION 3.16)
project(EMR_Cyber_Division LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(scripts)

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

add_executable(emergency_ui
  scripts/main.cpp
  scripts/BinaryMaxHeap.cpp
  scripts/BinomialMaxHeap.cpp
  scripts/data_pipeline/data_common.cpp
  scripts/data_pipeline/generator.cpp
  scripts/data_pipeline/cleaner.cpp
)
