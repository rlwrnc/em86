#ifndef EM86_H
#define EM86_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define GET_RM(byte)  ((byte) & 0b111)
#define GET_REG(byte) (((byte) >> 3) & 0b111)
#define GET_OPCODE_EXTENSION(byte) (GET_REG(byte))
#define GET_MOD(byte) (((byte) >> 6) & 0b11)

#define HAS_SECOND_INST_BYTE(code) ((code.flags) & 0b0100000)
#define HAS_OPCODE_EXTENSION(code) ((code.flags) & 0b0010000)
#define REG_DEST(code) ((code.flags) & 0b100000)

typedef enum {
	MOV_RM_REG
} InstructionClass;

typedef enum {
	AL, CL, DL, BL, AH, CH, DH, BH,
	AX, CX, DX, BX, SP, BP, SI, DI,
	BX_SI, BX_DI, BP_SI, BP_DI, M_SI, M_DI, DIRECT, M_BX,
	BX_SI_8, BX_DI_8, BP_SI_8, BP_DI_8, SI_8, DI_8, BP_8, BX_8,
	BX_SI_16, BX_DI_16, BP_SI_16, BP_DI_16, SI_16, DI_16, BP_16, BX_16,
} RMID;

typedef struct {
	InstructionClass type;
	size_t number_of_bytes;
	RMID destination;
	RMID source;
	uint16_t displacement;
	uint16_t data;
} Instruction;


void instruction_to_string(Instruction, char *, size_t);
Instruction decode_instruction(unsigned char *instruction_ptr);

#endif // EM86_H
