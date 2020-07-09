cmake_minimum_required(VERSION 3.10)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

project(epidemic-sfml CXX) 
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#this is to discover the configuration of SFML on this platform
list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules) 
include_directories(/usr/local/include/SFML)
find_package(SFML COMPONENTS graphics window system REQUIRED)

#uncomment the following to enable the execution of clang - tidy, if installed
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-readability-else-after-return)

#add instructions to build a program
add_executable(epidemic-sfml main.cpp epidemic.cpp)
target_compile_options(epidemic-sfml PRIVATE -Wall -Wextra)
target_include_directories(epidemic-sfml PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
target_link_libraries(epidemic-sfml sfml-system sfml-window sfml-graphics sfml-audio sfml-network)


#the following is to build tests and declare them as such
add_executable(run_functions_tests functions.test.cpp)
add_test(NAME functions_tests COMMAND run_functions_tests)

add_executable(run_epidemic_tests epidemic.test.cpp epidemic.cpp)
target_compile_options(run_epidemic_tests PRIVATE -Wall -Wextra -fsanitize=address)
target_include_directories(run_epidemic_tests PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
target_link_libraries(run_epidemic_tests sfml-system sfml-window sfml-graphics sfml-audio sfml-network -fsanitize=address)
add_test(NAME epidemic_tests COMMAND run_epidemic_tests)

add_executable(run_sfml_tests sfml.test.cpp)
target_compile_options(run_sfml_tests PRIVATE -Wall -Wextra)
target_include_directories(run_sfml_tests PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
target_link_libraries(run_sfml_tests sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
add_test(NAME sfml_tests COMMAND run_sfml_tests)