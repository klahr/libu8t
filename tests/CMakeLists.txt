add_executable(test_scanner
	main.c
	../src/scanner.c
)

target_include_directories(test_scanner PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

target_compile_options(test_scanner PRIVATE
	-std=c23
	-Wall
	-Wextra
	-pedantic
	-O0
	-g
)

target_link_libraries(test_scanner PRIVATE
	unit-check
)

add_test(NAME AllTests COMMAND test_scanner)

set_tests_properties(AllTests PROPERTIES
	LABELS "scanner"
	TIMEOUT 30
)
