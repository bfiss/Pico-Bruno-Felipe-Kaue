#include "entry.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

dimList * create_dimList(int l_inferior, int l_superior, int bSize, dimList * son) {
	dimList * novo = malloc(sizeof(dimList));
	novo->l_inferior = l_inferior;
	novo->l_superior = l_superior;
	if( !son ) {
		novo->list = NULL;
		novo->proxTam = bSize;
		novo->dimensoes = 1;
		novo->tamanho = (l_superior - l_inferior)*bSize;
		novo->c_inicial = l_inferior*bSize;
		return novo;
	}
	novo->list = son;
	novo->proxTam = novo->list->tamanho;
	novo->dimensoes = 1 + novo->list->dimensoes;
	novo->tamanho = (l_superior - l_inferior)*novo->proxTam;
	novo->c_inicial = l_inferior*novo->proxTam + novo->list->c_inicial;
	return novo;
}

entry_t * create_entry(char * name, int type, int size, int desloc, void * extra ) {
	entry_t * novo;
	novo = malloc(sizeof(entry_t));
	novo->name = strdup(name);
	novo->type = type;
	novo->size = size;
	novo->desloc = desloc;
	novo->extra = extra;
	return novo;
}
