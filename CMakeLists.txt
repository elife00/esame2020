set (CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.10)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

project(epidemic-sfml CXX) 
enable_testing()

#this is to discover the configuration of SFML on this platform
list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules) 
find_package(SFML COMPONENTS graphics window system REQUIRED)

#uncomment the following to enable the execution of clang - tidy, if installed
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-readability-else-after-return)

#add instructions to build a program
add_executable(epidemic-sfml main.cpp epidemic.cpp)
target_compile_options(epidemic-sfml PRIVATE -Wall -Wextra)
target_include_directories(epidemic-sfml PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(epidemic-sfml PRIVATE ${SFML_LIBRARIES})

#the following is to build tests and declare them as such
add_executable(run_functions_tests run_test.cpp functions.test.cpp)
add_test(NAME functions_tests COMMAND run_functions_tests)

#add_executable(run_regression_tests run_tests.cpp regression.test.cpp regression.cpp point.cpp)
#add_test(NAME regression_tests COMMAND run_regression_tests)
