cmake_minimum_required(VERSION 3.15)
project(feedback-ingest CXX)
set(PROJECT_AUTHORS "Rishabh Dwivedi")
include(cmake/StandardProjectSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/CompilerWarnings.cmake)
include(package.cmake)
find_package(Boost REQUIRED)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(src)
