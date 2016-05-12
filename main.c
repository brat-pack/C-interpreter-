//
// Created by Moro on 11/05/16.
//
#include <stdio.h>

enum TokenType { ID, NUM, REAL, IF, COMMA, LPAREN, RPAREN, END_OF_FILE };
struct Token {
    enum TokenType;
    char Value[];
};

// Need three arrays.
// 1 multidimensional array: a transition matrix.
// 1 array for storing whether a state is a final state or not.
// 1 array that has pointers to functions creating a token of the found type, and putting the string value inside.

int (*TransitionMatrix[4]) (char c);

int StateFinal[];
struct Token (*StateAction) (char* value);

int GetCharacter(FILE* fr) {
    return fgetc(fr);
}

struct Token Lexer(FILE *fs) {
        long int start_scan = ftell(fs);
        long int last_final_scan = start_scan;

        int previous_final_state = 0;
        int current_state = 1;


        while(1) {
            if (current_state != 0) {
                int next_char = GetCharacter(fs);
                current_state = 0;
                if (StateFinal[current_state]) {
                    previous_final_state = current_state;
                    last_final_scan = ftell(fs);
                }
            } else {
                long int chars_to_read = start_scan - last_final_scan;
                fseek(fs, start_scan, 0);
                int value[chars_to_read];
                for (int i = 0; i < chars_to_read; i++) {
                    value[i] = GetCharacter(fs);
                }
                return { ID, value};
            }
        }
}


int main(int argc, char* argv[]) {
    FILE *fr; /* declare file pointer */

    fr = fopen("files/files/my.txt", "rb");

    fclose(fr);
    return 0;
}