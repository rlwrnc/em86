#include "../em86.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define BEGIN_TESTS() size_t amount = 0, passes = 0, failures = 0
#define RUN_TEST(function) function() ? (amount++, passes++) : (amount++, failures++)
#define END_TESTS() fprintf(stderr, "\ntotal: %lu, passed: %lu, failed: %lu\n", amount, passes, failures);

bool test_decode_mov_reg_to_reg()
{
	uint8_t bytes[] = {0x89, 0xd9};
	Instruction instruction = decode_instruction(bytes);
	char string[32] = "";
	instruction_to_string(instruction, string);
	if (strcmp(string, "mov cx, bx") != 0) {
		return false;
	}
	return true;
}

int main()
{
	BEGIN_TESTS();
	RUN_TEST(test_decode_mov_reg_to_reg);
	END_TESTS();
	return 0;
}
