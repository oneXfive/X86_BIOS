#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <i86.h>

#define BIT0 0x00000001
#define BIT1 0x00000002
#define BIT2 0x00000004
#define BIT3 0x00000008
#define BIT4 0x00000010
#define BIT5 0x00000020
#define BIT6 0x00000040
#define BIT7 0x00000080

#define SIO_INDEX_PORT 0X2e
#define SIO_DATA_PORT 0x2f

void SIO_Write(unsigned char index,unsigned char data)
{
    outp(SIO_INDEX_PORT,index);
    outp(SIO_DATA_PORT,data);

}

unsigned char SIO_Read(unsigned char index)
{
    outp(SIO_INDEX_PORT,index);
    return inp(SIO_DATA_PORT);

}


void wdt_reset(const unsigned char time,const unsigned char count_mode)
{
    unsigned char data;
    outp(SIO_INDEX_PORT,0x87);
    outp(SIO_INDEX_PORT,0x87);//unlock

    SIO_Write(0x07,0x08);   //choice LDN8

    data = SIO_Read(0x30)|BIT0;
    SIO_Write(0x30,data);           //active wdt

    data = SIO_Read(0xF2)&(~BIT4);
    SIO_Write(0XF2,data);           //clear status

    data = SIO_Read(0XF0)&(~BIT3);
    data |= (count_mode<<3);
    SIO_Write(0XF0,data);           //choice min or sec mode

    SIO_Write(0XF1,time);


}

int main(void)
{
    int mode,time,min;
    printf("please select mode:0(sec),1(min)\n");
    scanf("%d",&mode);

    printf("please enther the time\n");
    scanf("%d",&time);

    wdt_reset(time,mode);

    if(mode==0)//select secondes mode
    {
        for(time; time>0; time--)
        {
            printf("shut dowm computer after %ds\n",time);
            delay(1000);

        }
    }
    if(mode==1)
    {
         min=time*60;
        for(min; min>0; min--)
        {
            printf(" shut dowm computer after %ds\n",min);
            delay(1000);
        }
    }

    return 0;

}


