cmake_minimum_required(VERSION 3.19)
include(CMakePrintHelpers)
######################################
# PROJECT
######################################
project(mpp LANGUAGES C VERSION 1.0.1)
message(STATUS "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*")
enable_testing()

macro(Git_WC_INFO dir)
    
endmacro()


######################################
# Options
######################################
set(CMAKE_VERBOSE_MAKEFILE ON)
option(OUTPUT_COMPILER_INFO "Output compiler information when launching the main executable." ON)
list(APPEND CMAKE_C_FLAGS "-pthread")
######################################
# Compiler configuration
######################################

set(CMAKE_C_STANDARD 11)

######################################
# Output some variables
######################################
cmake_print_variables(CMAKE_C_FLAGS)
cmake_print_variables(CMAKE_C_FLAGS_DEBUG)
cmake_print_variables(CMAKE_C_FLAGS_RELEASE)
cmake_print_variables(CMAKE_ROLE)
######################################
# Packages and dependencies initialization
######################################
find_package(PkgConfig REQUIRED)
find_package(Git)
if(Git_FOUND)
    cmake_print_variables(GIT_EXECUTABLE)
endif()
pkg_check_modules(UUID REQUIRED IMPORTED_TARGET uuid>=2.34)
pkg_check_modules(JANSSON REQUIRED IMPORTED_TARGET jansson>=2.12)

######################################
# Standard paths
######################################
set(MPP_PUBLIC_INCLUDE_DIR headers)
set(MPP_SOURCE_DIR src)

######################################
# Standard paths
######################################
configure_file(headers/MPPLibraryConfig.h.in MPPLibraryConfig.h)
######################################
# MPP Lists: Sources, Headers, etc ...
######################################
set(MPP_PRIVATE_INCLUDE_DIR headers/private)
set(MPP_C_PUBLIC_HEADERS
        ${MPP_PUBLIC_INCLUDE_DIR}/MPPLibrary.h
        ${PROJECT_BINARY_DIR}/MPPLibraryConfig.h)
set(MPP_C_PRIVATE_HEADERS
        ${MPP_PRIVATE_INCLUDE_DIR}/mpp_private.h)
set(MPP_C_SOURCE_FILES
        ${MPP_SOURCE_DIR}/MPPLibrary.c
        ${MPP_SOURCE_DIR}/MPPNetworkFrame.c
        ${MPP_SOURCE_DIR}/MPPSession.c)

######################################
# MPP target
######################################
add_library(mpp SHARED ${MPP_C_PUBLIC_HEADERS} ${MPP_C_SOURCE_FILES} ${MPP_C_PRIVATE_HEADERS})
set_target_properties(mpp PROPERTIES SOVERSION 1 OUTPUT_NAME "mpp-${CMAKE_PROJECT_VERSION}")
get_target_property(MPP_OUTPUT_NAME mpp OUTPUT_NAME)
cmake_print_variables(MPP_OUTPUT_NAME)
target_include_directories(mpp PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(mpp PkgConfig::UUID)
target_link_libraries(mpp PkgConfig::JANSSON)

######################################
# TESTS
######################################
# Premier test: basic_linking_test
set(MPP_TEST_BASIC_LINKING_SOURCES tests/basic_linking/basic_linking.c)
add_executable(basic_linking_test ${MPP_TEST_BASIC_LINKING_SOURCES} ${MPP_C_PUBLIC_HEADERS})
target_include_directories(basic_linking_test PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headers>
        $<INSTALL_INTERFACE:include/mpp>)
target_link_libraries(basic_linking_test mpp)

add_test(NAME BasicLinking COMMAND basic_linking_test)

set(MPP_TEST_BASIC_JSON_SOURCES tests/basic_json/basic_json.c)
add_executable(basic_json_test ${MPP_TEST_BASIC_JSON_SOURCES} ${MPP_C_PUBLIC_HEADERS})
target_include_directories(basic_json_test PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headers>
        $<INSTALL_INTERFACE:include/mpp>)
target_link_libraries(basic_json_test mpp)

add_test(NAME BasicJson COMMAND basic_json_test)
message(STATUS "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*")