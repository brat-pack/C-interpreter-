//
// Created by Moro on 16/08/16.
//

#include "NFA_Constructor.h"
#include "reader.h"
#include "RegexParser.h"

NFA* REGEX(Expression regex)
{
    NFA* term = TERM(regex);
    NFA* re = REGEX2(term, regex);
    return re;
}

NFA* REGEX2 (NFA* nfa, Expression regex)
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

NFA* TERM(Expression regex)
{
    NFA* Char = CHAR(regex);
    NFA* term = TERM2(Char, regex);
    return term;
}

NFA* TERM2(NFA* Char, Expression regex)
{
    NFA* term;
    switch(reader_peek(regex))
    {
        case '\0':
            return Char;
        case '+':
            reader_consume(regex);
            Char = Create_Plus_NFA(Char);
            term = TERM(regex);
            return Concatenate_NFA(Char, term);
        case '*':
            reader_consume(regex);
            Char = Create_Kleenstar_NFA(Char);
            term = TERM(regex);
            return Concatenate_NFA(Char, term);
        default:
            term = TERM(regex);
            return Concatenate_NFA(Char, term);
    }
}

NFA* CHAR(Expression regex)
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

NFA* REGEX_Evaluate(char* regex) {
    return REGEX(regex);
}

int main()
{
    char* str = "ab*";
    REGEX_Evaluate(str);
}