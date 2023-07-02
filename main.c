#define _CRT_SECURE_NO_WARNINGS


//
//#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include "drgwalletllib.h"













int main()
{

    /*   String pw="ciao";
       FILE *f=fopen("DrugWalletpswrd.bin", "wb");
       fwrite(pw, sizeof(String), 1, f);*/
         if(InsertPassword()==1)
        
         {
           
             printf("\n\n\n\n\n\n\n");
             menu();
             return 0;
         }
         else
             printf("\n\n\n\n\n\n\t~ARE YOU A COP?\n\n");
             return 0;
     }





















//
//
//   String pswrd="OSKI";
//    FILE*f=fopen("DrugWalletpswrd.bin", "wb");
//    fwrite(pswrd, sizeof(String), 1, f);
//    fclose(f);
//    printf("PSWRD SCRITTA.");
//}
