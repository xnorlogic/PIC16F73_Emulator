#include "PIC16F73.h"
#include "InstructionDecode.h"

PIC16F73 Emulator;

void Registe_Status_Proveout(){
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
}

int main()
{	
	//Test Program----------------------------------------------------
	//Load Program into program memory
	Emulator.Program_Memory[0x0005] = 0x300a; 	//MOVLW 0x01
	Emulator.Program_Memory[0x0006] = 0x00A1; 	//MOVWF 0x21
	Emulator.Program_Memory[0x0007] = 0x07A1; 	//ADDWF 0x21, f
	Emulator.PC = 0x0005;			        //Send program
							//counter to the 0x05
							//program memory start
	//----------------------------------------------------------------
	
	for (int CNT=0;CNT<3;CNT++){
	
		Emulator.OPCODE = (Emulator.Program_Memory[Emulator.PC] >> 8) & 255;
		Emulator.d 		= (Emulator.Program_Memory[Emulator.PC] >> 7) & 1;
		Emulator.f 		= Emulator.Program_Memory[Emulator.PC] & 127;
		Emulator.k 		= Emulator.Program_Memory[Emulator.PC] & 255;
		
		Emulator.PC = Instruction_Decode(&Emulator, Emulator.OPCODE, Emulator.PC);
		
		printf("\n");
		printf("f   = 0x%02x\n",Emulator.Data_Memory[Data_Memory_Address(&Emulator)]);
		printf("W   = 0x%02x\n",Emulator.W);
		printf("C   = %x\n",Emulator.C);
		printf("DC  = %x\n",Emulator.DC);
		printf("Z   = %x\n",Emulator.Z);
	}
	
	//Free memory for cleanup
	Memory_Free(&Emulator);
	
	return 0;
}
