#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"
#include "ALU.h"

Emulator PIC16F73;
Special_Function_Register STATUS;
Special_Function_Register PCL;
Special_Function_Register PCLATH; 
Special_Function_Register PORTB;

Special_Function_Register SFR_TMR0_REG;
//extern Special_Function_Register SFR_PCL_REG;
//extern Special_Function_Register SFR_STATUS_REG;
Special_Function_Register SFR_FSR_REG;
Special_Function_Register SFR_PORTA_REG;
//extern Special_Function_Register SFR_PORTB_REG;
Special_Function_Register SFR_PORTC_REG;
//extern Special_Function_Register SFR_PCLATH_REG;
Special_Function_Register SFR_INTCON_REG;
Special_Function_Register SFR_PIR1_REG;
Special_Function_Register SFR_PIR2_REG;
Special_Function_Register SFR_TMR1L_REG;
Special_Function_Register SFR_TMR1H_REG;
Special_Function_Register SFR_T1CON_REG;
Special_Function_Register SFR_TMR2_REG;
Special_Function_Register SFR_T2CON_REG;
Special_Function_Register SFR_SSPBUF_REG;
Special_Function_Register SFR_SSPCON_REG;
Special_Function_Register SFR_CCPR1L_REG;
Special_Function_Register SFR_CCPR1H_REG;
Special_Function_Register SFR_CCP1CON_REG;
Special_Function_Register SFR_RCSTA_REG;
Special_Function_Register SFR_TXREG_REG;
Special_Function_Register SFR_RCREG_REG;
Special_Function_Register SFR_CCPR2L_REG;
Special_Function_Register SFR_CCPR2H_REG;
Special_Function_Register SFR_CCP2CON_REG;
Special_Function_Register SFR_ADRES_REG;
Special_Function_Register SFR_ADCON0_REG;


word Data_Memory_Address(Emulator *PIC16F7x, Special_Function_Register *Gen_Use_Register){
	
	word ADDRESS;
	//Direct Addressing
	ADDRESS = (Gen_Use_Register->BIT_5 * 128) + (Gen_Use_Register->BIT_6 * 256) + PIC16F7x->f;
	
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
	PIC16F7x->Program_Physical_Memory = (byte*) malloc(PROGRAM_PHYSICAL_MEMORY * sizeof(byte));
	//-------------------------------------------------------------------------
}

void Memory_Free(Emulator *PIC16F7x){
	//Free Memory
	free(PIC16F7x->Data_Memory);
	free(PIC16F7x->Program_Memory);
	free(PIC16F7x->Program_Physical_Memory);
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
			
		//MOVF
		case 0x08:
			
			if(PIC16F7x->d == 1){  	
				PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)];
				if (PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)] == 0) STATUS.BIT_2 = 1;
				else STATUS.BIT_2 = 0;
				PC++; //One cycle operation
			}
			else{
				PIC16F7x->W = PIC16F7x->Data_Memory[Data_Memory_Address(PIC16F7x,&STATUS)];
				if (PIC16F7x->W == 0) STATUS.BIT_2 = 1;
				else STATUS.BIT_2 = 0;
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

byte PORTB_REG_Access(){ return	RegisterRead(&PIC16F73, &PORTB, PORTB_REG);	}
byte TMR0_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_TMR0_REG,TMR0_REG); }
//byte PCL_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PCL_REG,PCL_REG); }
//byte STATUS_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_STATUS_REG,STATUS_REG); }
byte FSR_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_FSR_REG,FSR_REG); }
byte PORTA_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PORTA_REG,PORTA_REG); }
//byte PORTB_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PORTB_REG,PORTB_REG); }
byte PORTC_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PORTC_REG,PORTC_REG); }
//byte PCLATH_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PCLATH_REG,PCLATH_REG); }
byte INTCON_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_INTCON_REG,INTCON_REG); }
byte PIR1_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PIR1_REG,PIR1_REG); }
byte PIR2_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_PIR2_REG,PIR2_REG); }
byte TMR1L_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_TMR1L_REG,TMR1L_REG); }
byte TMR1H_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_TMR1H_REG,TMR1H_REG); }
byte T1CON_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_T1CON_REG,T1CON_REG); }
byte TMR2_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_TMR2_REG,TMR2_REG); }
byte T2CON_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_T2CON_REG,T2CON_REG); }
byte SSPBUF_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_SSPBUF_REG,SSPBUF_REG); }
byte SSPCON_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_SSPCON_REG,SSPCON_REG); }
byte CCPR1L_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_CCPR1L_REG,CCPR1L_REG); }
byte CCPR1H_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_CCPR1H_REG,CCPR1H_REG); }
byte CCP1CON_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_CCP1CON_REG,CCP1CON_REG); }
byte RCSTA_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_RCSTA_REG,RCSTA_REG); }
byte TXREG_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_TXREG_REG,TXREG_REG); }
byte RCREG_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_RCREG_REG,RCREG_REG); }
byte CCPR2L_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_CCPR2L_REG,CCPR2L_REG); }
byte CCPR2H_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_CCPR2H_REG,CCPR2H_REG); }
byte CCP2CON_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_CCP2CON_REG,CCP2CON_REG); }
byte ADRES_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_ADRES_REG,ADRES_REG); }
byte ADCON0_REG_Access() {  return RegisterRead(&PIC16F73,&SFR_ADCON0_REG,ADCON0_REG); }

void Load_ProgramMEM(word PC, byte n, byte NumOfBytes){
	
	int cycle;
	
	for(cycle = 0; cycle <= NumOfBytes/2 ;cycle++){
		PIC16F73.Program_Memory[PC] = (PIC16F73.Program_Physical_Memory[n + 1] * 0x100) + PIC16F73.Program_Physical_Memory[n];
		n = n + 2;
		PC++;
	}
	
}

void LoadProgram_DLL(int SIZE, byte MemLocation[], byte NumOfBytes[], byte PhysicMem[]){

	byte COUNTER = 0;
	int cycle, aloc;
	
	for(cycle=0;cycle<SIZE;cycle++){
		for (aloc =0;aloc<NumOfBytes[cycle];aloc++){

			PIC16F73.Program_Physical_Memory[MemLocation[cycle] + aloc] = PhysicMem[COUNTER];
			COUNTER++;

		}
	}

	for(cycle=0;cycle<SIZE;cycle++){
		//Load_ProgramMEM(<word: Memory address of program memory>, <byte: physical Memory address of program>, <byte: number of bytes>)
		Load_ProgramMEM(MemLocation[cycle]/2, MemLocation[cycle], NumOfBytes[cycle]);
	}

}

/*
byte Bank_Select(struct PIC16F73 *Emulator){
	return (((Emulator->STATUS >> 5) & 1) * 1) + (((Emulator->STATUS >> 6) & 1) * 2);
}
*/
