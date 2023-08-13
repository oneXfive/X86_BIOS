#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<dos.h>

typedef unsigned char BYTE;
typedef unsigned int WORD;

BYTE my_inp(BYTE port)
{
    BYTE value;
    __asm
    {
        mov al,port
        out 70h,al
        in al,71h
        mov value,al
    }
    return value;
}


void my_outp(BYTE  port,BYTE value)
{
    __asm
    {
        mov al,port
        out 0x70,al
        mov al,value
        out 0x71,al
    }
}

void out_7071(BYTE port,BYTE value)
{
    __asm
    {
        mov al,port
        out 0x70,al
        mov al,value
        out 0x71,al
    }
}

void main()
{
    BYTE addr,data;
    for(addr=0x0; addr<0xff; addr++)
    {
        data = my_inp(addr);
        for(k=0x0; k<=0xf; k++)
        {
            printf("  %02x",k);
        }		

        if(addr%16==0)
        {
            printf("\n");
            printf("%02x",addr);
        }
        printf(" %02x",data);
    }

}
