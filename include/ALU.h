//ALU*****************************************************************************************
byte ADD(Special_Function_Register*, word, word);
byte AND(Special_Function_Register *Gen_Use_Register, word W1, word W2);
byte IOR(Special_Function_Register *Gen_Use_Register, word W1, word W2);
byte XOR(Special_Function_Register *Gen_Use_Register, word W1, word W2);
byte DEC(Special_Function_Register *Gen_Use_Register, word W1);
byte INC(Special_Function_Register *Gen_Use_Register, word W1);
byte COMP(Special_Function_Register *Gen_Use_Register, word W1);
