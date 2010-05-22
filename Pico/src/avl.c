#include "avl.h"
#include <assert.h>

extern int lineno;

pNodoA inicializaAVL()
{
	return NULL;
}

void destroiAVL(pNodoA arv)
{
	if(!NodoVazio(arv))
	{
		destroiAVL(arv->esq);
		destroiAVL(arv->dir);
		/*free(arv->entry);   este free depende de como a estrutura será utilizada, no nosso caso estamos alocando na pilha as variáveis entry por isso não dá para dar free aqui
		                         �(eh) bom evitar comentarios de c++ em c */
		free(arv);
		arv=NULL;
	}
}

pNodoA insereAVL(pNodoA arv, entry_t* simbol, int * success)
{
	if(NodoVazio(arv))
	{
		arv = (pNodoA) malloc(sizeof(hash_entry_t));
		assert(arv);
		arv->entry = simbol;
		arv->esq = NULL;
		arv->dir = NULL;
		arv->fatorB = 0;

                *success = 1;
		return arv;
	}

	if( strcmp(simbol->name, arv->entry->name) < 0 )
	{
		arv->esq = insereAVL(arv->esq, simbol,success);
		arv->fatorB = calculaFatorB(arv);
		if (arv->fatorB == 2)
			arv = balanceamentoEsq(arv);
	}
	else if( strcmp(simbol->name, arv->entry->name) > 0 )
	{
		arv->dir = insereAVL(arv->dir, simbol,success);
		arv->fatorB = calculaFatorB(arv);
		if (arv->fatorB == -2)
			arv = balanceamentoDir(arv);
	}else
	{
		printf("Warning: Variable %s on a definition ending at line %i could not be redefined\n",arv->entry->name,lineno);
                *success = 0;
		return arv;
	}

	return arv;
}


pNodoA findNodo(pNodoA arv, char* name, int* find)
{
	if(arv==NULL)
	{
		*find=0;
		return arv;
	}

	if( strcmp(name, arv->entry->name) < 0 )
	{
		arv=findNodo(arv->esq, name,find);
	}else if( strcmp(name, arv->entry->name) > 0 )
	{
		arv=findNodo(arv->dir, name,find);
	}else
	{
		*find=1;
	}

	return arv;
}

pNodoA rotEsquerda(pNodoA arv)
{
	pNodoA aux;

	aux = arv->dir;
	arv->dir = aux->esq;
	aux->esq = arv;

	aux->fatorB = 0;

	return aux;
}

pNodoA rotDireita(pNodoA arv)
{
	pNodoA aux;

	aux = arv->esq;
	arv->esq = aux->dir;
	aux->dir = arv;

	aux->fatorB = 0;

	return aux;
}

pNodoA rotDupEsquerda(pNodoA arv)
{
	pNodoA aux1 = arv->dir;
	pNodoA aux2 = aux1->esq;

	aux1->esq = aux2->dir;
	aux2->dir = aux1;
	arv->dir = aux2->esq;
	aux2->esq = arv;

	if(aux2->fatorB == -1)
		arv->fatorB = 1;
	else
		arv->fatorB = 0;

	if(aux2->fatorB == 1)
		aux1->fatorB = -1;
	else
		aux1->fatorB = 0;

	return aux2;
}

pNodoA rotDupDireita(pNodoA arv)
{
	pNodoA aux1 = arv->esq;
	pNodoA aux2 = aux1->dir;

	aux1->dir = aux2->esq;
	aux2->esq = aux1;
	arv->esq = aux2->dir;
	aux2->dir = arv;

	if(aux2->fatorB == 1)
		arv->fatorB = -1;
	else
		arv->fatorB = 0;

	if(aux2->fatorB == -1)
		aux1->fatorB = 1;
	else
		aux1->fatorB = 0;

	return aux2;
}

int calculaFatorB(pNodoA arv)
{
	return (altura(arv->esq) - altura(arv->dir));
}

pNodoA balanceamentoEsq(pNodoA arv)
{
	pNodoA f_esq;
	pNodoA f_dir;
	f_esq = arv->esq;
	f_dir = arv->dir;

	if(f_esq->fatorB == 1)
		arv = rotDireita(arv);
	else
		arv = rotDupDireita(arv);

	arv->fatorB = 0;
	return arv;
}

pNodoA balanceamentoDir(pNodoA arv)
{
	pNodoA f_dir;
	pNodoA f_esq;
	f_dir = arv->dir;
	f_esq = arv->esq;

	if(f_dir->fatorB == -1)
		arv = rotEsquerda(arv);
	else
		arv = rotDupEsquerda(arv);

	arv->fatorB = 0;
	return arv;
}

void imprimeAVL(FILE* out, pNodoA arv, int *n_nodo)
{
	dimList * aux;
	if(!NodoVazio(arv))
	{
		imprimeAVL(out, arv->esq, n_nodo);
		fprintf(out, "name: %s \t type: %c \t is matrix: %i \t desloc = %i\n", arv->entry->name, arv->entry->type, !!(arv->entry->extra), arv->entry->desloc);
		if( (aux = arv->entry->extra) ) {
		    while( aux ) {
			printf(" Dimension %i \t Size %i \t C_A %i \t l %i : %i \t nextSize: %i \t desloc = %i\n",aux->dimensoes,aux->tamanho,aux->c_inicial,aux->l_inferior,aux->l_superior,aux->proxTam, arv->entry->desloc);
                        aux = aux->list;
                    }
		}
		(*n_nodo)++;
		imprimeAVL(out, arv->dir, n_nodo);
	}
}

void printNiveis(pNodoA arv, int count)
{
	int i;

	if (arv!= NULL)
	{
		for(i=0; i<count; i++)
			printf("=");
		printf("%s\n",arv->entry->name);

		if(arv->esq != NULL)
			printNiveis(arv->esq, count+1);

		if(arv->dir != NULL)
			printNiveis(arv->dir, count+1);
	}
}

int NodoVazio(pNodoA arv)
{
	return (int)!arv;
}

int altura(pNodoA arv)
{
	int esq,dir;
	if(NodoVazio(arv))
		return 0;
	else {
		esq = altura(arv->esq);
		dir = altura(arv->dir);
		if( esq > dir )
			return 1 + esq;
	}

	return 1 + dir;
}
