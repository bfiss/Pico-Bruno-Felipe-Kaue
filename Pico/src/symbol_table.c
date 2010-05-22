#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

unsigned long hash(char *str)
{
		unsigned long hash = 5381;
		int c;

		while ((c = *str++) != '\0')
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash % TABLE_SIZE;
}

int init_table(symbol_t* table, symbol_t father)
{
	*table = (symbol_t) malloc(sizeof(s_symbol_t));
	assert(*table);
	memset(*table, 0, sizeof(s_symbol_t));		//colocando tudo NULL nao precisa inicializar nodo da arvore
	(*table)->father = father;
	return 0;
}

void free_table(symbol_t* table)
{
	int i;

	if(table && *table) {
		for(i=0;i<TABLE_SIZE;i++)
			destroiAVL((*table)->entry[i]);
		free(*table);
		*table = NULL;
	}
}

entry_t* lookup(symbol_t table, char* name)
{
	unsigned long hash_value = hash(name);
	pNodoA aux;
	int find=0;
	
	if( !table )
		return NULL;

	aux = findNodo(table->entry[hash_value],name,&find);

	if(!find)
		return lookup(table->father,name);
	else
		return aux->entry;
}

int insert(symbol_t* table, entry_t* entry)
{
        int success;
	unsigned long hash_value = hash(entry->name);

	(*table)->entry[hash_value] = insereAVL((*table)->entry[hash_value], entry, &success);

        if( !success )
            return -1;

	return 0;
}

int print_table(symbol_t table)
{
	return print_file_table(stdout, table);
}

int print_file_table(FILE* out, symbol_t table)
{
	int i, res = 0;
	for(i = 0; i < TABLE_SIZE; i++)
		if(table->entry[i])
		{
			imprimeAVL(out, table->entry[i], &res);
			/*fprintf(out, "\n");*/
		}

	return res;
}
