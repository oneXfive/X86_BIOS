#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <i86.h>

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned char uchar;
typedef char char8;

unsigned char *TblAddress;
unsigned char *type0;
unsigned short TblLen;
unsigned short *Tblen_temp;

unsigned int tbaddr;
unsigned int end_type_addr;

void find_type_end(unsigned char** addr);
void Search_Strs(void);
void print_type0(uchar *addr);
void print_type1(uchar *addr);
void print_type4(uchar *addr);
void print_type7(uchar *addr);
void print_type17(uchar *addr);


int main(void)
{
    Search_Strs();

    return 0;
}


/*seach the table's address,
传进来一个起始type的指针，函数打印完这个type的信息，并且把指针指向下一个type*/
void find_type_end(unsigned char** addr)
{

    if(**addr==0)print_type0(*addr);
    if(**addr==1)print_type1(*addr);
    if(**addr==4)print_type4(*addr);
    if(**addr==7)print_type7(*addr);
    if(**addr==17)print_type17(*addr);

    *addr=*addr+*(*addr+1);//point to word infomation address，把指针挪到显示信息的地方。
    while( **addr!=0x00 || (*(*addr+1)!=0x00)  )
    {
        //printf("%c",**addr);
        (*addr)++;
    }
    *addr=*addr+2;//指到下一个type的起始地址
}


void Search_Strs(void)
{
    unsigned char *str1="_SM_";
    unsigned char *str2="_DMI_";
    unsigned char *ptr=(unsigned char *)0xf0000;
    int i,j,k;
    k=0;

    while(ptr<0xfffff)
    {
        for(j=0; j<strlen(str1); j++)
        {
            if(*(ptr+j)!=*(str1+j))
                break;
        }
        if(j>=strlen(str1)) /*则已找到第一个字符串'_SM_',在其后16byte的5个byte是否是关键字'_DMI_'*/
        {
            ptr+=0x10;//地址加16个byte
            for(j=0; j<strlen(str2); j++)
            {
                if(*(ptr+j)!=*(str2+j))
                    break;
            }

            if(j>=strlen(str2))//找到第二个字符串'_DMI_'
            {
                TblAddress=ptr+8;//SMBIOS结构表地址
                tbaddr=*(unsigned int *)TblAddress;//get entry point,this is a number

                TblLen=*((unsigned short *)(ptr+6));   //get 2 byte data
                printf("structure table length: %d byte\n",TblLen);
                end_type_addr = TblLen+tbaddr;

                type0 = (unsigned char *)tbaddr;//type0 is a address,it flow
                printf("type0_entry_address: %p\n",type0);

                /*打印所有的type表，print all type table*/
                while(type0<(unsigned char *)end_type_addr)
                {
                    find_type_end(&type0);
                }
                break;
            }
        }
        else
        {
            ptr+=0x10;//没有找到第一个字符串，ptr+0x10重新search。
            k++;
        }
    }



}

void print_type0(uchar *addr)
{
    uchar i=0;
    uchar *type0_entry_addr = addr;
    uchar *type0_string_addr = addr+*(addr+1);
    printf("\n");
    printf("\n---BIOS Information (type0)---\n");
    printf("BIOS ROM Size: %dKB\n",64*(*(type0_entry_addr+0x9)+1));
    printf("Vendor:");//show first string mean
    while(*type0_string_addr!=0||*(type0_string_addr+1)!=0)
    {
        if(*type0_string_addr==0x0)
        {
            i++;
            switch(i)
            {
                case 1:
                    printf("\nBIOS Version:");
                    break;
                case 2:
                    printf("\nBIOS Release Data:");
                    break;
                default:
                    printf("-00?-");
                    break;
            }
        }
        printf("%c",*type0_string_addr);
        type0_string_addr++;
    }
}

void print_type1(uchar *addr)
{
    uchar i=0;
    uchar *type1_entry_addr = addr;
    uchar *type1_string_addr = addr+*(addr+1);//
    printf("\n");
    printf("\n---System Information (type1)---\n");

    printf("Manufacturer:");//show first string mean
    while(*type1_string_addr!=0x0||*(type1_string_addr+1)!=0x0)
    {
        if(*type1_string_addr==0x0)
        {
            i++;
            switch(i)
            {
                case 1:
                    printf("\nProduct Name:");
                    break;
                case 2:
                    printf("\nVersion:");
                    break;
                case 3:
                    printf("\nSerial Number:");
                    break;
                case 4:
                    printf("\nSKU Number:");
                    break;
                case 5:
                    printf("\nFamily:");
                    break;
                default:
                    printf("-1?-");
                    break;
            }
        }
        printf("%c",*type1_string_addr);
        type1_string_addr++;
    }
}

void print_type4(uchar *addr)
{
    uchar i=0;
    uchar *type_entry_addr = addr;
    uchar *type_string_addr = addr+*(addr+1);//
    printf("\n");
    printf("\n---Processor Information (type4)---\n");

    printf("Socket Designaion:");//show first string mean
    while(*type_string_addr!=0||*(type_string_addr+1)!=0)
    {
        if(*type_string_addr==0x0)
        {
            i++;
            switch(i)
            {
                case 1:
                    printf("\nProcessor Version:");
                    break;
                case 2:
                    printf("\nSerial Number:");
                    break;
                case 3:
                    printf("\nAsser Tag:");
                    break;
                case 4:
                    printf("\nPart Number:");
                    break;
                default:
                    printf("-4?-");
                    break;
            }
        }
        printf("%c",*type_string_addr);
        type_string_addr++;
    }
}

void print_type7(uchar *addr)
{
    uchar i=0;
    uchar *type_entry_addr = addr;
    uchar *type_string_addr = addr+*(addr+1);//
    printf("\n");
    printf("\n---Cache Information (type7)---\n");

    printf("Socket Designaion):");//show first string mean
    while(*type_string_addr!=0||*(type_string_addr+1)!=0)
    {
        if(*type_string_addr==0x0)
        {
            i++;
            switch(i)
            {
                case 1:
                    printf("\nProcessor Version:");
                    break;
                case 2:
                    printf("\nSerial Number:");
                    break;
                case 3:
                    printf("\nAsser Tag:");
                    break;
                case 4:
                    printf("\nPart Number:");
                    break;
                default:
                    printf("-7?-");
                    break;
            }
        }
        printf("%c",*type_string_addr);
        type_string_addr++;
    }
}

void print_type17(uchar *addr)
{
    uchar i=0;
    int kb_or_mb;
    unsigned short size = *((unsigned short*)(addr+0xc));
    unsigned short speed = *(unsigned short *)(addr+0x15);
    uchar *type_entry_addr = addr;
    uchar *type_string_addr = addr+*(addr+1);//
    printf("\n");
    printf("\n---Mermory Deivece(type17)---\n");
    kb_or_mb = (*(type_entry_addr+0xc)&0xffff)>>31;
    size = *(type_entry_addr+0xc)&0xefff;
    printf("Speed:%d Mhz\n",speed);
    printf("Deice Locator:");//show first string mean
    while(*type_string_addr!=0||*(type_string_addr+1)!=0)
    {
        if(*type_string_addr==0x0)
        {
            i++;
            switch(i)
            {
                case 1:
                    printf("\nBank Locator:");
                    break;
                case 2:
                    printf("\nManufacturer:");
                    break;
                case 3:
                    printf("\nSerial Number:");
                    break;
                case 4:
                    printf("\nAssert Tag:");
                    break;
                case 5:
                    printf("\nPart Number:");
                    break;

                default:
                    printf("-17?-");
                    break;
            }
        }
        printf("%c",*type_string_addr);
        type_string_addr++;
    }
}





