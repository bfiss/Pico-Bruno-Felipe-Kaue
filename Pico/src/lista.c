#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef _LISTA_C_
#define _LISTA_C_

// macro para inicialização dos membros da struct tac *
#define INICIALIZA(a) t->a = a;


struct tac * create_inst_tac(const enum tacType type, const enum opType tArg1,
            const enum opType tArg2, const enum opType tArg3, const int arg1,
                            const int arg2, const int arg3, const char op[2]) {
    struct tac * t = (struct tac *) malloc(sizeof(struct tac));
    assert(t);
    t->op[0] = op[0];
    t->op[1] = op[1];
    t->op[2] = '\0';
    INICIALIZA(arg1);
    INICIALIZA(arg2);
    INICIALIZA(arg3);
    INICIALIZA(tArg1);
    INICIALIZA(tArg2);
    INICIALIZA(tArg3);
    INICIALIZA(type);
    return t;
}

void print_inst_tac(FILE* out, struct tac i) {
    static int line = 0;
    switch (i.type) {
    case _LAB:
	--line;
	fprintf(out,"L_%03i:\n",i.arg1);
	break;
    case _IF:
	fprintf(out,"%03i:   IF ",line);
	if( i.tArg1 == _VAL )
	    fprintf(out,"%i ",i.arg1);
	else
	    fprintf(out,"%03i(%s) ",i.arg1,i.tArg1==_VAR?"SP":"Rx");
	fprintf(out,"%s ",i.op);
	if( i.tArg2 == _VAL )
	    fprintf(out,"%i ",i.arg2);
	else
	    fprintf(out,"%03i(%s) ",i.arg2,i.tArg2==_VAR?"SP":"Rx");
	fprintf(out,"GOTO L_%03i\n",i.arg3);
	break;
    case _GOTO:
	fprintf(out,"%03i:   ",line);
	fprintf(out,"GOTO L_%03i\n",i.arg1);
	break;
    case _PRINT:
	fprintf(out,"%03i:   PRINT ",line);
	if( i.tArg1 == _VAL )
	    fprintf(out,"%i\n",i.arg1);
	else
	    fprintf(out,"%03i(%s)\n",i.arg1,i.tArg1==_VAR?"SP":"Rx");
	break;
    case _ATR:
	fprintf(out,"%03i:   %03i(%s) := ",line,i.arg1,i.tArg1==_VAR?"SP":"Rx");
	if( i.tArg2 == _VAL )
	    fprintf(out,"%i",i.arg2);
	else
	    fprintf(out,"%03i(%s)",i.arg2,i.tArg2==_VAR?"SP":"Rx");
	if( i.tArg3 == _EMPTY )
	    fprintf(out,"\n");
	else {
	    switch(i.op[0]) {
	    case '+' : fprintf(out," ADD "); break;
	    case '-' : fprintf(out," SUB "); break;
	    case '*' : fprintf(out," MUL "); break;
	    case '/' : fprintf(out," DIV "); break;
	    }
	    if( i.tArg3 == _VAL )
		fprintf(out,"%i\n",i.arg3);
	    else
		fprintf(out,"%03i(%s)\n",i.arg3,i.tArg3==_VAR?"SP":"Rx");
	}
	break;
    case _RIDX:
	fprintf(out,"%03i:   %03i(%s) := %03i(SP)(",line,i.arg1,i.tArg1==_VAR?"SP":"Rx",i.arg2);
	if( i.tArg3 == _VAL )
	    fprintf(out,"%i)\n",i.arg3);
	else
	    fprintf(out,"%03i(%s))\n",i.arg3,i.tArg3==_VAR?"SP":"Rx");
	break;
    case _LIDX:
	fprintf(out,"%03i:   %03i(%s)(",line,i.arg1,i.tArg1==_VAR?"SP":"Rx");
	if( i.tArg3 == _VAL )
	    fprintf(out,"%i) := ",i.arg3);
	else
	    fprintf(out,"%03i(%s)) := ",i.arg3,i.tArg3==_VAR?"SP":"Rx");
	if( i.tArg2 == _VAL )
	    fprintf(out,"%i\n",i.arg2);
	else
	    fprintf(out,"%03i(%s)\n",i.arg2,i.tArg2==_VAR?"SP":"Rx");
	break;
    }
    ++line;
}

void print_tac(FILE* out, struct node_tac * code) {
	struct node_tac *list = code;
        if(list) { // se a lista não estiver vazia
		print_inst_tac(out, *list->inst);
		while(list->next != code) { // enquanto houver um próximo elemento
			list = list->next;
			print_inst_tac(out, *list->inst);
		}
	}
}

void append_inst_tac(struct node_tac ** code_ref, struct tac * inst) {
	struct node_tac *list = *code_ref;
	if(!list) { // se a lista estiver vazia
		*code_ref = (struct node_tac *) malloc(sizeof(struct node_tac));
		assert(*code_ref);
		list = *code_ref;
		list->inst = inst;
		list->prev = list->next = list;
	} else { // se a lista não estiver vazia
		list = list->prev; // o último elemento é o anterior ao primeiro
		list->next = (struct node_tac *) malloc(sizeof(struct node_tac));
		assert(list->next);
		list->next->prev = list;
		list = list->next;
		list->next = *code_ref;
		list->inst = inst;
		(*code_ref)->prev = list;
	}
}

void cat_tac(struct node_tac ** code_a, struct node_tac ** code_b) {
  struct node_tac *list_a = *code_a, *list_b = *code_b, *temp;
	if(!list_b) // se a list_b estiver vazia
		return; // a list_a não muda
	if(!list_a) { // se a list_a estiver vazia
		*code_a = list_b; // list_a vira list_b
	} else { // se ambas tiverem elementos
		list_a->prev->next = list_b;
		temp = list_b->prev;
		list_b->prev = list_a->prev;
		list_a->prev = temp;
		temp->next = list_a;
	}
}

#endif
