#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<dos.h>
#include<math.h>

//#define DEBUG

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

//----------------------------------------------------------
//Get temperature
//----------------------------------------------------------
WORD getTemp()
{
    WORD i;

    __asm{
        mov ecx,0x19c
        rdmsr
        mov i,eax
       }

    return i;
}

void main(){
    WORD TEM;
    TEM = (getTemp()>>16)&0xff;
    TEM = 0x64-TEM;

    printf("\nCPU Temperature:%d C\n\n",TEM);
}
