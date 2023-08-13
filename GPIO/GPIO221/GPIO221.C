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

void SIO_Write(unsigned char index,unsigned char data);
unsigned char SIO_Read(unsigned char index);
void NCT6106D_GPIO_INIT(void);
unsigned char set_bit(unsigned char bit,unsigned char *data);
unsigned char reset_bit(unsigned char bit,unsigned char *data);
unsigned char read_bit(unsigned char bit,unsigned char *data);
void output_mode(unsigned char a,unsigned char b);
void input_mode(unsigned char a,unsigned char b);
void output_low(unsigned char a,unsigned char b);
void output_high(unsigned char a,unsigned char b);
void read_high_low(unsigned char a,unsigned char b);

void NCT6106_GPIO_SELECT(void)
{
    unsigned char a,b,data,mode,high_or_low;
    NCT6106D_GPIO_INIT();
    printf("please input GPx\n");
    scanf("%d",&a);
    printf("please input GP%dx",a);
    scanf("%d",&b);
    printf("please input mode(0 or 1),1(input mode)  0(output mode)\n");
    scanf("%d",&mode);
    if(mode==0)
    {
        printf("please select high or low  ),1(high)  0(low)\n");
        scanf("%d",&high_or_low);
    }

    SIO_Write(0x07,0x07);   //choice LDN7
    data=SIO_Read(0x30);        //激活相应的的GPIO
    data = set_bit(a,&data);
    SIO_Write(0x30,data);       //相应的GPIO已经激活

    /* 看看是输入模式还是输出模式 */
    if(mode)//input mdoe
    {
        input_mode(a,b);
        read_high_low(a,b);

    }

    if(mode==0)//output mdoe
    {
        output_mode(a,b);
        if(high_or_low)//输出高电平
        {
            output_high(a,b);
        }
        if(high_or_low==0)//输出低电平
        {
            output_low(a,b);
        }
    }

    printf("end\n");


}


int main(void)
{
    NCT6106_GPIO_SELECT();

    return 0;

}


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


void NCT6106D_GPIO_INIT(void)
{
    unsigned char data;
    outp(SIO_INDEX_PORT,0x87);
    outp(SIO_INDEX_PORT,0x87);//unlock

    SIO_Write(0x07,0x08);   //choice LDN8
    data = SIO_Read(0x30);
    data |= BIT1;
    SIO_Write(0x30,data);//active gpio base mode
}


unsigned char set_bit(unsigned char bit,unsigned char *data)
{

    switch(bit)
    {
        case 0:
            *data |= BIT0;
            break;
        case 1:
            *data |= BIT1;
            break;
        case 2:
            *data |= BIT2;
            break;
        case 3:
            *data |= BIT3;
            break;
        case 4:
            *data |= BIT4;
            break;
        case 5:
            *data |= BIT5;
            break;
        case 6:
            *data |= BIT6;
            break;
        case 7:
            *data |= BIT7;
            break;
        default:
            printf("you input number error\n");
            break;
    }

    return *data;
}

unsigned char reset_bit(unsigned char bit,unsigned char *data)
{
    switch(bit)
    {
        case 0:
            *data &= ~(BIT0);
            break;
        case 1:
            *data &= ~(BIT1);
            break;
        case 2:
            *data &= ~(BIT2);
            break;
        case 3:
            *data &= ~(BIT3);
            break;
        case 4:
            *data &= ~(BIT4);
            break;
        case 5:
            *data &= ~(BIT5);
            break;
        case 6:
            *data &= ~(BIT6);
            break;
        case 7:
            *data &= ~(BIT7);
            break;
        default:
            printf("you input mode number error\n");
            break;
    }
    return *data;
}

unsigned char read_bit(unsigned char bit,unsigned char *data)
{
    switch(bit)
    {
        case 0:
            *data &= BIT0;
            *data>>0;
            break;
        case 1:
            *data &= BIT1;
            *data>>1;
            break;
        case 2:
            *data &= BIT2;
            *data>>2;
            break;
        case 3:
            *data &= BIT3;
            *data>>3;
            break;
        case 4:
            *data &= BIT4;
            *data>>4;
            break;
        case 5:
            *data &= BIT5;
            *data>>5;
            break;
        case 6:
            *data &= BIT6;
            *data>>6;
            break;
        case 7:
            *data &= BIT7;
            *data>>7;
            break;
        default:
            printf("you input mode number error\n");
            break;
    }
    return *data;
}




void output_mode(unsigned char a,unsigned char b)
{
    /*step3：先将它设置成输出模式*/
    unsigned char data;
    if(a==0)
    {
        data=SIO_Read(0xe0);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xe0,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==1)
    {
        data=SIO_Read(0xe4);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xe4,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==2)
    {
        data=SIO_Read(0xe8);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xe8,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==3)
    {
        data=SIO_Read(0xec);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xec,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==4)
    {
        data=SIO_Read(0xf0);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xf0,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==5)
    {
        data=SIO_Read(0xf4);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xf4,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==6)
    {
        data=SIO_Read(0xf8);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xf8,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==7)
    {
        data=SIO_Read(0xfc);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xfc,data);   //相应的GPIO0b已经设置成输出模式
    }

}

void input_mode(unsigned char a,unsigned char b)
{
    /*step3：先将它设置成输入模式*/
    unsigned char data;
    if(a==0)
    {
        data=SIO_Read(0xe0);    //GPIO0
        data = set_bit(b,&data);
        SIO_Write(0xe0,data);
    }

    if(a==1)
    {
        data=SIO_Read(0xe4);    //GPIO1
        data = set_bit(b,&data);
        SIO_Write(0xe4,data);
    }

    if(a==2)
    {
        data=SIO_Read(0xe8);    //GPIO2
        data = set_bit(b,&data);
        SIO_Write(0xe8,data);
    }

    if(a==3)
    {
        data=SIO_Read(0xec);    //GPIO3
        data = set_bit(b,&data);
        SIO_Write(0xec,data);
    }

    if(a==4)
    {
        data=SIO_Read(0xf0);    //GPIO4
        data = set_bit(b,&data);
        SIO_Write(0xf0,data);
    }

    if(a==5)
    {
        data=SIO_Read(0xf4);    //GPIO5
        data = set_bit(b,&data);
        SIO_Write(0xf4,data);
    }

    if(a==6)
    {
        data=SIO_Read(0xf8);    //GPIO6
        data = set_bit(b,&data);
        SIO_Write(0xf8,data);
    }

    if(a==7)
    {
        data=SIO_Read(0xfc);    //GPIO7
        data = set_bit(b,&data);
        SIO_Write(0xfc,data);
    }

}

void output_low(unsigned char a,unsigned char b)
{
    /*step4：先将它输出低电平*/
    unsigned char data;
    if(a==0)
    {
        data=SIO_Read(0xe1);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xe1,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==1)
    {
        data=SIO_Read(0xe5);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xe5,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==2)
    {
        data=SIO_Read(0xe9);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xe9,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==3)
    {
        data=SIO_Read(0xed);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xed,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==4)
    {
        data=SIO_Read(0xf1);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xf1,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==5)
    {
        data=SIO_Read(0xf4);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xf4,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==6)
    {
        data=SIO_Read(0xf9);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xf9,data);   //相应的GPIO0b已经设置成输出模式
    }

    if(a==7)
    {
        data=SIO_Read(0xfd);    //GPIO0
        data = reset_bit(b,&data);
        SIO_Write(0xfd,data);   //相应的GPIO0b已经设置成输出模式
    }

}

void output_high(unsigned char a,unsigned char b)
{
    /*step4：先将它输出高电平*/
    unsigned char data;
    if(a==0)
    {
        data=SIO_Read(0xe1);    //GPIO0
        data = set_bit(b,&data);
        SIO_Write(0xe1,data);
    }

    if(a==1)
    {
        data=SIO_Read(0xe5);    //GPIO1
        data = set_bit(b,&data);
        SIO_Write(0xe5,data);
    }

    if(a==2)
    {
        data=SIO_Read(0xe9);    //GPIO2
        data = set_bit(b,&data);
        SIO_Write(0xe9,data);
    }

    if(a==3)
    {
        data=SIO_Read(0xed);    //GPIO3
        data = set_bit(b,&data);
        SIO_Write(0xed,data);
    }

    if(a==4)
    {
        data=SIO_Read(0xf1);    //GPIO4
        data = set_bit(b,&data);
        SIO_Write(0xf1,data);
    }

    if(a==5)
    {
        data=SIO_Read(0xf4);    //GPIO5
        data = set_bit(b,&data);
        SIO_Write(0xf4,data);
    }

    if(a==6)
    {
        data=SIO_Read(0xf9);    //GPIO6
        data = set_bit(b,&data);
        SIO_Write(0xf9,data);
    }

    if(a==7)
    {
        data=SIO_Read(0xfd);    //GPIO7
        data = set_bit(b,&data);
        SIO_Write(0xfd,data);
    }

}

void read_high_low(unsigned char a,unsigned char b)
{
    /*step4：读取高低电平*/
    unsigned char data;
    if(a==0)
    {
        data=SIO_Read(0xe1);    //GPIO0
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==1)
    {
        data=SIO_Read(0xe5);    //GPIO1
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==2)
    {
        data=SIO_Read(0xe9);    //GPIO2
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==3)
    {
        data=SIO_Read(0xed);    //GPIO3
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==4)
    {
        data=SIO_Read(0xf1);    //GPIO4
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==5)
    {
        data=SIO_Read(0xf4);    //GPIO5
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==6)
    {
        data=SIO_Read(0xf9);    //GPIO6
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

    if(a==7)
    {
        data=SIO_Read(0xfd);    //GPIO7
        data = read(b,&data);
        printf("read result:%d\n",data);
    }

}
