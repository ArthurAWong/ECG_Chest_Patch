# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ECG_Chestpatch)

target_sources(app PRIVATE src/main.c src/max30003.c src/lsm6dsm.c src/spi_config.c)
zephyr_library_include_directories(./include)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE
  ${app_sources}
  )

# zephyr_library_include_directories(${ZEPHYR_BASE}/samples/bluetooth)

# target_sources(app PRIVATE src/main.c src/max30003.c src/spi_config.c)
zephyr_library_include_directories(./include)
zephyr_library_include_directories(C:/ncs/v2.1.2/zephyr/include/) # Might only work on Vince's computer
