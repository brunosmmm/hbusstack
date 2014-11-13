/// @file microcode.h
/// @brief Soft CPU emulator for small programs inside HBUS Slave
/// @author Bruno Morais <brunosmmm@gmail.com>
#include "microcode/microcode.h"
#include "hbus/hbus_objects.h"
#include "hbus/hbus_interrupt.h"
#include <string.h> ///for memcpy

static UCODE_CPUDATA CPUDATA;

///EXAMPLE CODE! Replace!!
const word UCODE_IMEM_W[INSTRUCTION_MEM_SIZE] = {0xC00C,
															0x5100,
															0xC201,
															0x0105,
															0x0122,
															0x6122,
															0x800A,
															0xC004,
															0xC1FF,
															0x4010,
															0x800A
};

const UCODE_INSTRUCTION * UCODE_IMEM = (UCODE_INSTRUCTION*)UCODE_IMEM_W;//(UCODE_INSTRUCTION*)(UCODE_IMEM_BASE_ADDR);

//INSTR MEM SETUP

///@todo check for endianness influency on code; this is big endian

byte UCODE_WRITE_IMEM_BYTE(byte b)
{
	static word IMEM_PTR = 0;
	
	(void)IMEMF_SetByteFlash(IMEMF_DataPtr2Addr((void*)(UCODE_IMEM+IMEM_PTR)),b);
	
	if (IMEM_PTR == (INSTRUCTION_MEM_SIZE*sizeof(UCODE_INSTRUCTION)))
	{
		IMEM_PTR = 0;
		
		return 1;
	}
	
	IMEM_PTR++;
	
	return 0;
}

void UCODE_IMEM_WRITE_START(void)
{
	
	UCODE_PAUSE_();
	
}

void UCODE_IMEM_WRITE_END(void)
{
	
	UCODE_STARTUP(); //resets
	
}

//virtual memory
static byte UCODE_LOAD_DATA(byte virtualAddress)
{	
	byte returnByte = 0xFF;
	byte objectNumber = virtualAddress / 4;
	byte objectByte = virtualAddress  % 4;
	byte * objectValue = 0;
	
	if (MCU_OBJECTS[objectNumber]->objectData)
	{
		if (objectByte < MCU_OBJECTS[objectNumber]->objectInfo.objectSize)	returnByte = MCU_OBJECTS[objectNumber]->objectData[objectByte];
		else returnByte = 0xFF; //byte unavailable at object
		
	}
	else if (MCU_OBJECTS[objectNumber]->getObjectData)
	{
		
		objectValue = (byte*)MCU_OBJECTS[objectNumber]->getObjectData();
		
		if (objectByte < MCU_OBJECTS[objectNumber]->objectInfo.objectSize) returnByte = objectValue[objectByte];
		else returnByte = 0xFF;
		
	}
	
	return returnByte;
	
}

static void UCODE_SAVE_DATA(byte DATA, byte virtualAddress)
{
	
	volatile byte objectNumber = virtualAddress / 4;
	volatile byte objectByte = virtualAddress % 4;
	
	byte objectValue[4];
	
	if (MCU_OBJECTS[objectNumber]->objectData)
	{
		
		if (objectByte < MCU_OBJECTS[objectNumber]->objectInfo.objectSize) MCU_OBJECTS[objectNumber]->objectData[objectByte] = DATA;
		
	} else if (MCU_OBJECTS[objectNumber]->setObjectData)
	{
		
		if (objectByte < MCU_OBJECTS[objectNumber]->objectInfo.objectSize)
		{
			
			//objectValue = (byte*)MCU_OBJECTS[objectNumber]->getObjectData();
			(void)memcpy(objectValue,MCU_OBJECTS[objectNumber]->getObjectData(),4);
			
			objectValue[objectByte] = DATA;
			
			MCU_OBJECTS[objectNumber]->setObjectData((void*)objectValue,MCU_OBJECTS[objectNumber]->objectInfo.objectSize);
			
		}
		
	}
	
	
}

void UCODE_STARTUP(void)
{
	
	//RESET
	
	CPUDATA.PC = 0x0000;
	CPUDATA.INST = UCODE_IMEM[0];
	
	CPUDATA.BANK[0] = 0;
	CPUDATA.BANK[1] = 0;
	CPUDATA.BANK[2] = 0;
	CPUDATA.BANK[3] = 0;
	CPUDATA.BANK[4] = 0;
	CPUDATA.BANK[5] = 0;
	CPUDATA.BANK[6] = 0;
	CPUDATA.BANK[7] = 0;
	CPUDATA.BANK[8] = 0;
	CPUDATA.BANK[9] = 0;
	CPUDATA.BANK[10] = 0;
	CPUDATA.BANK[11] = 0;
	CPUDATA.BANK[12] = 0;
	CPUDATA.BANK[13] = 0;
	CPUDATA.BANK[14] = 0;
	CPUDATA.BANK[15] = 0; //this register has a fixed value
	
	CPUDATA.REGHI = 0;
	CPUDATA.REGLO = 0;
	
	CPUDATA.STATUS = UCODE_RUNNING;
	
}

void UCODE_PAUSE_(void)
{
	
	CPUDATA.STATUS |= UCODE_PAUSE;
	
}

void UCODE_UNPAUSE(void)
{
	
	CPUDATA.STATUS &= ~UCODE_PAUSE;
	
}

void UCODE_CYCLE(void)
{
	
	word temp = 0;
	
	if (CPUDATA.STATUS & UCODE_PAUSE) return; //CPU is paused
	
	//FETCH
	CPUDATA.INST = UCODE_IMEM[(unsigned int)CPUDATA.PC];
	
	//DECODE
	switch (CPUDATA.INST.G.OPCODE)
	{
	
		case OPCODE_R:
			
			//arithmetic instructions
			if (CPUDATA.INST.R.REGA != 15)
			{
			
			switch (CPUDATA.INST.R.FUNC)
			{
			
				case FUNC_ADD:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] += CPUDATA.BANK[CPUDATA.INST.R.REGB];
					
					break;
					
				case FUNC_SUB:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] -= CPUDATA.BANK[CPUDATA.INST.R.REGB];
					
					break;
					
					
				case FUNC_AND:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] &= CPUDATA.BANK[CPUDATA.INST.R.REGB];
					
					break;
					
					
				case FUNC_OR:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] |= CPUDATA.BANK[CPUDATA.INST.R.REGB];
					
					break;
					
				case FUNC_XOR:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] ^= CPUDATA.BANK[CPUDATA.INST.R.REGB];
					
					break;
					
				case FUNC_NOT:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] ^= 0xFF;
					
					break;
					
				case FUNC_MULT:
					
					temp = CPUDATA.BANK[CPUDATA.INST.R.REGA] * CPUDATA.BANK[CPUDATA.INST.R.REGB];
					
					CPUDATA.REGLO = (byte)(temp);
					CPUDATA.REGHI = (byte)(temp>>8);
					
					break;
					
				case FUNC_SLT:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] = CPUDATA.BANK[CPUDATA.INST.R.REGA] < CPUDATA.BANK[CPUDATA.INST.R.REGB] ? 1:0;
					
					break;
					
				case FUNC_DIV:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] = (byte)((((word)CPUDATA.REGHI)<<8 | (word)CPUDATA.REGLO) % CPUDATA.BANK[CPUDATA.INST.R.REGB]);
					CPUDATA.BANK[CPUDATA.INST.R.REGB] = (byte)((((word)CPUDATA.REGHI)<<8 | (word)CPUDATA.REGLO) / CPUDATA.BANK[CPUDATA.INST.R.REGB]);
					
					break;

#if UCODE_ANEM_EXTENSION
					
				case FUNC_ADDI:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] += CPUDATA.INST.R.REGB;
					
					break;
					
				case FUNC_SUBI:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] -= CPUDATA.INST.R.REGB;
					
					break;
					
				case FUNC_ANDI:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] &= CPUDATA.INST.R.REGB;
					
					break;
					
				case FUNC_ORI:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] |= CPUDATA.INST.R.REGB;
					
					break;
					
				case FUNC_XORI:
					
					CPUDATA.BANK[CPUDATA.INST.R.REGA] ^= CPUDATA.INST.R.REGB;
					
					break;
#endif
					
				default:
					
					//illegal instruction
					
					break;
			
			}
			}
			
			//CPUDATA.ALUREG = CPUDATA.BANK[CPUDATA.INST.R.REGA];
			CPUDATA.PC++;
			
			break;
			
			
		case OPCODE_S:
			
			//shift instructions
			if (CPUDATA.INST.S.REG != 15)
			{
			
			switch (CPUDATA.INST.S.FUNC)
			{
			
				case FUNC_SHL:
					
					CPUDATA.BANK[CPUDATA.INST.S.REG] <<= CPUDATA.INST.S.SHAMT;
					
					break;
					
				case FUNC_SHR:
					
					CPUDATA.BANK[CPUDATA.INST.S.REG] >>= CPUDATA.INST.S.SHAMT;
					
					break;
					
				case FUNC_ROL:
					
					CPUDATA.BANK[CPUDATA.INST.S.REG] = (CPUDATA.BANK[CPUDATA.INST.S.REG] >> (8-CPUDATA.INST.S.SHAMT)) | (CPUDATA.BANK[CPUDATA.INST.S.REG] << CPUDATA.INST.S.SHAMT);
					
					break;
					
				case FUNC_ROR:
					
					CPUDATA.BANK[CPUDATA.INST.S.REG] = (CPUDATA.BANK[CPUDATA.INST.S.REG] << (8-CPUDATA.INST.S.SHAMT)) | (CPUDATA.BANK[CPUDATA.INST.S.REG] >> CPUDATA.INST.S.SHAMT);
					
					break;
					
				default:
					
					//illegal function
					
					break;
			
			}
			}
			
			//CPUDATA.ALUREG = CPUDATA.BANK[CPUDATA.INST.S.REG];
			CPUDATA.PC++;
			
			break;
			
			
		case OPCODE_JAL:
			
			//saves PC in 13,14
			
			CPUDATA.BANK[13] = (byte)(CPUDATA.PC>>8);
			CPUDATA.BANK[14] = (byte)(CPUDATA.PC);
			
			//J type jump
			
		case OPCODE_J:
			
			//J - inconditional jump
			CPUDATA.PC = (CPUDATA.INST.J.ADDRESS_2<<8)|(CPUDATA.INST.J.ADDRESS_1<<4)|(CPUDATA.INST.J.ADDRESS_0);
			
			break;
			
		case OPCODE_JR:
			
			CPUDATA.PC = CPUDATA.BANK[CPUDATA.INST.W.REGS]+CPUDATA.INST.W.OFFSET;
			
			break;
			
		case OPCODE_BEQ:
			
			if (CPUDATA.BANK[CPUDATA.INST.W.REGS] == CPUDATA.BANK[CPUDATA.INST.W.REGD]) CPUDATA.PC += CPUDATA.INST.W.OFFSET;
			else CPUDATA.PC++;
			
			break;
			
		case OPCODE_LIW:
			
			if (CPUDATA.INST.L.REG != 15)
				CPUDATA.BANK[CPUDATA.INST.L.REG] = CPUDATA.INST.L.DATA;
			
			CPUDATA.PC++;
			
			break;
			
		case OPCODE_LW:
			
			if (CPUDATA.INST.W.REGD != 15)
				CPUDATA.BANK[CPUDATA.INST.W.REGD] = UCODE_LOAD_DATA(CPUDATA.BANK[CPUDATA.INST.W.REGS]+CPUDATA.INST.W.OFFSET);
			
			CPUDATA.PC++;
			
			break;
			
		case OPCODE_SW:
			
			UCODE_SAVE_DATA(CPUDATA.BANK[CPUDATA.INST.W.REGS],CPUDATA.BANK[CPUDATA.INST.W.REGD]);
			
			CPUDATA.PC++;
			
			break;
			
		case OPCODE_MFHI:
			
			CPUDATA.BANK[CPUDATA.INST.M.REG] = CPUDATA.REGHI;
			
			CPUDATA.PC++;
			
			break;
			
		case OPCODE_MFLO:
			
			CPUDATA.BANK[CPUDATA.INST.M.REG] = CPUDATA.REGLO;
			
			CPUDATA.PC++;
			
			break;
			
		case OPCODE_M1:
			
		switch (CPUDATA.INST.M1.MOP)
		{
			case FUNC_LHL:
				
				CPUDATA.REGLO = CPUDATA.BANK[CPUDATA.INST.M1.DATA & 0x0F];
				CPUDATA.REGHI = CPUDATA.BANK[CPUDATA.INST.M1.DATA>>4];
				
				break;
				
			case FUNC_LIH:
				
				CPUDATA.REGHI = CPUDATA.INST.M1.DATA;
				
				break;
				
			case FUNC_LIL:
				
				CPUDATA.REGLO = CPUDATA.INST.M1.DATA;
				
				break;
				
			case FUNC_AIS:
				
				*(word*)(&CPUDATA.REGHI) = *(word*)(&CPUDATA.REGHI) + (signed char)(CPUDATA.INST.M1.DATA); //hack
				
				break;
		}
		
			CPUDATA.PC++;
			
			break;
			
		case OPCODE_INT:
#ifdef HBUS_INTERRUPT_SUPPORT
			INTERRUPT_EMIT(CPUDATA.INST.G.NIBBLE_0|(CPUDATA.INST.G.NIBBLE_1<<4));
#endif
			CPUDATA.PC++;
			
			break;
		
		default:
			
			//illegal instruction
			CPUDATA.PC++;
			
			break;
	
	
	}
	
	//FLAGS
	
	//if (CPUDATA.ALUREG == 0) CPUDATA.STATUS |= UCODE_ZERO;
	//else CPUDATA.STATUS &= ~UCODE_ZERO;
	
}
