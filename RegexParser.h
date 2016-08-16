//
// Created by Moro on 16/08/16.
//

#ifndef C_INTERPRETER_REGEXPARSER_H
#define C_INTERPRETER_REGEXPARSER_H
#include "NFA_Constructor.h"
#include <stdlib.h>
#include <stdio.h>

NFA* REGEX_Evaluate(Expression regex);
NFA* REGEX(Expression regex);
NFA* REGEX2 (NFA* nfa, Expression regex);
NFA* TERM(Expression regex);
NFA* TERM2(NFA* Char, Expression regex);
NFA* CHAR(Expression regex);


#endif //C_INTERPRETER_REGEXPARSER_H
