cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sample_test_suite)

FILE(GLOB test_script src/*.c)
FILE(GLOB test_source ../../src/*.c)
list(FILTER test_source EXCLUDE REGEX "main.c$")


target_sources(app PRIVATE ${test_script})
target_sources(app PRIVATE ${test_source})

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
zephyr_library_include_directories(D:/ncs/v2.1.2/zephyr/subsys/testsuite/ztest/include/zephyr)

