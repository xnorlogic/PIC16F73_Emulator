#include <stdio.h>
#include <stdlib.h>
#include "PIC16F73.h"
#include "ALU.h"

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

byte AND(Special_Function_Register *Gen_Use_Register, word W1, word W2){

	word W3;
	W3 = W1 & W2;

	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;

	return W3;
}

byte IOR(Special_Function_Register *Gen_Use_Register, word W1, word W2){

	word W3;
	W3 = W1 | W2;

	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;

	return W3;
}

byte XOR(Special_Function_Register *Gen_Use_Register, word W1, word W2){

	word W3;
	W3 = W1 ^ W2;

	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;

	return W3;
}

byte DEC(Special_Function_Register *Gen_Use_Register, word W1){
	
	word W3;
	W3 = W1 - 0x01;
	
	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;
	
	return W3;
}

byte INC(Special_Function_Register *Gen_Use_Register, word W1){

	word W3;
	W3 = W1 + 0x01;
	
	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;
	
	return W3;
}

byte COMP(Special_Function_Register *Gen_Use_Register, word W1){

	word W3;
	W3 = ~ W1;
	
	if(W3 == 0) Gen_Use_Register->BIT_2 = 1;
	else Gen_Use_Register->BIT_2 = 0;
	
	return W3;
}
