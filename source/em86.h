#ifndef EM86_H
#define EM86_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define GET_RM(byte)  ((byte) & 0b111)
#define GET_REG(byte) (((byte) >> 3) & 0b111)
#define GET_OPCODE_EXTENSION(byte) (GET_REG(byte))
#define GET_MOD(byte) (((byte) >> 6) & 0b11)

#define HAS_SECOND_INST_BYTE(code) ((code.flags) & 0b0100000)
#define HAS_OPCODE_EXTENSION(code) ((code.flags) & 0b0010000)
#define REG_DEST(code) ((code.flags) & 0b100000)

typedef struct {
	char name[6];
	unsigned char number_of_bytes;
	// to_register, second_instruction, opcode_extension, disp_low, disp_high, data_low, data_high
	unsigned char flags;
	// TODO: pack bools into bitfield
	bool to_register;
	bool is_wide;
	bool second_instruction;
	bool opcode_extension;
	bool disp_low;
	bool disp_high;
	bool data_low;
	bool data_high;
	unsigned short destination_id;
	unsigned short source_id;
	unsigned short displacement;
	unsigned short immediate;
} Instruction;

void instruction_to_string(Instruction instruction, char *string);
Instruction decode_instruction(unsigned char *instruction_ptr);

#endif // EM86_H
