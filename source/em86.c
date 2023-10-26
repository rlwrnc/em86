#include "em86.h"

static RMID reg_lookup[16] = {
	AL, CL, DL, BL, AH, CH, DH, BH,
	AX, CX, DX, BX, SP, BP, SI, DI
};

static RMID rm_lookup[24] = {
	BX_SI, BX_DI, BP_SI, BP_DI, M_SI, M_DI, DIRECT, M_BX,
	BX_SI_8, BX_DI_8, BP_SI_8, BP_DI_8, SI_8, DI_8, BP_8, BX_8,
	BX_SI_16, BX_DI_16, BP_SI_16, BP_DI_16, SI_16, DI_16, BP_16, BX_16,
};

void instruction_to_string(Instruction inst, char *string, size_t string_size)
{
	#define FORMAT_MAX 64
	static const char *class_format_lookup[FORMAT_MAX] = {
		"mov %s, %s"
	};

	static const char *rmid_string_lookup[64] = {
		"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
		"ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
		"[bx + si]", "[bx + di]", "[bp + si]", "[bp + di]",
		"[m + si]", "[m + di]", "[direct]", "[m + bx]",
		"[bx + si + D8]", "[bx + di + D8]", "[bp + si + D8]", "[bp + di + D8]",
		"[si + D8]", "[di + D8]", "[bp + D8]", "[bx + D8]",
		"[bx + si + D16]", "[bx + di + D16]", "[bp + si + D16]", "[bp + di + D16]",
		"[si + D16]", "[di + D16]", "[bp + D16]", "[bx + D16]",
	};

	char format[FORMAT_MAX];
	const char *dest = rmid_string_lookup[inst.destination];
	const char *source = rmid_string_lookup[inst.source];
	memcpy(string, class_format_lookup[inst.type], string_size);
	snprintf(string, string_size, format, inst.destination, inst.source);
}

void print_instruction(Instruction instruction)
{
	char instruction_string[64] = "";
	printf("%s\n", instruction_string);
}

Instruction decode_mov_rm_reg(const uint8_t *const inst_ptr)
{
	Instruction inst = { MOV_RM_REG, 2 };

	bool is_wide = inst_ptr[0] & 1;
	bool to_reg = inst_ptr[0] & 2;

	uint8_t mode = (inst_ptr[1] & 0b11000000) >> 6;
	uint8_t reg_index = ((inst_ptr[1] & 0b00111000) >> 3) | (is_wide << 3);
	uint8_t rm_index = (inst_ptr[1] & 0b00000111);

	uint16_t reg = reg_lookup[reg_index];
	uint16_t rm;

	if (mode == 0) {
		if (rm_index == 0b110) {
			inst.displacement = (inst_ptr[3] << 8) | inst_ptr[2];
			inst.number_of_bytes += 2;
		}
		rm_index |= (mode << 3);
		rm = rm_lookup[rm_index];
	}

	if (mode == 1) {
		rm_index |= (mode << 3);
		rm = rm_lookup[rm_index];
		inst.displacement = inst_ptr[2];
		inst.number_of_bytes += 1;
	}

	if (mode == 2) {
		rm_index |= (mode << 3);
		rm = rm_lookup[rm_index];
		inst.displacement = (inst_ptr[3] << 8) | inst_ptr[2];
		inst.number_of_bytes += 2;
	}

	if (mode == 3) {
		rm_index |= (is_wide << 3);
		rm = reg_lookup[rm_index];
	}

	if (to_reg) {
		inst.destination = reg;
		inst.source = rm;
	} else {
		inst.destination = rm;
		inst.source = reg;
	}

	return inst;
}

Instruction decode_instruction(unsigned char *instruction_ptr)
{
	typedef Instruction (*DecodeFunc)(const uint8_t *const);
	static DecodeFunc opcode_dispatch[256] = {
		[0x88] = decode_mov_rm_reg,
		[0x89] = decode_mov_rm_reg,
		[0x8a] = decode_mov_rm_reg,
		[0x8b] = decode_mov_rm_reg,
	};
	
	DecodeFunc decode = opcode_dispatch[instruction_ptr[0]];
	return decode(instruction_ptr);
}

void execute_instruction()
{
}

void assemble_instruction()
{
}
