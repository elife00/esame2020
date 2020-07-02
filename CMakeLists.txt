cmake_minimum_required(VERSION 3.10)

project(epidemic-sfml CXX)
enable_testing()

# this is to discover the configuration of SFML on this platform
list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules)
find_package(SFML COMPONENTS graphics window system REQUIRED)

# uncomment the following to enable the execution of clang-tidy, if installed
# set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-readability-else-after-return)

# add instructions to build a program
add_executable(epidemic-sfml gui3.cpp)
target_compile_options(epidemic-sfml PRIVATE -Wall -Wextra)
target_include_directories(epidemic-sfml PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(epidemic-sfml PRIVATE ${SFML_LIBRARIES})

# the following is to build tests and declare them as such
# add_executable(run_point_tests run_tests.cpp point.test.cpp point.cpp)
# add_test(NAME point_tests COMMAND run_point_tests)

# add_executable(run_regression_tests run_tests.cpp regression.test.cpp regression.cpp point.cpp)
# add_test(NAME regression_tests COMMAND run_regression_tests)
