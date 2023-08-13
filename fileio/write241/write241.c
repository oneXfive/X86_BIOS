#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
//#include <Protocol/EfiShell.h>
#include <Library/ShellLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern EFI_BOOT_SERVICES         *gBS;
extern EFI_SYSTEM_TABLE          *gST;

int
EFIAPI
main (
    IN int Argc,
    IN char **Argv
)
{
    EFI_STATUS                     Status;
    EFI_FILE_PROTOCOL              *Root;

    EFI_FILE_PROTOCOL              *efi_template = 0;
    EFI_FILE_PROTOCOL              *root_efi = 0;
    EFI_FILE_PROTOCOL              *root_efi_myefi = 0;

    //EFI_FILE_PROTOCOL              *EfiDirectory = 0;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;

    UINTN BufSize,BufSize2;
    CHAR16 *Buf = (CHAR16 *)L"DEBUG qsq";   
    CHAR16 READBUF[50];
    

    Status = gBS->LocateProtocol(
                 &gEfiSimpleFileSystemProtocolGuid,
                 NULL,
                 (VOID **)&SimpleFileSystem);


    if (EFI_ERROR(Status))
    {
        //未找到EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
        Print(L"Cannot find EFI_SIMPLE_FILE_SYSTEM_PROTOCOL \r\n");
        return Status;
    }
    //得到根目录的句柄
    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);
    if (EFI_ERROR(Status))
    {
        Print(L"OpenVolume error \r\n");
        return Status;
    }

    Status = Root->Open(
                 Root,
                 &root_efi,
                 (CHAR16 *)L"EFI",
                 EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                 0X10
             );//在root下创建efi目录

   // Print(L".EFI Status = %d\n", Status);
    Status = Root->Open(
                 root_efi,
                 &root_efi_myefi,
                 (CHAR16 *)L"MYEFI",
                 EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                 0X10
             );//在root->EFI下创建efi目录
    //Print(L".MYEFI Status = %d\n", Status);
    Status = Root->Open(
                 root_efi_myefi,
                 &efi_template,
                 (CHAR16 *)L"efi_template.txt",
                 EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                 0
             );//在root->EFI->MYEFI下创建efi_template.txt文件
    //Print(L".TXT Status = %d\n", Status);
    if (efi_template && !EFI_ERROR(Status))
    {
        BufSize = StrLen(Buf) * 2;
        Print(L"bufsize =%d\n", BufSize);
        Status = efi_template -> Write(efi_template, &BufSize, Buf); //写BUF下的字符到efi_template.txt当中
        Print(L"[%s]\r\n", Buf);
        Print(L"write done \r\n");
    }

    BufSize2= 50;

    efi_template->SetPosition(efi_template,0);//先把文件里面的指针移到开始的地方，要不然就读不了。
   
    Status = efi_template -> Read ( efi_template,
                                    &BufSize2,
                                    READBUF
                                  );//读取efi_template.txt里面的的内容，赋给READBUF

  

    //Print(L"read:status:%d\n", Status);

    if (!EFI_ERROR(Status))
    {
        Print(L"THE READ:%s\n", READBUF);//打印efi_template.txt文件里面的内容
        
    }

    Status = efi_template -> Close(efi_template);
    return EFI_SUCCESS;
}
