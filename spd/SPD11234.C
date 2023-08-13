#include <stdio.h>
#include <conio.h>
#include <dos.h>

/*register add*/
//#define SMB_Base_Add 0xf000

/*read SPD by byte*/
#define HST_STS 0x00
#define HST_CNT 0x02
#define HST_CMD 0x03
#define XMIT_SLVA 0x04
#define HST_D0 0x05

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned char uint8;
typedef char char8;

void D_delay();
int SMB_ACK();
int Register_Configuration(int SPD_add);
uint32 read_pci(uint32 bus,uint32 dev,uint32 fun,uint32 index);
uint16 find_pci(void);
void ShowMemoryType(void);
void show_DDRX(void);
void show_DIMM(void);
void show_capacity(void);

uint16 SMB_Base_Add;


void main()
{
    int data,k;
    printf("\n");
    SMB_Base_Add = find_pci()&0xfffe;

    printf("  ");
    for(k=0x0; k<=0xf; k++)
    {
        printf("  %02x",k);
    }           //打印行

    for(k=0x0; k<=0xff; k++)
    {
        if(k%16==0)
        {
            printf("\n");
            printf("%02x",k);
        }       //打印列
        data = Register_Configuration(k);/*read data*/
        printf("  %02x",data);
    }

    show_DDRX();
	show_DIMM();
	show_capacity();
	
}
int Register_Configuration(int SPD_add)
{
    int state,data;
    outp(SMB_Base_Add|HST_STS,0xFE);/*clear */
    outp(SMB_Base_Add|XMIT_SLVA,0xA1);/*slave/SPD base address *///1 is read,0 is write/* last bit is w/r,must be 1 */
    outp(SMB_Base_Add|HST_CMD,SPD_add);/*offset*/
    outp(SMB_Base_Add|HST_CNT,0x48);/*byte read*/
    D_delay();


    state = SMB_ACK();
    if(state!=0)
    {
        return 0xff;
    }
    data = inp(SMB_Base_Add|HST_D0);
    D_delay();
    return data;

}
void D_delay()
{
    int i;
    for(i=100; i>0; i--);
}
int SMB_ACK()                     /*check smbus ready*/
{
    int state_A,state_B,i,s=0;

    for(i=0; i<0x80; i++)             /*circle until ACK = 0*/
    {
        state_A = inp(SMB_Base_Add|HST_STS);
        D_delay();
        outp(SMB_Base_Add|HST_STS,state_A);
        D_delay();
        state_B = state_A;
        state_B = state_A&0x02;
        if(state_B!=0)          /*   INTR = 1 BIT1    */
        {
            goto ret;
        }
        state_B = state_A;
        state_B = state_B&0xbf;     /*  INUSE_STS = 0   BIT6   */
        if(state_B==0)
        {
            goto ret;
        }
        state_B = state_A;
        state_B = state_B&0x04;      /*  DEV_ERR = 1(Invalid Command/Time-out/Unclaimed Cycle)    BIT2    */
        if(state_B)
        {
            break;
        }
    }
    s = 1;
ret:
    return s;

}

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

uint16 find_pci(void)
{
    uint32 bus,dev,func;
    uint32 temp_data;
    uint16 smbus_addr;
    uint16 class_code;

    temp_data = read_pci(0,2,0,0x00);

    for (bus = 0; bus <=0xff ; bus++)
    {
        for (dev = 0; dev <=0x1f ; dev++)
        {
            for (func = 0; func <= 0x7; func++)
            {
                temp_data = read_pci(bus,dev,func,0x00);

                if(temp_data!=0xFFFFFFFF)
                {
                    class_code = read_pci(bus,dev,func,0x08)>>16;                       //取高2字节判断设备
                    if(class_code==0x0c05)      //choice smbus device
                    {
                        smbus_addr=read_pci(bus,dev,func,0x20)&0x0000ffff;      //低2字节是smbus的地址
                        printf("smbus_addr=%x\n",smbus_addr);
                    }
                }
            }
        }
    }
    return smbus_addr;
}

//----------------------------------------------
// Show Memory Type
//----------------------------------------------


void show_DDRX(void)
{
    int dat;
    dat = Register_Configuration(0x02);
    if (dat != 0xff)
    {
        printf("\nThis DRAM is ");
        switch(dat)
        {
            case 1:
                printf("stanard fpm dram ");
                break;
            case 2:
                printf("EDO ");
                break;
            case 3:
                printf("pipelined nibble ");
                break;
            case 4:
                printf("SDRAM ");
                break;
            case 5:
                printf("ROM ");
                break;
            case 6:
                printf("DDR SGRAM ");
                break;
            case 7:
                printf("DDR SDRAM ");
                break;
            case 8:
                printf("DDR2 SDRAM ");
                break;
            case 9:
                printf("DDR2 SDRAM FB-DIMM ");
                break;
            case 10:
                printf("DDR2 SDRAM FB-DIMM PROBE");
                break;
            case 11:
                printf("DDR3 SDRAM");
                break;
            case 12:
                printf("DDR4 SDRAM");
                break;
            default:
                printf("UNKNOWN ");
                break;
        }
    }
    else
    {
        printf("read SMB error1!\n");
    }

}

void show_DIMM(void)
{
    int dat;
    dat = Register_Configuration(0x03);
    dat =dat&0xf;
    if (dat != 0xff)
    {
        printf("\nThis base moduele type is ");
        switch(dat)
        {
            case 1:
                printf("RDIMM ");
                break;
            case 2:
                printf("UDIMM ");
                break;
            case 3:
                printf("SO-DIMM ");
                break;
            case 4:
                printf("LRP-DIMM ");
                break;
            case 5:
                printf("MINI-RDIMM ");
                break;
            case 6:
                printf("MINI-UDIMM ");
                break;

            case 8:
                printf("72B-SO-RDIMM ");
                break;
            case 9:
                printf("72B-SO-UDIMM ");
                break;
           
            case 12:
                printf("16B-SO-DIMM");
                break;
			case 13:
                printf("32B-SO-DIMM");
                break;
            default:
                printf("UNKNOWN ");
                break;
        }
    }
    else
    {
        printf("read SMB error1!\n");
    }

}

void show_capacity(void)
{
    int dat;
    dat = Register_Configuration(0x04);
        dat =dat&0xf;
    if (dat != 0xff)
    {
        printf("\nThis SDRAM capacity is ");
        switch(dat)
        {
            case 0:
                printf("256 MB ");
                break;
			case 1:
                printf("512 MB ");
                break;
            case 2:
                printf("1 GB ");
                break;
            case 3:
                printf("2 GB ");
                break;
            case 4:
                printf("4 GB ");
                break;
            case 5:
                printf("8 GB ");
                break;
            case 6:
                printf("16 GB ");
                break;

            case 7:
                printf("32 GB ");
                break;
 
            default:
                printf("UNKNOWN ");
                break;
        }
    }
    else
    {
        printf("read SMB error1!\n");
    }

}
