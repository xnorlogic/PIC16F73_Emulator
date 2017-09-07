typedef unsigned char	BOOL;
typedef unsigned char	T_UBYTE;
typedef signed char		T_SBYTE;
typedef unsigned short 	T_UWORD;
typedef signed short	T_SWORD;
typedef unsigned int 	T_ULONG;
typedef signed int		T_SLONG;

typedef enum
{
	FALSE
	,TRUE
}TF;

#define MAX_DATA_LENGHT 0x10
#define MAX_PROG_LENGHT 0xFF
#define MAX_LINE_LENGHT 0xFFFF

#define BASE_16 16

#define READ_ONLY "r"

typedef struct
{
	T_UBYTE uyDataLenght;
	T_UWORD uwMemAddr;
	T_UBYTE uySeparator;
	T_UBYTE uyChecksum;
	TF      isValid;
	T_UBYTE uyaData[MAX_DATA_LENGHT];
}sProgram;

extern void vHEXParseInit(void);
extern void vLoadProgram(char * programName);
extern void vValidateProgram();