cmake_minimum_required(VERSION 3.22)
project(Hex_coder)

OPTION(ON_WINDOWS "Set ON if on Windows" OFF)
OPTION(ON_LINUX "Set ON if on Linux" OFF)
list(APPEND SOURCES Platform_common.c)

add_executable(Hex_coder main.c)
if(ON_WINDOWS)
	list(APPEND SOURCES Windows.c)
endif()
if(ON_LINUX)
	list(APPEND SOURCES Linux.c)
endif()

target_sources(Hex_coder PRIVATE ${SOURCES})

