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
	/* iterators */
	int i = 0;

	/* file pointer */
	FILE * hex = fopen(programName, READ_ONLY);

	/* local variables */
	T_UBYTE line[MAX_LINE_LENGHT];
	T_UBYTE * pyDataLenght;
	T_UBYTE * pyMemAddr;
	T_UBYTE * pyData;
	T_UBYTE * pyTempData;
	T_UBYTE * pyCheckSum;
	T_UBYTE * pySeparator;

	/* temporary line structure*/
	sProgram rsTmpLine;

	/* addres pointer used to calculate address for data*/
	T_ULONG * address;

	/* line counter */
	T_UWORD programCnt = 0;

	while (fgets(line, sizeof(line), hex))
	{
		/* allocate memory for local pointers */
		pyDataLenght = (T_UBYTE *)malloc(2);
		pyMemAddr    = (T_UBYTE *)malloc(4);
		pyCheckSum   = (T_UBYTE *)malloc(2);
		pySeparator  = (T_UBYTE *)malloc(2);
		pyData = (char *)malloc(MAX_DATA_LENGHT * 2);

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
		/* set address to scan */
		address = &pyData;
		
		/* loop throught al data */
		for (i = 0; i <= rsTmpLine.uyDataLenght; i++)
		{
			/* allocate memory for pyTempData */
			pyTempData = malloc(2);
			/* copy data from address to pyTempData cast away type */
			memcpy(pyTempData, *(T_ULONG *)address, 2);
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

	/* clear pointer */
	address = NULL;
	
	/* clear memory */
	memset(&pyDataLenght, 0, sizeof(pyDataLenght));
	memset(&pyMemAddr	, 0, sizeof(pyMemAddr   ));
	memset(&pyCheckSum	, 0, sizeof(pyCheckSum  ));
	memset(&pySeparator	, 0, sizeof(pySeparator ));
	memset(&pyTempData	, 0, sizeof(pyTempData  ));
	memset(&pyData		, 0, sizeof(pyData      ));

	/* free memory */
	free(pyDataLenght);
	free(pyMemAddr   );
	free(pyCheckSum  );
	free(pySeparator );
	free(pyTempData  );
	free(pyData      );
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
	/* calculated checksum */
	int lnCalcChecksum;

	/* iterators */
	int i = 0;
	int j = 0;

	for (i = 0; i <= MAX_PROG_LENGHT; i++)
	{
		lnCalcChecksum = rsProgram[i].uwMemAddr
			           + rsProgram[i].uyDataLenght
			           + rsProgram[i].uySeparator;

		for (j = 0; j <= rsProgram[i].uyDataLenght; j++)
		{
			lnCalcChecksum += rsProgram[i].uyaData[j];
		}

		lnCalcChecksum = (-1 * lnCalcChecksum) & 0xFF;

		if ((T_UBYTE)lnCalcChecksum == rsProgram[i].uyChecksum)
		{
			rsProgram[i].isValid = TRUE;
		}
	}
}