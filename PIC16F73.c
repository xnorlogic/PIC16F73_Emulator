#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"

Emulator PIC16F73;
Special_Function_Register STATUS;
Special_Function_Register PCL;
Special_Function_Register PCLATH; 
Special_Function_Register PORTB;


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

byte RegisterRead(Emulator *PIC16F7x, Special_Function_Register *Gen_Use_Register, byte address){
	PIC16F7x->f = address;
	return PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)];
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

//Memory allocation to emulate PIC16F73
void Memory_Allocation(Emulator *PIC16F7x){
	//allocate the emulated data memory space of the PIC16F73 in host
	PIC16F7x->Data_Memory = (byte*) malloc(DATA_MEMORY * sizeof(byte));
	//allocate the emulated program memory space of the PIC16F73 in host
	PIC16F7x->Program_Memory = (word*) malloc(PROGRAM_MEMORY * sizeof(word));
	//-------------------------------------------------------------------------
	PIC16F7x->Program_Physical_Memory = (byte*) malloc(PROGRAM_PHYSICAL_MEMORY * sizeof(byte));
	//-------------------------------------------------------------------------
}

void Memory_Free(Emulator *PIC16F7x){
	//Free Memory
	free(PIC16F7x->Data_Memory);
	free(PIC16F7x->Program_Memory);
	free(PIC16F7x->Program_Physical_Memory);
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

		//MOVWF and NOP
		case 0x00: 	
			
			if(PIC16F7x->d == 1){ //MOVWF 	
				PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->W ;
				PC++; //One cycle operation
			}
			else{
				PC++; //One cycle operation NOP
			}
			
			if(PIC16F7x->k == 0x64){ //CLRWDT
			}
			else if(PIC16F7x->k == 0x09){ //RETFIE
			}
			else if(PIC16F7x->k == 0x08){ //RETURN
			}
			else if(PIC16F7x->k == 0x63){ //SLEEP
			}
			else{
				//Nothing
			}
			
		break;

		//CLRF and CLRW
		case 0x01: //CLRF and CLRW
			if(PIC16F7x->d == 1){
				PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = 0x00;
				PC++; //One cycle operation
			}
			else{
				PIC16F7x->W = 0x00;
				PC++; //One cycle operation
			}
		break;

		//MOVLW
		case 0x30:  
			PIC16F7x->W = PIC16F7x->k;
			PC++; //One cycle operation
		break;

		//ADDWF
		case 0x07: 
			if(PIC16F7x->d == 1){ //ADDWF save result in F
				PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = ADD(&STATUS,(word)PIC16F7x->W, (word)PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)]);
				PC++; //One cycle operation
			}
			else{ //ADDWF save result in W
				PIC16F7x->W = ADD(&STATUS,(word)PIC16F7x->W, (word)PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)]);
				PC++; //One cycle operation
			}
		break;

		//GOTO
		case 0x28: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x29: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x2A: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x2B: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x2C: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x2D: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x2E: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;
		case 0x2F: PC = (((PIC16F7x->OPCODE >> 0) & 1) * 1) + (((PIC16F7x->OPCODE >> 1) & 1) * 2) + (((PIC16F7x->OPCODE >> 0) & 1) * 4) + PIC16F7x->k; break;

		//BCF
		case 0x10:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 2;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 1;
					PC++; //One cycle operation
				}
		break;
		
		case 0x11:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 8;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 4;
					PC++; //One cycle operation
				}
		break;
		
		case 0x12:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 32;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 16;
					PC++; //One cycle operation
				}
		break;
		
		case 0x13:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 128;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] ^ 64;
					PC++; //One cycle operation
				}
		break;
		
		//BSF
		case 0x14:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
						PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 2;
						PC++; //One cycle operation
					}
					else{ //ADDWF save result in W
						PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 1;
						PC++; //One cycle operation
					}
		break;
		
		case 0x15:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 8;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 4;
					PC++; //One cycle operation
				}
		break;
		
		case 0x16:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 32;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 16;
					PC++; //One cycle operation
				}
		break;
		
		case 0x17:
			if(PIC16F7x->d == 1){ //ADDWF save result in F
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 128;
					PC++; //One cycle operation
				}
				else{ //ADDWF save result in W
					PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] | 64;
					PC++; //One cycle operation
				}
			
		break;
		
		default: break;
	}

	return PC;
}

//Export functions to DLL
void Load_ProgramMEM_DLL(word PC, byte n, byte NumOfBytes){
	for(int cycle = 0; cycle <= NumOfBytes/2 ;cycle++){
		PIC16F73.Program_Memory[PC] = (PIC16F73.Program_Physical_Memory[n + 1] * 0x100) + PIC16F73.Program_Physical_Memory[n];
		n = n + 2;
		PC++;
	}
}
void EmulatorCore_DLL(){
	
		PIC16F73.OPCODE = (PIC16F73.Program_Memory[PIC16F73.PC] >> 8) & 255;
		PIC16F73.d 		= (PIC16F73.Program_Memory[PIC16F73.PC] >> 7) & 1;
		PIC16F73.f 		=  PIC16F73.Program_Memory[PIC16F73.PC] & 127;
		PIC16F73.k 		=  PIC16F73.Program_Memory[PIC16F73.PC] & 255;
		PIC16F73.b      =  PIC16F73.Program_Memory[PIC16F73.PC] >> 7 & 3;
		
		PIC16F73.PC = Instruction_Decode(&PIC16F73, PIC16F73.OPCODE, PIC16F73.PC);
		
}
void Memcleanup_DLL(){
	Memory_Free(&PIC16F73);
}
void Memalloc_DLL(){
	Memory_Allocation(&PIC16F73);
}
void InitializeReg_DLL(){
	//initializen Status Register 
	InitRegister(&STATUS,0,0,0,1,1,0,0,0);
	RegisterWrite(&PIC16F73, &STATUS, STATUS_REG);
	//initializen PCL Register
	InitRegister(&PCL,0,0,0,0,0,0,0,0);
	RegisterWrite(&PIC16F73, &PCL, PCL_REG);
	//initializen PCLATH Register
	InitRegister(&PCLATH,0,0,0,0,0,0,0,0);
	RegisterWrite(&PIC16F73, &PCLATH, PCLATH_REG);
}

//Access to PORTB
byte PORT_B_REG_Access(){
	return	RegisterRead(&PIC16F73, &PORTB, PORTB_REG);
}

/*
byte Bank_Select(struct PIC16F73 *Emulator){
	return (((Emulator->STATUS >> 5) & 1) * 1) + (((Emulator->STATUS >> 6) & 1) * 2);
}
*/
