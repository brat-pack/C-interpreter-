//
// Created by hachiko on 14/05/16.
//

#include <stdio.h>
#include <stdlib.h>

struct State
{
    int c;
    struct State *out;
    struct State *out1;
    int lastlist;
};

struct Frag
{
    struct State *start;
    union Ptrlist *out;
};

union Ptrlist
{
    union Ptrlist *next;
    struct State *s;
};

struct State *state(int c, struct State *out, struct State *out1){
    struct State *s;

    nstate++;
    s = malloc(sizeof *s);
    s->lastlist = 0;
    s->c = c;
    s->out = out;
    s->out1 = out1;
    return s;
}

struct Frag frag(struct State *start, union Ptrlist *out){
    struct Frag n = {start, out};
    return n;
}
/*
 * List1 creates a new pointer list containing the single pointer outp.
 * Append concatenates two pointer lists, returning the result.
 * Patch connects the dangling arrows in the pointer list l to the state s: it sets *outp = s for each pointer outp in l.
 */

union Ptrlist *list1(struct State **outp);
union Ptrlist *apppend(union Ptrlist *l1, union Ptrlist *l2);

void patch(Ptrlist *l1, struct State *s);

struct State* post2nfa(char *postfix)
{
    char *p;
    struct Frag stack[1000], *stackp, e1, e2, e;
    struct State *s;

    #define push(s) *stackp++ = s
    #define pop() *--stackp

    stackp = stack;
    for(p = postfix; *p; p++){
        switch(*p){ /* compilation cases */
            default: /* Literal characters */
                s = state(*p, NULL, NULL);
                push(frag(s, list1(&s->out)));
                break;
            case '.': /* Catenation */
                e2 = pop();
                e1 = pop();
                patch(e1.out, e2.start);
                push(frag(e1.start, e2.out));
                break;
            case '|': /* Alternation */
                e2 = pop();
                e1 = pop();
                s = state(Split, e.start, NULL);
                push(frag(e1.start, e2.out));
                break;
            case '?': /* Zero or one */
                e = pop();
                s = state(Split, e.start, NULL);
                push(frag(s, append(e.out, listl(&s->out1))));
            case '*': /* Zero or more */
                e = pop();
                s = state(Split, e.start, NULL);
                patch(e.out, s);
                push(frag(s, list1(&s->out1)));
                break;
            case '+': /* One or more */
                e = pop();
                s = state(Split, e.start, NULL);
                patch(e.out, s);
                push(frag(e.start, listl(&s->out1)));
                break;
        }
    }
    e = pop();
    patch(e.out, matchstate);
    return e.start;
}

