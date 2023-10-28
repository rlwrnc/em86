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

void test_decode_mov_byte_to_register_positive()
{
	uint8_t mov_al_12[] = {0xb0, 0x0c};
	
	Instruction instruction = decode_instruction(mov_al_12);
	TEST_INSTRUCTION(instruction, MOV_REG_IMMED, 2, AL, IMMEDIATE, 0, 0x0c);
}

void test_decode_mov_byte_to_register_negative()
{
	uint8_t mov_al_n12[] = {0xb0, 0xf4};

	Instruction instruction = decode_instruction(mov_al_n12);
	TEST_INSTRUCTION(instruction, MOV_REG_IMMED, 2, AL, IMMEDIATE, 0, 0xf4);
}

void test_decode_mov_word_to_register()
{
	uint8_t mov_bx_3858[] = {0xbb, 0x12, 0x0f};
	
	Instruction instruction = decode_instruction(mov_bx_3858);
	TEST_INSTRUCTION(instruction, MOV_REG_IMMED, 3, BX, IMMEDIATE, 0, 0x0f12);
}

void test_decode_mov_word_to_register_sign_extension()
{
	uint8_t mov_ax_n12[] = {0xb8, 0xf4, 0xff};

	Instruction instruction = decode_instruction(mov_ax_n12);
	TEST_INSTRUCTION(instruction, MOV_REG_IMMED, 3, AX, IMMEDIATE, 0, 0xfff4);
}

void test_decode_mov_register_to_memory_signed_displacement()
{
	uint8_t mov_si_n300_cx[] = {0x89, 0x8c, 0xd4, 0xfe};

	Instruction instruction = decode_instruction(mov_si_n300_cx);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 4, SI_16, CX, (uint16_t) -300, 0);
}

void test_decode_mov_memory_to_register_signed_displacement()
{
	uint8_t mov_dx_bx_n30[] = {0x8b, 0x57, 0xe2 };

	Instruction instruction = decode_instruction(mov_dx_bx_n30);
	TEST_INSTRUCTION(instruction, MOV_RM_REG, 3, DX, BX_8, 0xe2, 0);
}

void test_decode_mov_byte_to_memory_explicit()
{
	uint8_t mov_bp_di_9[] = {0xc6, 0x03, 0x09};

	Instruction instruction = decode_instruction(mov_bp_di_9);
	TEST_INSTRUCTION(instruction, MOV_RM_IMMED, 3, BP_DI, 0, 0, 9);
}

void test_decode_mov_word_to_memory_explicit()
{
	uint8_t mov_di_901_340[] = {0xc7, 0x85, 0x85, 0x03, 0x54, 0x01};

	Instruction instruction = decode_instruction(mov_di_901_340);
	TEST_INSTRUCTION(instruction, MOV_RM_IMMED, 6, DI_16, 0, 901, 340);
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
	RUN_TEST(test_decode_mov_byte_to_register_positive);
	RUN_TEST(test_decode_mov_byte_to_register_negative);
	RUN_TEST(test_decode_mov_word_to_register);
	RUN_TEST(test_decode_mov_word_to_register_sign_extension);
	RUN_TEST(test_decode_mov_register_to_memory_signed_displacement);
	RUN_TEST(test_decode_mov_memory_to_register_signed_displacement);
	RUN_TEST(test_decode_mov_word_to_memory_explicit);
	RUN_TEST(test_decode_mov_word_to_memory_explicit);
	return UnityEnd();
}
