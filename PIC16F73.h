/*
STATUS Register implemented 8/23/2017 ... AndyR.


*/
#include <stdio.h>
#include <stdlib.h>

#define BANK_ZERO_GENPURPOSE 512
#define PROGRAM_MEMORY 4095
#define STATUS_REG 0x03

typedef unsigned char byte;  //8  bits
typedef unsigned short word; //16 bits

struct PIC16F73{
	byte f:7;   //file register --> 7  bits
	byte W;     //W    register --> 8  bits
	word PC:13; //PC   register --> 13 bits
	
	//Status Register
	byte STATUS;
	byte RP0:1;
	byte RP1:1;
	byte IRP:1;
	byte TO:1;
	byte PD:1;
	byte Z:1;
	byte DC:1;
	byte C:1;
	 
	byte* Data_Memory;
	word* Program_Memory;
};

//Memory allocation to emulate PIC16F73----------------------------------------
void Memory_Allocation(struct PIC16F73 *Emulator){
	//allocate the emulated data memory space of the PIC16F73 in host
	Emulator->Data_Memory = (byte*) malloc(BANK_ZERO_GENPURPOSE * sizeof(byte));
	//allocate the emulated program memory space of the PIC16F73 in host
	Emulator->Program_Memory = (word*) malloc(PROGRAM_MEMORY * sizeof(word));
	//-------------------------------------------------------------------------
}

void Memory_Free(struct PIC16F73 *Emulator){
	//Free Memory
	free(Emulator->Data_Memory);
	free(Emulator->Program_Memory);
}

byte Carry (struct PIC16F73 *Emulator){
	return (Emulator->STATUS >> 0) & 1;
}

byte Digit_Carry (struct PIC16F73 *Emulator){
	return (Emulator->STATUS >> 1) & 1;
}

byte Zero_bit (struct PIC16F73 *Emulator){
	return (Emulator->STATUS >> 2) & 1;
}

byte Power_Down_bit (struct PIC16F73 *Emulator){
	return (Emulator->STATUS >> 3) & 1;
}

byte Time_Out_bit (struct PIC16F73 *Emulator){
	return (Emulator->STATUS >> 4) & 1;
}

byte Bank_Select(struct PIC16F73 *Emulator){
	return (((Emulator->STATUS >> 5) & 1) * 1) + (((Emulator->STATUS >> 6) & 1) * 2);
}

byte Bank_Select_IRP(struct PIC16F73 *Emulator){
	return (Emulator->STATUS >> 7) & 1;
}

void REG_STATUS_DISP(struct PIC16F73 *Emulator){
	printf("--STATUS REGISTER------------\n");
	printf("Carry bit                = %x\n",Carry(Emulator));
	printf("Digital Carry bit        = %x\n",Digit_Carry(Emulator));
	printf("Zero bit                 = %x\n",Zero_bit(Emulator));
	printf("Power Down bit           = %x\n",Power_Down_bit(Emulator));
	printf("Time out bit             = %x\n",Time_Out_bit(Emulator));
	printf("Bank Select              = %x\n",Bank_Select(Emulator));
	printf("Bank Select IRP          = %x\n",Bank_Select_IRP(Emulator));
	printf("-----------------------------\n");
}

void REG_STATUS_WRT(struct PIC16F73 *Emulator){
	Emulator->STATUS = (Emulator->C * 1) + (Emulator->DC * 2) + (Emulator->Z * 4) + (Emulator->PD * 8) + (Emulator->TO * 16) + (Emulator->RP0 * 32) + (Emulator->RP1 * 64) + (Emulator->IRP * 128);
}

byte REG_STATUS_RD(struct PIC16F73 *Emulator){
	return Emulator->STATUS;
}

word Data_Memory_Address(struct PIC16F73 *Emulator){
	
	word ADDRESS;
	//Direct Addressing
	ADDRESS = (((Emulator->STATUS >> 5) & 1) * 128) + (((Emulator->STATUS >> 6) & 1) * 256) + Emulator->f;
	
	//Indirect Addressing
	//Implement code for indirect addressing here
	
	return ADDRESS;
}
