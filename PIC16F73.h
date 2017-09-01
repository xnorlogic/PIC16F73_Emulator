#ifndef _PIC16F73_H

#define DATA_MEMORY 	512
#define PROGRAM_MEMORY 	4095

//Special function REGISTERS Address
#define TMR0_REG 		0x01
#define PCL_REG 		0x02
#define STATUS_REG 		0x03
#define FSR_REG 		0x04
#define PORTA_REG 		0x05
#define PORTB_REG 		0x06
#define PORTC_REG 		0x07
#define PCLATH_REG 		0x0A
#define INTCON_REG 		0x0B
#define PIR1_REG 		0x0C
#define PIR2_REG 		0x0D
#define TMR1L_REG 		0x0E
#define TMR1H_REG 		0x0F
#define T1CON_REG 		0x10
#define TMR2_REG 		0x11
#define T2CON_REG 		0x12
#define SSPBUF_REG 		0x13
#define SSPCON_REG 		0x14
#define CCPR1L_REG 		0x15
#define CCPR1H_REG 		0x16
#define CCP1CON_REG 	0x17
#define RCSTA_REG 		0x18
#define TXREG_REG 		0x19
#define RCREG_REG 		0x1A
#define CCPR2L_REG 		0x1B
#define CCPR2H_REG 		0x1C
#define CCP2CON_REG 	0x1D
#define ADRES_REG 		0x1E
#define ADCON0_REG 		0x1F

typedef unsigned char byte;  //8  bits

typedef unsigned short word; //16 bits

typedef struct{
	
	byte f		:7;		//file register --> 7  bits
	word PC		:13;	//PC   register --> 13 bits
	byte OPCODE	:6;		//opcode        --> 6 bits
	byte d		:1;
	byte k;				//k    register --> 8 bits
	byte W;     		//W    register --> 8  bits
	 
	byte* Data_Memory;
	word* Program_Memory;
	
}Emulator;

typedef struct{
	
	byte BIT_0:1;
	byte BIT_1:1;
	byte BIT_2:1;
	byte BIT_3:1;
	byte BIT_4:1;
	byte BIT_5:1;
	byte BIT_6:1;
	byte BIT_7:1;
	
}Special_Function_Register;

extern Emulator PIC16F73;
extern Special_Function_Register STATUS;

extern Special_Function_Register PCL;
extern Special_Function_Register PCLATH; 

extern Special_Function_Register PORTB;
/*
Define other registers
...
...
*/

//Register control-------------------------------
word Data_Memory_Address(Emulator*, Special_Function_Register*);
void RegisterWrite(Emulator*, Special_Function_Register*, byte);
byte RegisterRead(Emulator *, Special_Function_Register *, byte);
void RegisterDisplay(Emulator*, Special_Function_Register*, byte , byte);
void InitRegister(Special_Function_Register*, byte, byte, byte, byte, byte, byte, byte, byte);

//Memory alocation-------------------------------
void Memory_Allocation(Emulator*);
void Memory_Free(Emulator*);
//-----------------------------------------------

//ALU--------------------------------------------
byte ADD(Special_Function_Register*, word, word);
//byte SUB(Special_Function_Register*, word, word);


//Instruction decode for PIC16F7X family---------
word Instruction_Decode (Emulator*, byte, word);

//Access points----------------------------------
byte PORT_B_REG_Access();
void Memalloc_DLL();
void Memcleanup_DLL();
void Load_ProgramMEM_DLL(word PC, word Instruction);
void EmulatorCore_DLL();
void InitializeReg_DLL();
//-----------------------------------------------

/*
byte Bank_Select(struct PIC16F73 *Emulator){
	return (((Emulator->STATUS >> 5) & 1) * 1) + (((Emulator->STATUS >> 6) & 1) * 2);
}
*/

#endif
