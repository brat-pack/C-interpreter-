//
// Created by Moro on 11/05/16.
//

#include <stdio.h>



int main(int argc, char* argv[]) {
    printf("print");

    FILE *fr; /* declare file pointer */
    int c;

    fr = fopen("files/files/my.txt", "rb");

    while((c = getc(fr)) != EOF)
        putchar(c);
    fclose(fr);
    return 0;
}