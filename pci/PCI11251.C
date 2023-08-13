#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef unsigned int uint32;
typedef unsigned short char16;
typedef int int32;
typedef unsigned char uint8;
typedef char char8;

uint32 read_pci(uint32 bus,uint32 dev,uint32 fun,uint32 index)
{
    uint32 addr = 0x80000000;
    uint32 this_bus,this_dev,this_fun;
    uint32 data = 0;

    this_bus = bus;
    this_dev = dev;
    this_fun = fun;

    addr |= (this_bus<<16)|(this_dev<<11)|(this_fun<<8);
    addr += index;

    outpd(0xcf8,addr);
    data = inpd(0xcfc);

    return data;
}

void find_pci(void)
{
    uint32 bus,dev,func;
    uint32 temp_data;
    char16 vendor,device;

    temp_data = read_pci(0,2,0,0x00);
//    printf("\ntemp_data = %p\n",temp_data);
    
    printf("\nbus\tdevice\tfunction\tvendor id\tdevice id\n");
    for (bus = 0; bus <=0xff ; bus++)
    {
        for (dev = 0; dev <=0x1f ; dev++)
        {
            for (func = 0; func <= 0x7; func++)
            {
                temp_data = read_pci(bus,dev,func,0x00);
                
                if(temp_data!=0xFFFFFFFF)
                {
                    vendor = temp_data&0x0000ffff;
                    device = (temp_data&0xffff0000)>>16;
                    printf("%2.2x\t%2.2x\t%2.2x\t\t%04x\t\t%04x",bus,dev,func,vendor,device);
                    printf("\n");
                }
            }
        }
    } 

}

int main(void)
{
    find_pci();    
    return 0;
}
