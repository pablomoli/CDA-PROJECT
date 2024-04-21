#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if((PC >> 2) % 4 != 0) return 1; // HALT

	*instruction = Mem[PC >> 2];
	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op = instruction >> 26 & ((1 << 6) -1); //6 bits

	*r1 = instruction >> 21 & ((1 << 5)-1); // 5 bits
	*r2 = instruction >> 16 & ((1 << 5)-1); // 5 bits
	*r3 = instruction >> 11 & ((1 << 5) -1); // 5 bits

	*funct= instruction  & ((1 << 6) -1); //6 bits
	*offset = instruction & ((1 << 5) - 1); // 16bits
	*jsec == instruction  & ((1 << 26) -1); // 26 bits
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	controls->RegDst = 0;
	controls->Jump = 0;
	controls->Branch = 0;
	controls->MemRead = 0;
	controls->MemtoReg = 0;
	controls->ALUOp = 0;
	controls->MemWrite = 0;
	controls->ALUSrc = 0;
	controls->RegWrite = 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	unsigned isNegative = (offset >> 15);
	if(isNegative){
		*extended_value = offset | 0xFFFF0000;
		return;
	}
	*extended_value = offset & 0x0000FFFF ;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	*PC += 4;

	if(Branch == 1 && Zero == 1){
		*PC += extended_value << 2;
	}
	if(Jump){
		*PC = (jsec << 2) | (*PC & 0xF0000000);
	}
}

