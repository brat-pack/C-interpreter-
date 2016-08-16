#ifndef C_INTERPRETER_READER_H
#define C_INTERPRETER_READER_H

char reader_peek(char** str);

char reader_next(char** str);

char reader_consume(char** str);

char* reader_make_substring(char* str, int b, int e);


#endif //C_INTERPRETER_READER_H
