#include "eapol_fun.h"



int main()
{
    UCHAR szMac[MAC_SIZE];
    USHORT usIndex = 0;
    
    InitEapHashTable();
    
	InsertEapHashNode(szMac, &usIndex);

    DeleteEapHashNode(usIndex);
    return 0;
}