#include "../test-framework/unity.h"
#include "../source/em86.h"
#include <stdint.h>

#define TEST_INSTRUCTION(instruction, ex_type, ex_num_bytes, ex_dest, ex_source, ex_disp, ex_data) \
	do { \
		TEST_ASSERT_EQUAL_UINT(ex_type, instruction.type); \
		TEST_ASSERT_EQUAL_UINT(ex_num_bytes, instruction.number_of_bytes); \
		TEST_ASSERT_EQUAL_UINT16(ex_dest, instruction.destination); \
		TEST_ASSERT_EQUAL_UINT16(ex_source, instruction.source); \
		TEST_ASSERT_EQUAL_UINT16(ex_disp, instruction.displacement); \
		TEST_ASSERT_EQUAL_UINT16(ex_data, instruction.data); \
	} while (0)

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
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 2, CH, AH, 0, 0);
}

void test_decode_mov_reg_to_reg_wide()
{
	uint8_t bytes[] = {
		// mov dx, bx
		0x89, 0xda,
	};

	Instruction instruction = decode_instruction(bytes);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 2, DX, BX, 0, 0);
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
	uint8_t mov_al_bx_si[] = {0x8a, 0x00};
	uint8_t mov_bx_bp_di[] = {0x8b, 0x1b};

	Instruction instruction = decode_instruction(mov_al_bx_si);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 2, AL, BX_SI, 0, 0);

	instruction = decode_instruction(mov_bx_bp_di);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 2, BX, BP_DI, 0, 0);
}

void test_decode_mov_memory_to_register_direct()
{
	uint8_t mov_cx_30[] = {0x8b, 0x0e, 0x1e, 00};

	Instruction instruction = decode_instruction(mov_cx_30);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 4, CX, DIRECT, 30, 0);
}

void test_decode_mov_memory_to_register_byte_displacement()
{
	uint8_t mov_ah_bx_si_4[] = {0x8a, 0x60, 0x04};

	Instruction instruction = decode_instruction(mov_ah_bx_si_4);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 3, AH, BX_SI_8, 4, 0);
}

void test_decode_mov_memory_to_register_wide_displacement()
{
	uint8_t mov_al_bx_si_5000[] = {0x8a, 0x80, 0x88, 0x13};

	Instruction instruction = decode_instruction(mov_al_bx_si_5000);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 4, AL, BX_SI_16, 5000, 0);
}

int main()
{
	UnityBegin("test.c");
	RUN_TEST(test_decode_mov_reg_to_reg_byte);
	RUN_TEST(test_decode_mov_reg_to_reg_wide);
	RUN_TEST(test_decode_mov_memory_to_register_no_displacement);
	RUN_TEST(test_decode_mov_memory_to_register_direct);
	RUN_TEST(test_decode_mov_memory_to_register_byte_displacement);
	RUN_TEST(test_decode_mov_memory_to_register_wide_displacement);
	return UnityEnd();
}
