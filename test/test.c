#include "../test-framework/unity.h"
#include "../source/em86.h"
#include <stdint.h>

#define test_decode_instruction(expected, actual) \
	do {}

void setUp()
{
}

void tearDown()
{
}

void test_decode_mov_reg_to_reg_byte()
{
	uint8_t bytes[] = {
		// mov cx, bx
		0x89, 0xd9,
		// mov ch, ah
		0x88, 0xe5,
		// mov dx, bx
		0x89, 0xda,
		// mov si, bx
		0x89, 0xde,
		// mov bx, di
		0x89, 0xfb,
		// mov ch, ch
		0x88, 0xc8,
		// mov al, cl
		0x88, 0xed,
		// mov bx, ax
		0x89, 0xc3,
		// mov bx, si
                0x89, 0xf3,
		// mov sp, di
		0x89, 0xfc,
		// mov bp, ax
		0x89, 0xc5                         
	};

	const char *instruction_strings[] = {
		"mov cx, bx",
		"mov ch, ah",
		"mov dx, bx",
		"mov si, bx",
		"mov bx, di",
		"mov al, cl",
		"mov ch, ch",
		"mov bx, ax",
		"mov bx, si",
		"mov sp, di",
		"mov bp, ax"
	};

	Instruction instruction = {0};
	char string[32] = "";

	for (int i = 0, j = 0; i < sizeof bytes; i += instruction.number_of_bytes, j++) {
		instruction = decode_instruction(&bytes[i]);
		instruction_to_string(instruction, string);
		TEST_ASSERT_EQUAL_STRING(instruction_strings[j], string);
	}
}

void test_decode_mov_reg_to_reg_wide()
{}

void test_decode_mov_immediate_to_register_byte()
{
	uint8_t mov_cl_12[] = {0xb1, 0x0c};
	uint8_t mov_cl_neg_12[] = {0xb6, 0xf4};

	Instruction instruction = {0};
	char string[32] = "";

	decode_instruction(mov_cl_12);
	instruction_to_string(instruction, string);
	TEST_ASSERT_EQUAL_STRING("mov cl, 12", string);

	decode_instruction(mov_cl_neg_12);
	instruction_to_string(instruction, string);
	TEST_ASSERT_EQUAL_STRING("mov cl, -12", string);
}

void test_decode_mov_immediate_to_register_wide()
{
	uint8_t mov_cx_12[] = {0xb9, 0x0c, 0x00};
	uint8_t mov_cx_neg_12[] = {0xb9, 0xf4, 0xff};
	uint8_t mov_dx_4321[] = {0xba, 0xe1, 0x10};
	uint8_t mov_dx_neg_4321[] = {0xba, 0x1f, 0xef};

	Instruction instruction = {0};
	char string[32] = "";

	instruction = decode_instruction(mov_cx_12);
	instruction_to_string(instruction, string);
	TEST_ASSERT_EQUAL_STRING("mov cx, 12", string);

	instruction = decode_instruction(mov_cx_neg_12);
	instruction_to_string(instruction, string);
	TEST_ASSERT_EQUAL_STRING("mov cx, -12", string);

	instruction = decode_instruction(mov_dx_4321);
	instruction_to_string(instruction, string);
	TEST_ASSERT_EQUAL_STRING("mov dx, 4321", string);

	instruction = decode_instruction(mov_dx_neg_4321);
	instruction_to_string(instruction, string);
	TEST_ASSERT_EQUAL_STRING("mov dx, -4321", string);
}

void test_decode_mov_source_address_no_displacement()
{}

void test_decode_mov_source_address_byte_displacement()
{}

void test_decode_mov_source_address_wide_displacement()
{}

int main()
{
	UnityBegin("test.c");
	RUN_TEST(test_decode_mov_reg_to_reg_byte);
	RUN_TEST(test_decode_mov_immediate_to_register_byte);
	RUN_TEST(test_decode_mov_immediate_to_register_wide);
	return UnityEnd();
}
