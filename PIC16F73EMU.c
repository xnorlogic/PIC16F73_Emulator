#include "PIC16F73.h"
#include "ALU.h"

PIC16F73 Emulator;

int main()
{
	//file location of PIC16F73. 7 bits to access any memory location
	Emulator.f = 0x7F;
	
	//Memory allocation to emulate PIC16F73
	Memory_Allocation(&Emulator);
	
	//Registe Status Prove out------------------------------------------------------
	//Bank Select-------------------------------------------------------------------
	Emulator.RP0 = 0;
	Emulator.RP1 = 0;
	//------------------------------------------------------------------------------
	Emulator.IRP = 0; //indirect addressing
	Emulator.TO  = 1; //Time out
	Emulator.PD  = 1; //power down
	Emulator.Z   = 0; //Zero bit
	Emulator.DC  = 0; //digit carry
	Emulator.C   = 0; //carry
	
	//write Status Register
	REG_STATUS_WRT(&Emulator);
	//Display status register
	REG_STATUS_DISP(&Emulator);
	//point to Status Register
	Emulator.f = STATUS_REG;
	//Allocate Status Register in its appropiate PIC16F73 memory address
	Emulator.Data_Memory[Data_Memory_Address(&Emulator)] = REG_STATUS_RD(&Emulator);
	//------------------------------------------------------------------------------
	
	//initialize a specific memory location using the file location
	//Emulator.Data_Memory[Bank_Select(&Emulator) + Emulator.f] = (byte)0x00;
	
	printf("PIC16F73 BankSelect      = %x\n",Bank_Select(&Emulator));
	printf("PIC16F73 Memory location = %x\n",Data_Memory_Address(&Emulator));
	printf("Host Memory location     = %x\n",&Emulator.Data_Memory[Data_Memory_Address(&Emulator)]);
	printf("value                    = %x\n",Emulator.Data_Memory[Data_Memory_Address(&Emulator)]);
	
	//Free memory for cleanup
	Memory_Free(&Emulator);
	
	return 0;
}
