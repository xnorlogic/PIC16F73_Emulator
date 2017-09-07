#include <string.h>
#include <stdio.h>
#include <stdlib.h>  
#include "HEXParse.h"

/* local ram structure */
sProgram rsProgram[MAX_PROG_LENGHT];

/*
* Init function 
* called by main service
*/
void vHEXParseInit(void)
{
	memset(rsProgram, 0, sizeof(rsProgram));
}

/*
 * parse .hex file to local ram structure rsProgram
 * called by main service
 */
void vLoadProgram(char * programName)
{
	/* file pointer */
	FILE * hex = fopen(programName, READ_ONLY);

	/* local variables */
	char line[MAX_LINE_LENGHT];
	char * pyDataLenght;
	char * pyMemAddr;
	char * pyData;
	char * pyTempData;
	char * pyCheckSum;
	char * pySeparator;

	/* temporary line structure*/
	sProgram rsTmpLine;

	/* addres pointer used to calculate address for data*/
	int *address;

	/* line counter */
	T_UWORD programCnt = 0;

	while (fgets(line, sizeof(line), hex))
	{
		/* allocate memory for local pointers */
		pyDataLenght = malloc(2);
		pyMemAddr    = malloc(4);
		pyData       = malloc(MAX_DATA_LENGHT * 2);
		pyCheckSum   = malloc(2);
		pySeparator  = malloc(2);

		/* clear rsTmpLine */
		memset(&rsTmpLine, 0, sizeof(rsTmpLine));

		/* Data length in element 1 to 2 */
		memcpy(pyDataLenght, &line[1], 2);
		/* convert ASCII to T_UBYTE */
		rsTmpLine.uyDataLenght = (T_UBYTE)strtol(pyDataLenght, NULL, BASE_16);

		/* Memory addres in element 3 to 6 */
		memcpy(pyMemAddr, &line[3], 4);
		/* convert ASCII to T_UWORD */
		rsTmpLine.uwMemAddr = (T_UWORD)strtol(pyMemAddr, NULL, BASE_16);

		/* Separator between memory address and data in element 7 to 9 */
		memcpy(pySeparator, &line[7], 2);
		/* convert ASCII to T_UBYTE */
		rsTmpLine.uySeparator = (T_UBYTE)strtol(pySeparator, NULL, 16);

		/* Checksum in element 9 + variable data length to 9 + variable data length + 2 */
		memcpy(pyCheckSum, &line[9 + rsTmpLine.uyDataLenght * 2], 2);
		/* convert ASCII to T_UBYTE */
		rsTmpLine.uyChecksum = (T_UBYTE)strtol(pyCheckSum, NULL, BASE_16);

		/* Data in element 9 to variable data length * 2 */
		memcpy(pyData, &line[9], rsTmpLine.uyDataLenght * 2);
		address = &pyData;
		
		/* loop throught al data */
		for (int i = 0; i <= rsTmpLine.uyDataLenght; i++)
		{
			/* allocate memory for pyTempData */
			pyTempData = malloc(2);
			/* copy data from address to pyTempData*/
			memcpy(pyTempData, *address, 2);
			/* convert ASCII to T_UBYTE */
			rsTmpLine.uyaData[i] = (T_UBYTE)strtol(pyTempData, NULL, BASE_16);
			/* next address */
			*address += 2;
		}

		/* copy temporary item to ram structure */
		memcpy(&rsProgram[programCnt], &rsTmpLine, sizeof(rsTmpLine));

		/* increment program counter */
		programCnt++;
	}

	/* close file */
	fclose(hex);
}

/*
 * f-n validate line checksum
 *     adds all elements in one line
 *     multiplies by -1 and applies 0xFF mask
 *     compares calculated checksum with 
 *     read checksum, if match, set isValid field to TRUE
 *     else set isValid field to FALSE
 * called by main service
 */
void vValidateProgram()
{
	int lnCalcChecksum;

	for (int i = 0; i <= MAX_PROG_LENGHT; i++)
	{
		lnCalcChecksum = rsProgram[i].uwMemAddr
			           + rsProgram[i].uyDataLenght
			           + rsProgram[i].uySeparator;

		for (int j = 0; j <= rsProgram[i].uyDataLenght; j++)
		{
			lnCalcChecksum += rsProgram[i].uyaData[j];
		}

		lnCalcChecksum = (-1 * lnCalcChecksum) & 0xFF;

		if (lnCalcChecksum == rsProgram[i].uyChecksum)
		{
			rsProgram[i].isValid = TRUE;
		}
	}
}