//ALU---------------------------------------------------
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
byte ADDWF(struct PIC16F73 *Emulator, word W1, word W2){
  //CODE here
}
//*******************************************************
