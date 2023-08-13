#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
//#include <Protocol/EfiShell.h>
#include <Library/ShellLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include <Uutil.h>
#include <Library/IoLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern EFI_BOOT_SERVICES         *gBS;
extern EFI_SYSTEM_TABLE          *gST;


/*register add*/
//#define SMB_Base_Add 0xf000

/*read SPD by byte*/
#define HST_STS 0x00
#define HST_CNT 0x02
#define HST_CMD 0x03
#define XMIT_SLVA 0x04
#define HST_D0 0x05



void D_delay();
CHAR16 SMB_ACK();
CHAR16 Register_Configuration(int SPD_add);
UINT32 read_pci(UINT32 bus, UINT32 dev, UINT32 fun, UINT32 index);
CHAR16 find_pci(void);

CHAR16 SMB_Base_Add;

EFI_STATUS
GetFileIo( EFI_FILE_PROTOCOL **Root)
{
    EFI_STATUS  Status = 0;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
    Status = gBS->LocateProtocol(
                 &gEfiSimpleFileSystemProtocolGuid,
                 NULL,
                 (VOID **)&SimpleFileSystem
             );
    if (EFI_ERROR(Status))
    {
        return Status;
    }
    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, Root);
    if (EFI_ERROR(Status))
    {
        Print(L"OpenVolume error \r\n");
        return Status;
    }
    return Status;
}


int
EFIAPI
main (
    IN int Argc,
    IN char **Argv
)
{
    EFI_STATUS                     Status;
    EFI_FILE_PROTOCOL              *Root;
    EFI_FILE_PROTOCOL              *DIMM_SPD = 0;
    CHAR16 data,k;
    CHAR16 BufSize;
    CHAR16 spd_data[300];
    CHAR16 *spd=spd_data;

    FILE *fp;
    char filename[]="DIMM_SPD.txt";
    int i;
    fp = fopen(filename,"w+");
    if(NULL==fp)
  {
    printf("Failed to open\n");
  }
  else
  {
    printf("open file success!\n");
  }
  
    SMB_Base_Add = find_pci() & 0xfffe;
    Print(L"SMB_Base_Add =%x\n", SMB_Base_Add);
    Print(L"  ");
    fprintf(fp,"  ");
    for (k = 0x0; k <= 0xf; k++)
    {
        Print(L"  %02x", k);
        fprintf(fp,"  %02x", k);
    } //printf raw

    for (k = 0x0; k <= 0xff; k++)
    {
        if (k % 16 == 0)
        {
            Print(L"\n");
            fprintf(fp,"\r\n");
            Print(L"%02x", k);
            fprintf(fp,"%02x", k);
        } //print line
        spd_data[k] = Register_Configuration(k);

        fprintf(fp,"  %02x",Register_Configuration(k));
        Print(L"  %02x", spd_data[k]);
    }

   
    
    fclose(fp);

    return EFI_SUCCESS;
}

void D_delay()
{
    int i;
    for (i = 100; i > 0; i--);
}

CHAR16 SMB_ACK()                     /*check smbus ready*/
{
    int state_A, state_B, i;
    CHAR16 s = 0;

    for (i = 0; i < 0x80; i++)        /*circle until ACK = 0*/
    {
        state_A = IoRead8(SMB_Base_Add | HST_STS);
        D_delay();
        IoWrite8(SMB_Base_Add | HST_STS, state_A);
        D_delay();
        state_B = state_A;
        state_B = state_A & 0x02;
        if (state_B != 0)       /*   INTR = 1 BIT1    */
        {
            goto ret;
        }
        state_B = state_A;
        state_B = state_B & 0xbf;   /*  INUSE_STS = 0   BIT6   */
        if (state_B == 0)
        {
            goto ret;
        }
        state_B = state_A;
        state_B = state_B & 0x04;    /*  DEV_ERR = 1(Invalid Command/Time-out/Unclaimed Cycle)    BIT2    */
        if (state_B)
        {
            break;
        }
    }
    s = 1;
ret:
    return s;

}

CHAR16 Register_Configuration(CHAR16 SPD_add)
{
    CHAR16 state, data;
    IoWrite8(SMB_Base_Add | HST_STS, 0xFE); /*clear */
    IoWrite8(SMB_Base_Add | XMIT_SLVA, 0xA1);/*slave/SPD base address */ //1 is read,0 is write/* last bit is w/r,must be 1 */
    IoWrite8(SMB_Base_Add | HST_CMD, SPD_add); /*offset*/
    IoWrite8(SMB_Base_Add | HST_CNT, 0x48); /*byte read*/
    D_delay();


    state = SMB_ACK();
    if (state != 0)
    {
        return 0xff;
    }
    data = IoRead8(SMB_Base_Add | HST_D0);
    D_delay();
    return data;

}

UINT32 read_pci(UINT32 bus, UINT32 dev, UINT32 fun, UINT32 index)
{
    UINT32 addr = 0x80000000;
    UINT32 this_bus, this_dev, this_fun;
    UINT32 data = 0;

    this_bus = bus;
    this_dev = dev;
    this_fun = fun;

    addr |= (this_bus << 16) | (this_dev << 11) | (this_fun << 8);
    addr += index;

    IoWrite32(0xcf8, addr);
    data = IoRead32(0xcfc);

    return data;
}//只要输入bdf和偏移地址就可以直接读取它存储的值。如：输入read_pci(0,2,0,0).输出：0f318086



CHAR16 find_pci(void)
{
    UINT32 bus, dev, func;
    UINT32 temp_data;
    CHAR16 smbus_addr;
    CHAR16 class_code;


    temp_data = read_pci(0, 2, 0, 0x00);
    Print(L"\ntemp_data = %p\n", temp_data);

    for (bus = 0; bus <= 0xff ; bus++)
    {
        for (dev = 0; dev <= 0x1f ; dev++)
        {
            for (func = 0; func <= 0x7; func++)
            {
                temp_data = read_pci(bus, dev, func, 0x00);
                // printf("\ntemp_data = %p\n",temp_data);
                if (temp_data != 0xFFFFFFFF)
                {
                    //printf("%2.2x\t%2.2x\t%2.2x\t",bus,dev,func);
                    //Print(L"\n");
                    class_code = read_pci(bus, dev, func, 0x08) >> 16;
                    if (class_code == 0x0c05)
                    {
                        smbus_addr = read_pci(bus, dev, func, 0x20) % 0x0000ffff;
                        Print(L"smbus_addr=%x\n", smbus_addr);
                        goto ret;
                    }
                }
            }
        }
    }
ret:
    return smbus_addr;
}