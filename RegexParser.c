//
// Created by Moro on 16/08/16.
//

#include "NFA_Constructor.h"
#include "reader.h"
#include "RegexParser.h"
#include "PrettyPrinter.h"
#include <string.h>

NFA* REGEX(Expression* regex)
{
    NFA* term = TERM(regex);
    if (term == NULL) {
        printf("Invalid regular expression:");
        printf(*regex);
        exit(-1);
    }
    NFA* re = REGEX2(term, regex);
    return re;
}

NFA* REGEX2 (NFA* nfa, Expression* regex)
{
    if (reader_peek(regex) == '|')
    {
        reader_consume(regex);
        NFA* re = REGEX(regex);
        return Create_Union_NFA(nfa, re);
    }
    else if (reader_peek(regex) == '\0')
    {
        return nfa;
    }
    else {
        printf("Invalid regular expression:");
        printf(*regex);
        exit(-1);
    }
}

NFA* TERM(Expression* regex)
{
    NFA* Char = CHAR(regex);
    if (Char == NULL) {
        return NULL;
    }

    NFA* term = NULL;
    switch(reader_peek(regex))
    {
        case '+':
            reader_consume(regex);
            Char = Create_Plus_NFA(Char);
            term = TERM2(regex);
            break;
        default:
            term = TERM2(regex);
    }
    if (term != NULL)
        return Concatenate_NFA(Char, term);
    return Char;
}

NFA* TERM2(Expression* regex)
{
    switch(reader_peek(regex))
    {
        case '\0':
            return NULL;
        default:
            return TERM(regex);
    }
}

NFA* CHAR(Expression* regex)
{
    switch(reader_peek(regex))
    {
        case '\0':
            printf("Invalid regular expression:");
            printf(*regex);
            exit(-1);
        /*case '(':
            reader_consume(regex);
            NFA* re = REGEX(regex);
            if (reader_consume(regex) == ')')
            {
                return re;
            }
            else
            {
                printf("Invalid regular expression:");
                printf(regex);
                exit(-1);
            }
        case ')':*/
        case '\\':
            reader_consume(regex);
            return Create_Primitive_NFA(reader_consume(regex));
        case 'a':
        case 'b':
        case 'c':
        case 'd':
            return Create_Primitive_NFA(reader_consume(regex));
        default:
            return NULL;
    }
}

NFA* REGEX_Evaluate(Expression* regex) {
    return REGEX(regex);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen (argv[1] , "rb" );
    if( !fp ) perror(argv[1]),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

/* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

/* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    fclose(fp);
    NFA* nfa = REGEX_Evaluate(&buffer);
    PrettyPrint_NFA(nfa);
}

