cmake_minimum_required (VERSION 3.15)
project (u8t
	VERSION
		1.4.2
	DESCRIPTION
		"libu8t C library"
	LANGUAGES
		C
)

include(GNUInstallDirs)

add_compile_options(-std=c23 -Wall -Wextra -pedantic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX /usr CACHE PATH "Install path prefix." FORCE)

add_library(${PROJECT_NAME} SHARED
	src/scanner.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 1
	EXPORT_NAME ${PROJECT_NAME}
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

add_custom_target(uninstall COMMAND rm -rf ${CMAKE_INSTALL_PREFIX}/include/u8t; rm -rf ${CMAKE_INSTALL_PREFIX}/lib/libu8t*)

enable_testing()
add_subdirectory(tests)
