


#include <stdio.h>
#include "N_NumFind1.h"


int main()
{
	ULONG ulNum = 0;
	ULONG ulOneIsFound;

	ulNum = Input_NumFind();

	(void)N_NumFindOne(ulNum, &ulOneIsFound);
	
	(void)Output_NumFind(ulOneIsFound);

	return OK;
}
		
