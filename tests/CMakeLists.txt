# Common test configuration function
function(add_scanner_test TEST_NAME TEST_HEADER)
	# Create a minimal main.c for this test
	set(TEST_MAIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}_main.c")
	file(WRITE ${TEST_MAIN}
"#include <unit-check/uc.h>
#include <u8t/scanner.h>
#include \"${CMAKE_CURRENT_SOURCE_DIR}/${TEST_HEADER}\"

int main(void) {
	return UC_PrintResults();
}
")

	add_executable(${TEST_NAME}
		${TEST_MAIN}
		${CMAKE_SOURCE_DIR}/src/scanner.c
	)

	target_include_directories(${TEST_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/include
	)

	target_compile_options(${TEST_NAME} PRIVATE
		-std=c23
		-Wall
		-Wextra
		-pedantic
		-O0
		-g
	)

	target_link_libraries(${TEST_NAME} PRIVATE
		unit-check
	)

	add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

	set_tests_properties(${TEST_NAME} PROPERTIES
		LABELS "scanner"
		TIMEOUT 10
	)
endfunction()

# Add individual test executables
add_scanner_test(test_basic "test_basic.h")
add_scanner_test(test_strings "test_strings.h")
add_scanner_test(test_numbers "test_numbers.h")
add_scanner_test(test_identifiers "test_identifiers.h")
add_scanner_test(test_edge_cases "test_edge_cases.h")
add_scanner_test(test_api "test_api.h")
