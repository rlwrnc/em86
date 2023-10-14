#include "em86.h"

#define MOV_BFRM 0x88

#define OPCODES \
	X(0x88, "mov", 2, 0b01000, ID_RM, ID_REG) \
	X(0x89, "mov", 2, 0b11000, ID_RM, ID_REG) \
	X(0x8a, "mov", 2, 0b01000, ID_REG, ID_RM) \
	X(0x8b, "mov", 2, 0b11000, ID_REG, ID_RM) \

void instruction_to_string(Instruction instruction, char *string)
{
	static const char *rm_strings[64] = {
		// MOD=00 (memory, no displacement)
		// W=0
		"(bx) + (si)", "(bx) + (di)", "(bp) + (si)", "(bp) + (di)",
		"(si)", "(di)", "DIRECT", "(bx)",
		// W=1
		"(bx) + (si)", "(bx) + (di)", "(bp) + (si)", "(bp) + (di)",
		"(si)", "(di)", "DIRECT", "(bx)",

		// MOD=01 (memory, 8-bit displacement)
		"(bx) + (si) + D8", "(bx) + (di) + D8", "(bp) + (si) + D8", "(bp) + (di) + D8",
		"(si) + D8", "(di) + D8", "(bp) + D8", "(bx) + D8",
		"(bx) + (si) + D8", "(bx) + (di) + D8", "(bp) + (si) + D8", "(bp) + (di) + D8",
		"(si) + D8", "(di) + D8", "(bp) + D8", "(bx) + D8",

		// MOD=10 (memory, 16-bit displacement)
		"(bx) + (si) + D16", "(bx) + (di) + D16", "(bp) + (si) + D16", "(bp) + (di) + D16",
		"(si) + D16", "(di) + D16", "(bp) + D16", "(bx) + D16",
		"(bx) + (si) + D16", "(bx) + (di) + D16", "(bp) + (si) + D16", "(bp) + (di) + D16",
		"(si) + D16", "(di) + D16", "(bp) + D16", "(bx) + D16",

		// MOD=11 (register)
		"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
		"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
	};

	int destination_index, source_index;

	if (instruction.to_register) {
		destination_index = (0b11 << 4) | instruction.destination_id;
		source_index = instruction.source_id;
	} else {
		destination_index = instruction.destination_id;
		source_index = (0b11 << 4) | instruction.source_id;
	}

	sprintf(string, "%s %s, %s", instruction.name, rm_strings[destination_index],
			rm_strings[source_index]);
}

void print_instruction(Instruction instruction)
{
	char instruction_string[32] = "";
	instruction_to_string(instruction, instruction_string);
	printf("%s\n", instruction_string);
}

Instruction decode_instruction(unsigned char *instruction_ptr)
{
	// we only need the macro if we need to redefine the opcode definitions in other
	// functions, which we probably won't need to.
	#if 0
	static const Instruction codes[255] = {
		#define X(code, name, ...) \
			[code] = {name, __VA_ARGS__},
		OPCODES
		#undef X
	};
	#else
	#define IS_REG 1
	static const Instruction codes[255] = {
		[0x88] = {"mov", 2, .second_instruction = true},
		[0x89] = {"mov", 2, .is_wide = true, .second_instruction = true},
		[0x8a] = {"mov", 2, .to_register = true, .second_instruction = true},
		[0x8b] = {"mov", 2, .to_register = true, .is_wide = true, .second_instruction = true}
	};
	#endif
	
	Instruction instruction_code = codes[*instruction_ptr];
	if (instruction_code.second_instruction) {
		unsigned char second_instruction = *(instruction_ptr + 1);
		if (instruction_code.to_register) {
			instruction_code.destination_id = (GET_MOD(second_instruction) << 4)
				| (instruction_code.is_wide << 3) | GET_REG(second_instruction);
			instruction_code.source_id = (GET_MOD(second_instruction) << 4)
				| (instruction_code.is_wide << 3) | GET_RM(second_instruction);
		} else {
			instruction_code.destination_id = (GET_MOD(second_instruction) << 4)
				| (instruction_code.is_wide << 3) | GET_RM(second_instruction);
			instruction_code.source_id = (GET_MOD(second_instruction) << 4)
				| (instruction_code.is_wide << 3) | GET_REG(second_instruction);
		}
	}

	instruction_ptr += instruction_code.number_of_bytes;
	return instruction_code;
}

void execute_instruction()
{
}

void assemble_instruction()
{
}

#if 0
int main()
{
	uint8_t bytes[] = {0x89, 0xd9};
	size_t length = sizeof bytes;
	Instruction instruction = {0};
	for (int i = 0; i < length; i += instruction.number_of_bytes) {
		instruction = decode_instruction(&bytes[i]);
		print_instruction(instruction);
	}
	return 0;
}
#endif
