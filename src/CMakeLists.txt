cmake_minimum_required(VERSION 3.22)
project(Hex_coder)

OPTION(ON_WINDOWS "True if on Windows" OFF)
list(APPEND SOURCES Platform_common.c)

add_executable(Hex_coder main.c)
if(ON_WINDOWS)
	list(APPEND SOURCES Windows.c)
endif()
target_sources(Hex_coder PRIVATE ${SOURCES})

