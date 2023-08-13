#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <i86.h>

unsigned char *TblAddress;
unsigned char *type0;
unsigned short TblLen;
unsigned short *Tblen_temp;

unsigned int tbaddr;
unsigned int end_type_addr;

/*seach the table's address,
传进来一个起始type的指针，函数打印完这个type的信息，并且把指针指向下一个type*/
void find_type_end(unsigned char** addr)
{
    int pr147=0;//set the print swtich,设置打印type0 1 4 7 17 的开关
     if(**addr==0||**addr==1||**addr==4||**addr==7||**addr==17)
    {
        pr147 = 1;
    }
    if(pr147)  printf("\ntype:%d\n",**addr);
    *addr=*addr+*(*addr+1);//point to word infomation address，把指针挪到显示信息的地方。
    while( **addr!=0x00 || (*(*addr+1)!=0x00)  )
    {
        if(pr147==1)
        {
            if(**addr==0x00){ printf("  ");}//printf 2 space,split 2 sentences
            printf("%c",**addr);
        }
        (*addr)++;
    }
    *addr=*addr+2;
   
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
            printf("part1:OK\n");
            printf("\n%x\n",ptr);
            for(i=0; i<4; i++)
            {
                printf("%x",*(ptr+i));
            }          //show _smi_
            printf("\nshow _smi_ ok \n");

            ptr+=0x10;//地址加16个byte
            for(j=0; j<strlen(str2); j++)
            {
                if(*(ptr+j)!=*(str2+j))
                    break;
            }

            if(j>=strlen(str2))//找到第二个字符串'_DMI_'
            {
                printf("part2:ok\n");
                for(i=0; i<5; i++)
                {
                    printf("%x",*(ptr+i));
                }      //show _dmi_
                printf("\nshow _DMI_ ok\n");

                TblAddress=ptr+8;//SMBIOS结构表地址
                tbaddr=*(unsigned int *)TblAddress;//get entry point,this is a number
                printf("\nthe tbaddr = %x\n",tbaddr);

                TblLen=*((unsigned short *)(ptr+6));   //get 2 byte data
                end_type_addr = TblLen+tbaddr;
                printf("\n*tableaddress=%x\n",*TblAddress);

                type0 = (unsigned char *)tbaddr;//type0 is a address,it flow
                printf("\n");

                printf("\ntype0 = %p\n",type0);

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

int main(void)
{
    Search_Strs();
    printf("\n P TblAddress:%p\n",TblAddress);
    printf("\n x *TblAddress:%x\n",*TblAddress);

    printf("\n x TblLen:%x\n",TblLen);

    return 0;
}

