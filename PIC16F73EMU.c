#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"

int main()
{	
	
	//This is the program manual extract from the HEX in Intel format**********************************
	//sample ASM of this HEX is in the repository for reference****************************************
	
	//This array hold the memory location of the starting location in memory of a specific program byte
	byte MemLocation[] = {0x00,0x02,0x08,0x0a,0x10,0x20};
	//This array holds the number of bytes in corresponding program line
	byte NumOfBytes[]  = {0x02,0x04,0x02,0x06,0x08,0x0c};
	//This array hold the contents of the physical memory in the PIC device
	byte PhysicMem[]   = {0x05,0x28,0x00,0x34,0x00,0x34,0x10,0x28,0x83,0x16,0x86,0x01,0x83,0x12,0x86,
			      0x01,0x03,0x01,0x00,0x00,0x10,0x28,0x03,0x01,0x01,0x30,0x86,0x00,0x86,0x07,
			      0x00,0x00,0x13,0x28};
	
	//*************************************************************************************************
	
	int SIZE =  sizeof(MemLocation); 	//Size of the program
	byte NoB;				//Number of bytes in the program line
	byte MemLoc;				//Memory location of a program byte
	
	//Allocate the Memory for the Emulator
	Memalloc_DLL();
	
	//Initialize the registers
	InitializeReg_DLL();
	
	//Load Program into memory
	LoadProgram_DLL(SIZE,MemLoc,NoB);
		
	//Load the PC couter
	PIC16F73.PC = (RegisterRead(&PIC16F73, &PCLATH, PCLATH_REG) * 1000) + RegisterRead(&PIC16F73, &PCL, PCL_REG);
	
	//Emulation Core--------------------------------------------------
	while(RegisterRead(PORT_B_REG_Access()<0xff){
	
		EmulatorCore_DLL();
		
		printf("PortB    = %x\n",PORT_B_REG_Access());
		
		/*
		printf("\n");
		printf("f   = 0x%02x\n",PIC16F73.Data_Memory[Data_Memory_Address(&PIC16F73,&STATUS)]);
		printf("W   = 0x%02x\n",PIC16F73.W);
		printf("C   = %x\n",STATUS.BIT_0);
		printf("DC  = %x\n",STATUS.BIT_1);
		printf("Z   = %x\n",STATUS.BIT_2);
		printf("************************");
		*/
	
	}
	//------------------------------------------------------------------
	
	printf("\n---Registry summary---\n");
	//Using export function for PotB
	printf("PortB     = %x\n",PORT_B_REG_Access());
	
	printf("STATUS    = %x\n",RegisterRead(&PIC16F73, &STATUS, STATUS_REG));
	printf("PCL       = %x\n",RegisterRead(&PIC16F73, &PCL, PCL_REG));
	printf("PCLATH    = %x\n",RegisterRead(&PIC16F73, &PCLATH, PCLATH_REG));
	
	//Free memory for cleanup
	Memcleanup_DLL();
	
	return 0;
}
