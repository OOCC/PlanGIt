#include <stdlib.h>  
#include <ctype.h>  
  
  
  
long __cdecl atol(  
const char *nptr  
)  
{  
    int c;  
    long total;  
    int sign;  
      
      
    while ( isspace((int)(unsigned char)*nptr) )  
        ++nptr;  
      
    c = (int)(unsigned char)*nptr++;  
    sign = c;  
    if (c == ''-'' || c == ''+'')  
        c = (int)(unsigned char)*nptr++;  
      
    total = 0;  
      
    while (isdigit(c)) {  
        total = 10 * total + (c - ''0'');  
        c = (int)(unsigned char)*nptr++;  
    }  
      
    if (sign == ''-'')  
        return -total;  
    else  
        return total;  
}  
  
  
  
  
int __cdecl atoi(  
const char *nptr  
)  
{  
return (int)atol(nptr);  
}  
  
#ifndef _NO_INT64  
  
__int64 __cdecl _atoi64(  
const char *nptr  
)  
{  
int c;  
__int64 total;  
int sign;  
  
  
while ( isspace((int)(unsigned char)*nptr) )  
++nptr;  
  
c = (int)(unsigned char)*nptr++;  
sign = c;  
if (c == ''-'' || c == ''+'')  
c = (int)(unsigned char)*nptr++;  
  
total = 0;  
  
while (isdigit(c)) {  
total = 10 * total + (c - ''0'');  
c = (int)(unsigned char)*nptr++;  
}  
  
if (sign == ''-'')  
return -total;  
else  
return total;  
}  
  
#endif  
  
  
#include <msvcrt/errno.h>  
#include <msvcrt/stdlib.h>  
#include <msvcrt/internal/file.h>  
char* _itoa(int value, char* string, int radix)  
{  
    char tmp[33];  
    char* tp = tmp;  
    int i;  
    unsigned v;  
    int sign;  
    char* sp;  
  
    if (radix > 36 || radix <= 1)  
    {  
        __set_errno(EDOM);  
        return 0;  
    }  
  
    sign = (radix == 10 && value < 0);  
    if (sign)  
        v = -value;  
    else  
        v = (unsigned)value;  
    while (v || tp == tmp)  
    {  
        i = v % radix;  
        v = v / radix;  
        if (i < 10)  
            *tp++ = i+''0'';  
        else  
            *tp++ = i + ''a'' - 10;  
    }  
  
    if (string == 0)  
    string = (char*)malloc((tp-tmp)+sign+1);  
    sp = string;  
      
    if (sign)  
    *sp++ = ''-'';  
    while (tp > tmp)  
        *sp++ = *--tp;  
    *sp = 0;  
    return string;  
} </span>  