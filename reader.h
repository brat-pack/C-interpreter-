//
// Created by Moro on 23/05/16.
//

#ifndef C_INTERPRETER_READER_H
#define C_INTERPRETER_READER_H

char peek(char* str);

char next(char* str);

char consume(char** str);

int eat(char** str, char c);

char* make_substring(char* str, int b, int e);


#endif //C_INTERPRETER_READER_H