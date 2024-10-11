#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include "drgwalletlib.h"

int main() {
    if (InsertPassword() == 1) {
        printf("\n\n\n\n\n\n\n");
        menu();
        return 0;
    } else {
        printf("\n\n\n\n\n\n\t~ARE YOU A COP?\n\n");
        return 0;
    }
}
