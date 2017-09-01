#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"

int main()
{	
	
	word TestProgram[] = {0x0103,0x3001,0x00FF,0x07FF,0x0000,0x2808};

	Memalloc_DLL();
	InitializeReg_DLL();
	
	/*
	for(int cnt=0;cnt<8;cnt++){
		RegisterDisplay(&PIC16F73, &STATUS, cnt, STATUS_REG);
	}
	*/
	
	//Load Program into memory
	for(int PCcnt = 5;PCcnt<11;PCcnt++){
		Load_ProgramMEM_DLL(PCcnt, TestProgram[PCcnt-5]);
	}
	
	//Load the PC couter----------------------------------------------
	PIC16F73.PC = (RegisterRead(&PIC16F73, &PCLATH, PCLATH_REG) * 1000) + RegisterRead(&PIC16F73, &PCL, PCL_REG);
	
	//Emulation Core--------------------------------------------------
	for (int CNT=0;CNT<10;CNT++){
	
		printf("\nPC   = %x\n",PIC16F73.PC);
	
		EmulatorCore_DLL();
		
		printf("\n");
		printf("f   = 0x%02x\n",PIC16F73.Data_Memory[Data_Memory_Address(&PIC16F73,&STATUS)]);
		printf("W   = 0x%02x\n",PIC16F73.W);
		printf("C   = %x\n",STATUS.BIT_0);
		printf("DC  = %x\n",STATUS.BIT_1);
		printf("Z   = %x\n",STATUS.BIT_2);
		printf("************************");
	
	}
	//------------------------------------------------------------------
	
	//Free memory for cleanup
	Memcleanup_DLL();
	
	return 0;
}
