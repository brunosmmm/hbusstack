#ifndef _MICROCODE_H_INCLUDED
#define _MICROCODE_H_INCLUDED

#include "setup.h"

///This is where the raw data is at memory
#define UCODE_IMEM_BASE_ADDR 0xFEAD

#define UCODE_ANEM_EXTENSION 0

#define INSTRUCTION_MEM_SIZE	128
#define DATA_MEM_SIZE			1

//OPCODES

#define OPCODE_R	0x0
#define OPCODE_S	0x1

//J TYPE
#define OPCODE_J	0x8
#define OPCODE_JAL	0x9

//W TYPE
#define OPCODE_BEQ 	0x6
#define OPCODE_JR	0x7
#define OPCODE_SW	0x4
#define OPCODE_LW	0x5

//L TYPE
#define OPCODE_LIW	0xC //load immediate word BANK

//M TYPE
#define OPCODE_MFHI	0xA
#define OPCODE_MFLO	0xB
#define OPCODE_MFRET    0x2

#define OPCODE_M1	0xD
#define OPCODE_RET      0xE

#define FUNC_LHL	0x0
#define FUNC_LIH	0x1
#define FUNC_LIL	0x2
#define FUNC_AIS	0x3


//ARITHMETIC FUNCTIONS
#define FUNC_ADD	0x0
#define FUNC_SUB	0x1
#define FUNC_AND	0x2
#define FUNC_OR		0x3
#define FUNC_XOR	0x4
#define FUNC_NOT	0x5
#define FUNC_MULT	0x6
#define FUNC_SLT	0x7
#define FUNC_DIV	0x8
#if UCODE_ANEM_EXTENSION
#define FUNC_ADDI	0x9
#define FUNC_SUBI	0xA
#define FUNC_ANDI	0xB
#define FUNC_ORI	0xC
#define FUNC_XORI	0xD
#endif

//SHIFT FUNCTIONS
#define FUNC_SHR	0x0
#define FUNC_SHL	0x1
#define FUNC_ROR	0x2
#define FUNC_ROL	0x3

//FLAGS
#define UCODE_PAUSE		0x01
#define UCODE_RUNNING	0x02
#define UCODE_ZERO		0x04

//INTERRUPT
#define OPCODE_INT	0xF

//MISC
#define UCODE_INST_OPCODE_MASK	0xF000
#define UCODE_INST_OPCODE_POS	12

typedef struct CPU_INSTRUCTION_R
{
	
	byte REGA : 4;
	byte OPCODE : 4;
	byte FUNC : 4;
	byte REGB : 4;
	
} UCODE_INSTRUCTION_R;
//R: OPCODE(4) REGA(4) REGB(4) FUNC(4)

typedef struct CPU_INSTRUCTION_S
{
	
	byte REG : 4;
	byte OPCODE : 4;
	byte FUNC : 4;
	byte SHAMT : 4;
	
} UCODE_INSTRUCTION_S;
//S: OPCODE(4) REG(4) SHAMT(4) FUNC(4)

typedef struct CPU_INSTRUCTION_W
{
	
	byte REGD : 4;
	byte OPCODE : 4;
	byte OFFSET : 4;
	byte REGS : 4;
	
} UCODE_INSTRUCTION_W;
//W: OPCODE(4) REGD(4) REGS(4) OFFSET(4)

typedef struct CPU_INSTRUCTION_J
{
	
	byte ADDRESS_2 : 4;
	byte OPCODE : 4;
	byte ADDRESS_0 : 4;
	byte ADDRESS_1 : 4;
	
} UCODE_INSTRUCTION_J;

typedef struct CPU_INSTRUCTION_L
{
	byte REG : 4;
	byte OPCODE : 4;
	byte DATA;
	
} UCODE_INSTRUCTION_L;

//L: OPCODE(4) REG(4) DATA(8)

typedef struct CPU_INSTRUCTION_M
{
	
	byte REG : 4;
	byte OPCODE : 4;
	byte _NULL_ ;
	
} UCODE_INSTRUCTION_M;

typedef struct CPU_INSTRUCTION_M1
{
	byte MOP 	: 4;
	byte OPCODE	: 4;
	byte DATA	: 8;
	
} UCODE_INSTRUCTION_M1;

typedef struct CPU_INSTRUCTION_GENERIC
{
	
	byte NIBBLE_2: 4;
	byte OPCODE: 4;
	byte NIBBLE_0 :4;
	byte NIBBLE_1: 4;
	
} UCODE_INSTRUCTION_GENERIC;
//GENERIC: OPCODE(4) NIBBLE2(4) NIBBLE1(4) NIBBLE0(4)

typedef union CPU_INSTRUCTIONS
{
	
	UCODE_INSTRUCTION_R R;
	UCODE_INSTRUCTION_S S;
	UCODE_INSTRUCTION_W W;
	UCODE_INSTRUCTION_L L;
	UCODE_INSTRUCTION_J J;
	UCODE_INSTRUCTION_GENERIC G;
	UCODE_INSTRUCTION_M M;
	UCODE_INSTRUCTION_M1 M1;
	
} UCODE_INSTRUCTION;

typedef struct CPUEMU_S
{
	//registradores
	word PC;			//program counter
	byte W;				//W register (acc)
	byte BANK[16];		//register bank
	
	//byte ALUREG; //ALU OUT
	
	byte STATUS;		//status register
	
	//extension registers
	byte REGHI;
	byte REGLO;
        word REGRET;
	
	UCODE_INSTRUCTION INST; //instruction register
	
} UCODE_CPUDATA;


void UCODE_STARTUP(void);
void UCODE_CYCLE(void);
void UCODE_PAUSE_(void);
void UCODE_UNPAUSE(void);

#endif
