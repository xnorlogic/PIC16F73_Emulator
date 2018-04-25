//#define WINDOWS_MINGW_NET 
#define DEVICE_PIC16F73

#include "context.h"

#ifdef DEVICE_PIC16F73
#define DATA_MEMORY 			512
#define PROGRAM_MEMORY 			4095
#define PROGRAM_PHYSICAL_MEMORY 	8190

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
#define CCP1CON_REG 		0x17
#define RCSTA_REG 		0x18
#define TXREG_REG 		0x19
#define RCREG_REG 		0x1A
#define CCPR2L_REG 		0x1B
#define CCPR2H_REG 		0x1C
#define CCP2CON_REG 		0x1D
#define ADRES_REG 		0x1E
#define ADCON0_REG 		0x1F
#endif

typedef struct{
	
	byte f		:7;	//file register --> 7  bits
	word PC		:13;	//PC   register --> 13 bits
	byte OPCODE	:6;	//opcode        --> 6 bits
	byte d		:1;
	byte b		:3;	//b    		--> 3 bits
	byte k;			//k    register --> 8 bits
	byte W;     		//W    register --> 8  bits
	 
	byte* Data_Memory;
	word* Program_Memory;
	byte* Program_Physical_Memory;
	
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

//extern Special_Function_Register SFR_STATUS_REG;
extern Special_Function_Register STATUS;
//extern Special_Function_Register SFR_PCL_REG;
extern Special_Function_Register PCL;
//extern Special_Function_Register SFR_PCLATH_REG;
extern Special_Function_Register PCLATH; 
//extern Special_Function_Register SFR_PORTB_REG;
extern Special_Function_Register PORTB;

extern Special_Function_Register SFR_TMR0_REG;
extern Special_Function_Register SFR_FSR_REG;
extern Special_Function_Register SFR_PORTA_REG;
extern Special_Function_Register SFR_PORTC_REG;
extern Special_Function_Register SFR_INTCON_REG;
extern Special_Function_Register SFR_PIR1_REG;
extern Special_Function_Register SFR_PIR2_REG;
extern Special_Function_Register SFR_TMR1L_REG;
extern Special_Function_Register SFR_TMR1H_REG;
extern Special_Function_Register SFR_T1CON_REG;
extern Special_Function_Register SFR_TMR2_REG;
extern Special_Function_Register SFR_T2CON_REG;
extern Special_Function_Register SFR_SSPBUF_REG;
extern Special_Function_Register SFR_SSPCON_REG;
extern Special_Function_Register SFR_CCPR1L_REG;
extern Special_Function_Register SFR_CCPR1H_REG;
extern Special_Function_Register SFR_CCP1CON_REG;
extern Special_Function_Register SFR_RCSTA_REG;
extern Special_Function_Register SFR_TXREG_REG;
extern Special_Function_Register SFR_RCREG_REG;
extern Special_Function_Register SFR_CCPR2L_REG;
extern Special_Function_Register SFR_CCPR2H_REG;
extern Special_Function_Register SFR_CCP2CON_REG;
extern Special_Function_Register SFR_ADRES_REG;
extern Special_Function_Register SFR_ADCON0_REG;

//Register control****************************************************************************
word Data_Memory_Address(Emulator*, Special_Function_Register*);
void RegisterWrite(Emulator*, Special_Function_Register*, byte);
byte RegisterRead(Emulator *, Special_Function_Register *, byte);
void RegisterDisplay(Emulator*, Special_Function_Register*, byte , byte);
void InitRegister(Special_Function_Register*, byte, byte, byte, byte, byte, byte, byte, byte);
//Memory alocation****************************************************************************
void Memory_Allocation(Emulator*);
void Memory_Free(Emulator*);
//Load into program memory********************************************************************
void Load_ProgramMEM(word, byte, byte);
//Instruction decode for PIC16F7X family******************************************************
word Instruction_Decode (Emulator*, byte, word);

//Access points*******************************************************************************
//access points to the emulator to develop emulator shell

#ifdef WINDOWS_MINGW_NET

//declaration for exporting to windows use... with export decoration
__declspec(dllexport) extern byte TMR0_REG_Access();
__declspec(dllexport) extern byte PCL_REG_Access();
__declspec(dllexport) extern byte STATUS_REG_Access();
__declspec(dllexport) extern byte FSR_REG_Access();
__declspec(dllexport) extern byte PORTA_REG_Access();
__declspec(dllexport) extern byte PORTB_REG_Access();
__declspec(dllexport) extern byte PORTC_REG_Access();
__declspec(dllexport) extern byte PCLATH_REG_Access();
__declspec(dllexport) extern byte INTCON_REG_Access();
__declspec(dllexport) extern byte PIR1_REG_Access();
__declspec(dllexport) extern byte PIR2_REG_Access();
__declspec(dllexport) extern byte TMR1L_REG_Access();
__declspec(dllexport) extern byte TMR1H_REG_Access();
__declspec(dllexport) extern byte T1CON_REG_Access();
__declspec(dllexport) extern byte TMR2_REG_Access();
__declspec(dllexport) extern byte T2CON_REG_Access();
__declspec(dllexport) extern byte SSPBUF_REG_Access();
__declspec(dllexport) extern byte SSPCON_REG_Access();
__declspec(dllexport) extern byte CCPR1L_REG_Access();
__declspec(dllexport) extern byte CCPR1H_REG_Access();
__declspec(dllexport) extern byte CCP1CON_REG_Access();
__declspec(dllexport) extern byte RCSTA_REG_Access();
__declspec(dllexport) extern byte TXREG_REG_Access();
__declspec(dllexport) extern byte RCREG_REG_Access();
__declspec(dllexport) extern byte CCPR2L_REG_Access();
__declspec(dllexport) extern byte CCPR2H_REG_Access();
__declspec(dllexport) extern byte CCP2CON_REG_Access();
__declspec(dllexport) extern byte ADRES_REG_Access();
__declspec(dllexport) extern byte ADCON0_REG_Access();

#else

extern void Memalloc_DLL();
extern void Memcleanup_DLL();
extern void LoadProgram_DLL(int, byte [], byte [], byte []);
extern void EmulatorCore_DLL();
extern void InitializeReg_DLL();

extern byte TMR0_REG_Access();
extern byte PCL_REG_Access();
extern byte STATUS_REG_Access();
extern byte FSR_REG_Access();
extern byte PORTA_REG_Access();
extern byte PORTB_REG_Access();
extern byte PORTC_REG_Access();
extern byte PCLATH_REG_Access();
extern byte INTCON_REG_Access();
extern byte PIR1_REG_Access();
extern byte PIR2_REG_Access();
extern byte TMR1L_REG_Access();
extern byte TMR1H_REG_Access();
extern byte T1CON_REG_Access();
extern byte TMR2_REG_Access();
extern byte T2CON_REG_Access();
extern byte SSPBUF_REG_Access();
extern byte SSPCON_REG_Access();
extern byte CCPR1L_REG_Access();
extern byte CCPR1H_REG_Access();
extern byte CCP1CON_REG_Access();
extern byte RCSTA_REG_Access();
extern byte TXREG_REG_Access();
extern byte RCREG_REG_Access();
extern byte CCPR2L_REG_Access();
extern byte CCPR2H_REG_Access();
extern byte CCP2CON_REG_Access();
extern byte ADRES_REG_Access();
extern byte ADCON0_REG_Access();

#endif
//********************************************************************************************

/*
byte Bank_Select(struct PIC16F73 *Emulator){
	return (((Emulator->STATUS >> 5) & 1) * 1) + (((Emulator->STATUS >> 6) & 1) * 2);
}
*/
