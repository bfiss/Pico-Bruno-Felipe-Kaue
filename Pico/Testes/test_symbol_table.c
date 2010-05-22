#include "../src/symbol_table.h"
#include <assert.h>
#include <time.h>

int main()
{

	int i,j;
	char* str_test;
	symbol_t table;

	entry_t entry[26000];

	static FILE *devnull;
	devnull = fopen("/dev/null", "w");

	str_test = (char*) malloc(sizeof(char)*2);

	for(i=0;i<40;i++)
		entry[i].name = (char*) malloc(sizeof(char)*40);

	for(i=0;i<26000;i++)
		entry[i].name = (char*) malloc(sizeof(char)*5);

/*
BATERIA DE TESTES 2 - ENTRADAS DIVERSAS
*/

	strcpy(entry[0].name, "aux");
	strcpy(entry[1].name, "teste");
	strcpy(entry[2].name, "var1");
	strcpy(entry[3].name, "var2");
	strcpy(entry[4].name, "_testando");
	strcpy(entry[5].name, "ele23_we");
	strcpy(entry[6].name, "i");
	strcpy(entry[7].name, "j");
	strcpy(entry[8].name, "k");
	strcpy(entry[9].name, "coun");
	strcpy(entry[10].name, "n_var");
	strcpy(entry[11].name, "table");
	strcpy(entry[12].name, "pointer");
	strcpy(entry[13].name, "pNodo");
	strcpy(entry[14].name, "AUX");
	strcpy(entry[15].name, "a123");
	strcpy(entry[16].name, "idf");
	strcpy(entry[17].name, "var_new");
	strcpy(entry[18].name, "force");
	strcpy(entry[19].name, "ac");
	strcpy(entry[20].name, "vel");
	strcpy(entry[21].name, "printTableRLC");
	strcpy(entry[22].name, "aux1");
	strcpy(entry[23].name, "aux2");
	strcpy(entry[24].name, "aux3");
	strcpy(entry[25].name, "aux4");
	strcpy(entry[26].name, "zk_98e_23dashd");
	strcpy(entry[27].name, "variavel_com_um_nome_bem_grande");
	strcpy(entry[28].name, "a");
	strcpy(entry[29].name, "v0123456789032344");
	strcpy(entry[30].name, "_1234");
	strcpy(entry[31].name, "iterator");
	strcpy(entry[32].name, "esq");
	strcpy(entry[33].name, "dir");
	strcpy(entry[34].name, "fil");
	strcpy(entry[35].name, "_end");
	strcpy(entry[36].name, "str_test");
	strcpy(entry[37].name, "compil");
	strcpy(entry[38].name, "djTENRS");
	strcpy(entry[39].name, "SSSW_");

	init_table(&table);

	assert(print_table(table) == 0);

	for(i=0;i<40;i++)
	{
		insert(&table, &entry[i]);
		assert(print_table(table) == (i+1));
		printf("\n");
		assert(lookup(table, entry[i].name) == &entry[i]);
		for(j=0;j<i;j++)
		{
			assert(lookup(table, entry[j].name) == &entry[j]);
		}
	}

	free_table(&table);
	free_table(NULL);

/*
BATERIA DE TESTES 1
*/

	for(i=0;i<26;i++)
	{
		entry[i].name[0] = 97+i;
		entry[i].name[1] = '\0';
	}

	for(i=26;i<52;i++)
	{
		entry[i].name[0] = 97+i-26;
		entry[i].name[1] = '\0';
	}

	init_table(&table);

	assert(print_table(table) == 0);

	for(i=0;i<26;i++)
	{
		insert(&table, &entry[i]);
		assert(print_table(table) == (i+1));
		printf("\n");
		str_test[0] = 97+i;
		str_test[1] = '\0';
		assert(lookup(table, str_test) == &entry[i]);
		for(j=0;j<i;j++)
		{
			str_test[0] = 97+j;
			str_test[1] = '\0';
			assert(lookup(table, str_test) == &entry[j]);
		}
	}

/*
TENTANDO ADICIONAR TUDO NOVAMENTE
*/

	for(i=26;i<52;i++)
	{
		insert(&table, &entry[i]);
		assert(print_table(table) == 26);
		printf("\n");
		str_test[0] = 97+i-26;
		str_test[1] = '\0';
		assert(lookup(table, str_test) == &entry[i-26]);
	}

	free_table(&table);
	free_table(NULL);

/*
BATERIA DE TESTES 2 - ALEATORIAMENTE
*/

	init_table(&table);

	assert(print_table(table) == 0);

	for(j=0;j<5000;j++)
	{
		i = j%26;
		insert(&table, &entry[i]);
		//assert(print_file_table(devnull,table) != 0);
		str_test[0] = 97+i;
		str_test[1] = '\0';
		assert(lookup(table, str_test) == &entry[i]);
	}

	free_table(&table);
	free_table(NULL);


/*
BATERIA DE TESTES 3 - HASH + NODO
*/

	for(i=0;i<26;i++)
	{
		for(j=0;j<4;j++)
		{
			entry[i*4+j].name[0] = 97+i;
			entry[i*4+j].name[1] = 97+j;
			entry[i*4+j].name[2] = '\0';
		}
	}

	for(i=26;i<52;i++)
	{
		for(j=0;j<4;j++)
		{
			entry[i*4+j].name[0] = 97+i-26;
			entry[i*4+j].name[1] = 97+j;
			entry[i*4+j].name[2] = '\0';
		}
	}

	init_table(&table);

	assert(print_table(table) == 0);

	for(i=0;i<(26*4);i++)
	{
		insert(&table, &entry[i]);
		assert(print_file_table(devnull,table) == (i+1));
		assert(lookup(table, entry[i].name) == &entry[i]);
		for(j=0;j<i;j++)
			assert(lookup(table, entry[j].name) == &entry[j]);
	}

/*
TENTANDO ADICIONAR TUDO NOVAMENTE
*/

	for(i=(26*4);i<(52*4);i++)
	{
		insert(&table, &entry[i]);
		assert(print_file_table(devnull,table) == (26*4));
		assert(lookup(table, entry[i].name) == &entry[i-(26*4)]);
	}

	free_table(&table);
	free_table(NULL);

/*
BATERIA DE TESTES 4 - Tentando exaustivamente
*/

	init_table(&table);

	assert(print_table(table) == 0);

	srand( time(NULL) );

	for(i=0;i<26;i++)
	{
		for(j=0;j<1000;j++)
		{
            entry[i*1000+j].name[0] = 97+i;
			entry[i*1000+j].name[1] = 97 + (rand()*j)%23;
			entry[i*1000+j].name[2] = 97 + (rand()*j)%23 + 1;
			entry[i*1000+j].name[3] = 97 + (rand()*j)%23 + 2;
			entry[i*1000+j].name[3] = '\0';
		}
	}

	for(i=0;i<26000;i++)
	{
		insert(&table, &entry[i]);
		//assert(print_file_table(devnull,table) != 0);
		if((i%1000)==0)
			assert(lookup(table, entry[i].name) == &entry[i]);
	}

	free_table(&table);
	free_table(NULL);

	return 0;
}
