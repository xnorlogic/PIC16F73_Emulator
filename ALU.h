/*
ALU--------------------------------		
Arithmetic		Logic		control
ADDWF 0x07		ANDWF 0x05	GOTO 0x28
ADDLW 0x3E		ANDLW 0x39	CALL 0x20
SUBWF 0x02		IORWF 0x04	RETURN 0x00
SUBLW 0x3C		XORWF 0x06	SLEEP 0x00
DECF 0x03		IORLW 0x38	CLRWDT 0x00
DECFSZ 0x0B		XORLW 0x3A	
INCF 0x0A		SWAPF 0x0E	
INCFSZ 0x0F		BCF 0x010	
			BSF 0x014	
			BTFSC 0x018	
			BTFSS 0x01C	
			RLF 0x0D	
			RRF 0x0C	
			CLRF 0x01	
			CLRW 0x01	
			MOVF 0x08	
			MOVWF 0x00	
			MOVLW 0x30	
			RETLW 0x34	
			RETFIE 0x00	
			COMF 0x09	
			NOP 0x00	
-----------------------------------
/*
********************************************************
ADDWF

	Status affected:---
	byte Z			:1;
	byte DC			:1;
	byte C			:1;
	-------------------
	
*/
byte ADDWF(struct PIC16F73 *Emulator, word W1, word W2){

	word W3;
	W3 = W1 + W2;
	
	if(((W3 >> 8) & 1) == 1) Emulator->C = 1;
	else Emulator->C = 0;
	
	if(((W3 >> 4) & 1) == 1) Emulator->DC = 1;
	else Emulator->DC = 0;
	
	if(W3 == 0) Emulator->Z = 1;
	else Emulator->Z = 0;

	return W3;
}
//*******************************************************
/*
********************************************************
ANDWF

	Status affected:---
	byte Z			:1;
	-------------------
	
*/
byte ADDLW(struct PIC16F73 *Emulator, word W1, word W2){
  //CODE here
}
//*******************************************************
