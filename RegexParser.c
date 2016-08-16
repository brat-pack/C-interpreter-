//
// Created by Moro on 16/08/16.
//

#include "NFA_Constructor.h"
#include "reader.h"
#include "RegexParser.h"
#include <string.h>

NFA* REGEX(Expression* regex)
{
    NFA* term = TERM(regex);
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
    else
    {
        return nfa;
    }
}

NFA* TERM(Expression* regex)
{
    NFA* Char = CHAR(regex);
    NFA* term = NULL;
    switch(reader_peek(regex))
    {
        case '+':
            reader_consume(regex);
            Char = Create_Plus_NFA(Char);
            term = TERM2(regex);
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
            printf(regex);
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
        default:
            return Create_Primitive_NFA(reader_consume(regex));
    }
}

NFA* REGEX_Evaluate(Expression* regex) {
    return REGEX(regex);
}

int main()
{
        char* c = "ab+a";
        REGEX_Evaluate(&c);
}