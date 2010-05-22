#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef _LISTA_C_
#define _LISTA_C_

// macro para inicialização dos membros da struct tac *
#define INICIALIZA(x) t->x = (char*)malloc(sizeof(char)*(1+strlen(x))); \
	 assert(t->x); \
   strcpy(t->x, x);

struct tac* create_inst_tac(const char* res, const char* arg1, 
                 const char* op, const char* arg2) {
   struct tac * t = (struct tac *)malloc(sizeof(struct tac));
	 assert(t);
   INICIALIZA(op);
   INICIALIZA(res);
   INICIALIZA(arg1);
   INICIALIZA(arg2);
   return t;
}

void print_inst_tac(FILE* out, struct tac i) {
   fprintf(out, "%s\t:= %s %s %s\n", i.res, i.arg1, i.op, i.arg2);
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
