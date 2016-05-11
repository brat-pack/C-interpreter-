//
// Created by Moro on 11/05/16.
//

#include <stdio.h>

FILE *fr; /* declare file pointer */

int main(int argc, char* argv[]) {

    int c;

    fr = fopen("LICENCE.txt", "rt");

    while((c = getc(fr)) != EOF)
        putchar(c);
    fclose(fr);
    return 0;
}