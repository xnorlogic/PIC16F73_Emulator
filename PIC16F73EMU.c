#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"

int main()
{	
	//Set up memory block
	Memory_Allocation(&PIC16F73);
	
	//initializen Status Register 
	InitRegister(&STATUS,0,0,0,1,1,0,0,0);
	
	//Send data to Status Register in emulated mem location of PIC16F73
	RegisterWrite(&PIC16F73, &STATUS, STATUS_REG);
	
	/*
	for(int cnt=0;cnt<8;cnt++){
		RegisterDisplay(&PIC16F73, &STATUS, cnt, STATUS_REG);
	}
	*/
	
	//Test Program----------------------------------------------------
	//Load Program into program memory
	PIC16F73.Program_Memory[0x0005] = 0x0103; 	//CLRW
	PIC16F73.Program_Memory[0x0006] = 0x3001; 	//MOVLW 0x01
	PIC16F73.Program_Memory[0x0007] = 0x00FF; 	//MOVWF 0x21
	PIC16F73.Program_Memory[0x0008] = 0x07FF; 	//ADDWF 0x21, f
	PIC16F73.Program_Memory[0x0009] = 0x0000; 	//NOP
	PIC16F73.Program_Memory[0x000a] = 0x2808; 	//GOTO 0x08
	PIC16F73.PC = 0x0005;			        	//Send program
												//counter to the 0x05
												//program memory start
	//----------------------------------------------------------------
	
	for (int CNT=0;CNT<3;CNT++){
	
		PIC16F73.OPCODE = (PIC16F73.Program_Memory[PIC16F73.PC] >> 8) & 255;
		PIC16F73.d 		= (PIC16F73.Program_Memory[PIC16F73.PC] >> 7) & 1;
		PIC16F73.f 		=  PIC16F73.Program_Memory[PIC16F73.PC] & 127;
		PIC16F73.k 		=  PIC16F73.Program_Memory[PIC16F73.PC] & 255;
		
		PIC16F73.PC = Instruction_Decode(&PIC16F73, PIC16F73.OPCODE, PIC16F73.PC);
		
		printf("\n");
		printf("f   = 0x%02x\n",PIC16F73.Data_Memory[Data_Memory_Address(&PIC16F73,&STATUS)]);
		printf("W   = 0x%02x\n",PIC16F73.W);
		printf("C   = %x\n",STATUS.BIT_0);
		printf("DC  = %x\n",STATUS.BIT_1);
		printf("Z   = %x\n",STATUS.BIT_2);
		
	}
	
	//Free memory for cleanup
	Memory_Free(&PIC16F73);
	
	return 0;
}
