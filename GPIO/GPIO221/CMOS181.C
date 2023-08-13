#include <stdio.h>
#include <conio.h>
#include <dos.h>

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned char uint8;
typedef char char8;

#define CMOS_INDEX_PORT 0X70
#define CMOS_DATA_PORT 0x71

void CMOS_Write(unsigned char index,unsigned char data)
{
    outp(CMOS_INDEX_PORT,index);
    outp(CMOS_DATA_PORT,data);

}

unsigned char CMOS_Read(unsigned char index)
{
    outp(CMOS_INDEX_PORT,index);
    return inp(CMOS_DATA_PORT);

}

void show_cmos(void)
{
	uint8 data;
    for(k=0x0; k<=0xf; k++)
    {
        printf("  %02x",k);
    }   //show line
    
    for(k=0x0; k<=0xff; k++)
    {
        if(k%16==0)
        {
            printf("\n");
            printf("%02x",k);
        }//show raw
        data = CMOS_Read(k);/*read data*/
        printf("  %02x",data);
    }
}

int main(void)
{
	show_cmos();

    return 0;
}

