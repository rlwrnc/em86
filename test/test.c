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
		// mov ch, ah
		0x88, 0xe5,
	};

	Instruction instruction = decode_instruction(bytes);
	TEST_ASSERT_EQUAL_UINT(MOV_RM_REG, instruction.type);
	TEST_ASSERT_EQUAL_UINT(2, instruction.number_of_bytes);
	TEST_ASSERT_EQUAL_UINT16(CH, instruction.destination);
	TEST_ASSERT_EQUAL_UINT16(AH, instruction.source);
	TEST_ASSERT_EQUAL_UINT16(0, instruction.displacement);
	TEST_ASSERT_EQUAL_UINT16(0, instruction.data);
}

void test_decode_mov_reg_to_reg_wide()
{
	uint8_t bytes[] = {
		// mov dx, bx
		0x89, 0xda,
	};

	Instruction instruction = decode_instruction(bytes);
	TEST_ASSERT_EQUAL_UINT(MOV_RM_REG, instruction.type);
	TEST_ASSERT_EQUAL_UINT(2, instruction.number_of_bytes);
	TEST_ASSERT_EQUAL_UINT16(DX, instruction.destination);
	TEST_ASSERT_EQUAL_UINT16(BX, instruction.source);
	TEST_ASSERT_EQUAL_UINT16(0, instruction.displacement);
	TEST_ASSERT_EQUAL_UINT16(0, instruction.data);
}

void test_decode_mov_immediate_to_register_byte()
{
	uint8_t mov_cl_12[] = {0xb1, 0x0c};
	uint8_t mov_cl_neg_12[] = {0xb6, 0xf4};

	Instruction instruction = {0};
}

void test_decode_mov_immediate_to_register_wide()
{
	uint8_t mov_cx_12[] = {0xb9, 0x0c, 0x00};
	uint8_t mov_cx_neg_12[] = {0xb9, 0xf4, 0xff};
	uint8_t mov_dx_4321[] = {0xba, 0xe1, 0x10};
	uint8_t mov_dx_neg_4321[] = {0xba, 0x1f, 0xef};

	Instruction instruction = {0};
}

void test_decode_mov_memory_to_register_no_displacement()
{
	uint8_t mov_al_bx_plus_si[] = {0x8a, 0x00};
	uint8_t mov_bx_bp_plus_di[] = {0x8b, 0x1b};
	uint8_t mov_dx_bp[] = {0x8b, 0x56, 0x00};

	Instruction instruction = {0};
}

void test_decode_mov_memory_to_register_byte_displacement()
{
	uint8_t mov_ah_bh_si_4[] = {0x8a, 0x60, 0x04};

	Instruction instruction = {0};
}

void test_decode_mov_memory_to_register_wide_displacement()
{
	uint8_t mov_al_bx_si_5000[] = {0x8a, 0x80, 0x87, 0x13};

	Instruction instruction = {0};
}

int main()
{
	UnityBegin("test.c");
	RUN_TEST(test_decode_mov_reg_to_reg_byte);
	RUN_TEST(test_decode_mov_immediate_to_register_byte);
	return UnityEnd();
}
