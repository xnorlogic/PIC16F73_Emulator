#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"

Emulator PIC16F73;
Special_Function_Register STATUS;

word Data_Memory_Address(Emulator *PIC16F7x, Special_Function_Register *Gen_Use_Register){
	
	word ADDRESS;
	//Direct Addressing
	ADDRESS = (((Gen_Use_Register->BIT_5 >> 5) & 1) * 128) + (((Gen_Use_Register->BIT_6 >> 6) & 1) * 256) + PIC16F7x->f;
	
	//Indirect Addressing
	//Implement code for indirect addressing here
	
	return ADDRESS;
}

void RegisterWrite(Emulator *PIC16F7x, Special_Function_Register *Gen_Use_Register, byte address){
	
	byte REGISTER;	
	
	REGISTER	= 	(Gen_Use_Register->BIT_0 * 1) + 
					(Gen_Use_Register->BIT_1 * 2) + 
					(Gen_Use_Register->BIT_2 * 4) + 
					(Gen_Use_Register->BIT_3 * 8) + 
					(Gen_Use_Register->BIT_4 * 16) + 
					(Gen_Use_Register->BIT_5 * 32) + 
					(Gen_Use_Register->BIT_6 * 64) + 
					(Gen_Use_Register->BIT_7 * 128);
					
	PIC16F7x->f = address;
	PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = REGISTER;
					
}

void RegisterDisplay(Emulator *PIC16F7x, Special_Function_Register *Gen_Use_Register, byte BIT, byte address){
	
	switch (BIT){
		case 0: printf("BIT_0 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_0); break;
		case 1: printf("BIT_1 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_1); break;
		case 2: printf("BIT_2 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_2); break;
		case 3: printf("BIT_3 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_3); break;
		case 4: printf("BIT_4 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_4); break;
		case 5: printf("BIT_5 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_5); break;
		case 6: printf("BIT_6 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_6); break;
		case 7: printf("BIT_7 of 0x%02x register= %x\n", address, Gen_Use_Register->BIT_7); break;
		default: printf("no valid bit");
	}

	printf("full 0x%02x register    = 0x%02x\n", address, PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)]);
}

void InitRegister(Special_Function_Register *Gen_Use_Register, byte b0, byte b1,byte b2,byte b3,byte b4,byte b5,byte b6,byte b7){
	
	Gen_Use_Register->BIT_0 = b0;
	Gen_Use_Register->BIT_1 = b1;
	Gen_Use_Register->BIT_2 = b2;
	Gen_Use_Register->BIT_3 = b3;
	Gen_Use_Register->BIT_4 = b4;
	Gen_Use_Register->BIT_5 = b5;
	Gen_Use_Register->BIT_6 = b6;
	Gen_Use_Register->BIT_7 = b7;
	
}

//Memory allocation to emulate PIC16F73----------------------------------------
void Memory_Allocation(Emulator *PIC16F7x){
	//allocate the emulated data memory space of the PIC16F73 in host
	PIC16F7x->Data_Memory = (byte*) malloc(DATA_MEMORY * sizeof(byte));
	//allocate the emulated program memory space of the PIC16F73 in host
	PIC16F7x->Program_Memory = (word*) malloc(PROGRAM_MEMORY * sizeof(word));
	//-------------------------------------------------------------------------
}

void Memory_Free(Emulator *PIC16F7x){
	//Free Memory
	free(PIC16F7x->Data_Memory);
	free(PIC16F7x->Program_Memory);
}

byte ADD(Special_Function_Register *Gen_Use_Register, word W1, word W2){

	word W3;
	W3 = W1 + W2;
	
	if(((W3 >> 8) & 1) == 1) Gen_Use_Register->BIT_0 = 1;
	else Gen_Use_Register->BIT_0 = 0;
	
	if(((W3 >> 4) & 1) == 1) Gen_Use_Register->BIT_1 = 1;
	else Gen_Use_Register->BIT_1 = 0;
	
	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;

	return W3;
}

word Instruction_Decode (Emulator *PIC16F7x, byte OPCODE, word ProgCNT){
	
	word PC = ProgCNT;
	
	switch(OPCODE){
		case 0x30: //MOVLW 
			PIC16F7x->W = PIC16F7x->k;
			PC++; //One cycle operation
		break;
		
		case 0x00: //MOVWF 
			PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->W ;
			PC++; //One cycle operation
		break;
		
		case 0x07: //ADDWF
			if(PIC16F7x->d == 1){
				PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = ADD(&STATUS,(word)PIC16F7x->W, (word)PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)]);
				PC++; //One cycle operation
			}
			else{
				PIC16F7x->W = ADD(&STATUS,(word)PIC16F7x->W, (word)PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)]);
				PC++; //One cycle operation
			}
		break;
	}
	return PC;
}

/*
byte Bank_Select(struct PIC16F73 *Emulator){
	return (((Emulator->STATUS >> 5) & 1) * 1) + (((Emulator->STATUS >> 6) & 1) * 2);
}
*/