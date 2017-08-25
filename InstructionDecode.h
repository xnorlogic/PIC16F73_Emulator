#include "ALU.h"

//Instruction Decode---------------------------------------------------
/*
	BYTE oriented
	ADDWF 	0x07
	ANDWF 	0x05
	CLRF 	0x01
	CLRW 	0x01
	COMF 	0x09
	DECF 	0x03
	DECFSZ 	0x0B
	INCF 	0x0A
	INCFSZ 	0x0F
	IORWF 	0x04
	MOVF 	0x08
	MOVWF 	0x00
	NOP 	0x00
	RLF 	0x0D
	RRF 	0x0C
	SUBWF 	0x02
	SWAPF 	0x0E
	XORWF 	0x06

	BIT oriented
	BCF 	0x10
	BSF 	0x14
	BTFSC 	0x18
	BTFSS 	0x1C

	LITERAL and CONTROL
	ADDLW 	0x3E
	ANDLW 	0x39
	CALL 	0x20
	CLRWDT 	0x00
	GOTO 	0x28
	IORLW 	0x38
	MOVLW 	0x30
	RETFIE 	0x00
	RETLW 	0x34
	RETURN 	0x00
	SLEEP 	0x00
	SUBLW 	0x3C
	XORLW 	0x3A
*/
word Instruction_Decode (struct PIC16F73 *Emulator, byte OPCODE, word ProgCNT){
	word PC =ProgCNT;
	switch(OPCODE){
		case 0x30: //MOVLW 
			Emulator->W = Emulator->k;
			PC++; //One cycle operation
		break;
		
		case 0x00: //MOVWF 
			Emulator->Data_Memory[Data_Memory_Address(Emulator)] = Emulator->W ;
			PC++; //One cycle operation
		break;
		
		case 0x07: //ADDWF
			if(Emulator->d == 1){
				Emulator->Data_Memory[Data_Memory_Address(Emulator)] = ADDWF(Emulator,(word)Emulator->W, (word)Emulator->Data_Memory[Data_Memory_Address(Emulator)]);
				PC++; //One cycle operation
			}
			else{
				Emulator->W = ADDWF(Emulator,(word)Emulator->W, (word)Emulator->Data_Memory[Data_Memory_Address(Emulator)]);
				PC++; //One cycle operation
			}
		break;
	}
	return PC;
}
