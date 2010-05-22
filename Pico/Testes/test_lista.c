#include "../src/lista.h"
#include <string.h>
#include <stdlib.h>

void print_tac_inverse(FILE* out, struct node_tac * code) {
	struct node_tac *list = code;
  if(list) {
		while(list->prev != code){
			list = list->prev;
			print_inst_tac(out, *list->inst);
		}
		list = list->prev;
		print_inst_tac(out, *list->inst);
	}
}

int main()
{

	int i;

	char* _res;
	char*	_arg1; 
	char*	_op;
	char*	_arg2;

	static FILE *devnull;
	devnull = fopen("/dev/null", "w");

/*
BATERIA DE TESTES 0
*/

	struct node_tac *lista = NULL, *listb = NULL;
	
	printf("TESTES 0\n\n");

	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);
	
	append_inst_tac(&lista, create_inst_tac("res", "arg1", "op", "arg2"));
	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);
	printf("\n");
	
	append_inst_tac(&lista, create_inst_tac("res1", "arg11", "op1", "arg21"));
	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);
	printf("\n");
	
	cat_tac(&lista, &listb);
	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);
	printf("\n");
	
	cat_tac(&listb, &lista);
	print_tac(stdout, listb);
	print_tac_inverse(stdout, listb);
	printf("\n");
	
	lista = NULL;
	append_inst_tac(&lista, create_inst_tac("resA", "arg1A", "opA", "arg2A"));
	append_inst_tac(&lista, create_inst_tac("res1A", "arg11A", "op1A", "arg21A"));
	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);
	printf("\n");
	
	cat_tac(&listb, &lista);
	print_tac(stdout, listb);
	print_tac_inverse(stdout, listb);
	printf("\n");

/*
BATERIA DE TESTES 1
*/
	
	printf("TESTES 1\n\n");

	_res = (char*) malloc(sizeof(char)*7);
	strcpy(_res, "res0");
	_arg1 =  (char*) malloc(sizeof(char)*7);
	strcpy(_arg1, "arg0");
	_op = (char*) malloc(sizeof(char)*7);
	strcpy(_op, "op0");
	_arg2 = (char*) malloc(sizeof(char)*7);
	strcpy(_arg2, "arg0");

	struct tac* inst[79];

	for(i=0;i<79;i++)
	{
		_res[3]=(i+48);
		_arg1[3]=(i+48);
		_op[2]=(i+48);
		_arg2[3]=(i+48);
		inst[i] = create_inst_tac(_res, _arg1, _op, _arg2);
		print_inst_tac(devnull, *inst[i]);
	}

	lista = NULL;
	listb = NULL;

	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);

	cat_tac(&lista, &listb);

	print_tac(stdout, lista);
	print_tac_inverse(stdout, lista);

	for(i=0;i<79;i++)
		append_inst_tac(&lista, inst[i]);

	//print_tac(stdout, lista);
	//printf("\n");
	//print_tac_inverse(stdout, lista);

	cat_tac(&lista, &listb);
	//print_tac(stdout, lista);

	for(i=0;i<79;i++)
		append_inst_tac(&listb, inst[i]);

	cat_tac(&lista, &listb);
	print_tac(stdout, lista);
	printf("\n");
	print_tac_inverse(stdout, lista);

	return 0;
}
