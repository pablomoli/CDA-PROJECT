#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch (ALUControl) {
        case '000':
            // Z = A + B
            *ALUresult = A + B;
            break;
        case '001':
            // Z = A - B
            *ALUresult = A - B;
            break;
        case '010':
            // If A < B, Z = 1; otherwise, Z = 0
            *ALUresult = A < B ? 1 : 0;
            break;
        case '011':
            // If A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
            *ALUresult = A < B ? 1 : 0;
            break;
        case '100':
            // Z = A AND B
            *ALUresult = A & B;
            break;
        case '101':
            // Z = A OR B
            *ALUresult = A | B;
            break;
        case '110':
            // Z = Shift B left by 16 bits
            *ALUresult = B << 16;
            break;
        case '111':
            // Z = NOT A
            *ALUresult = ~A;
            break;
    }
    *Zero = (*ALUresult == 0) ? '1' : '0';
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
  *op = instruction >> 26 & ((1 << 6) - 1); //6 bits
  *r1 = instruction >> 21 & ((1 << 5) - 1); // 5 bits
  *r2 = instruction >> 16 & ((1 << 5) - 1); // 5 bits
  *r3 = instruction >> 11 & ((1 << 5) - 1); // 5 bits
  *funct= instruction  & ((1 << 6) - 1); //6 bits
  *offset = instruction & ((1 << 5) - 1); // 16bits
  *jsec = instruction  & ((1 << 26) - 1); // 26 bits
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    switch (op) {
        // We might need to add more cases here and change control values
        case '000':
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        default :
            return 1;
    }
  return 0;
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
  *extended_value = offset & 0x0000FFFF;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  if(ALUSrc == 1)
    data2 = extended_value;
  if(ALUOp == 7){
    switch(funct){
      case 4:
        ALUOp = 6;
        break;
      case 32:
        ALUOp = 0;
        break;
      case 34:
        ALUOp = 1;
        break; 
      case 36:
          ALUOp = 4;
          break;
      case 37:
          ALUOp = 5;
          break;
      case 39:
          ALUOp = 7;
          break;
      case 42:
          ALUOp = 2;
          break;
      case 43:
          ALUOp = 3;
          break;
      default:
          return 1;
  }
  ALU(data1, data2, ALUOp, ALUresult, Zero);
  } else
      ALU(data1, data2, ALUOp, ALUresult, Zero);
      return 0;
  
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  if(MemWrite == 1){
    if((ALUresult % 4) == 0) Mem[ALUresult >> 2] = data2; else return 1;
  }

  if(MemRead == 1){
    if((ALUresult % 4) == 0) *memdata = Mem[ALUresult >> 2]; else return 1;
  }

  return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  if(MemtoReg == 1 && RegDst == 0 && RegWrite == 1){
      Reg[r2] = memdata;
  }
  else if(MemtoReg == 0 && RegDst == 1 &&  RegWrite == 1){
      Reg[r3] = ALUresult;
  }
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
