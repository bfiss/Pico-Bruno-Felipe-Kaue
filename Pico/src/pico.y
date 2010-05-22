%{
  /* Aqui, pode-se inserir qualquer codigo C necessario ah compilacao
   * final do parser. Sera copiado tal como esta no inicio do y.tab.c
   * gerado por Yacc.
   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "symbol_table.h"

#define UNDEFINED_SYMBOL_ERROR -21
#define NESTED_LIMIT_EXCEEDED  -22
#define MAX_SIZE 20

#define CHECK_IDF(name) \
	idf_aux = lookup(s_table[table_atual], name); \
	if(idf_aux == NULL) \
	{ \
		printf("UNDEFINED SYMBOL. A variavel %s nao foi declarada.\n", name); \
		return( UNDEFINED_SYMBOL_ERROR ); \
	}

	int yylex(void);
	void yyerror(char *);

	void print(char *s, char *t);
	char *cat(char *s, char *t, char *c);

	Node* create_int(int x);
	Node* create_float(float x);
	Node* create_idf(char* name);
	
	int process_decl(Node * node);
	int type_size(Node * node);
	dimList * matrix_info(Node * node, int size);
	
	#define STR_SIZE 1000
	char str[STR_SIZE];

	int lineno, tmp;

	Node* syntax_tree = NULL;

	entry_t* idf_aux;

	symbol_t s_table[MAX_SIZE];
	int desloc[MAX_SIZE];
	int table_atual = 0;
	int increase_symbol_table();

%}

%union {
	char *string;
	int number_int;
	float number_float;
	void* leaf;
}

%type <leaf> tipounico tipolista tipo listadeclaracao declaracao declaracoes expr listaexpr listadupla
%type <leaf> acoes comando lvalue chamaproc enunciado expbool fiminstcontrole code bloco

%token <string> IDF 
%token INT
%token FLOAT
%token DOUBLE
%token CHAR
%token QUOTE
%token DQUOTE
%token LE
%token GE
%token EQ
%token NE
%token AND
%token OR
%token NOT
%token IF
%token THEN
%token ELSE
%token WHILE
%token <number_int> INT_LIT
%token <number_float> F_LIT
%token END
%token TRUE
%token FALSE
%token FOR
%token NEXT
%token REPEAT
%token UNTIL

%start code
%left OR
%left AND
%left NOT
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'

 /* A completar com seus tokens - compilar com 'yacc -d' */

%%
code: declaracoes acoes { syntax_tree = Create_node(lineno, program_node, "code", s_table[table_atual], 2, $1, $2); $$ = syntax_tree; }
    | acoes { syntax_tree = $1; $$ = syntax_tree; }
    ;

declaracoes: declaracao ';'	{ process_decl($1); $$ = $1; }
           | declaracoes declaracao ';'	{ process_decl($2); $$ = Create_node(lineno, decs_node, "declaracoes", NULL, 2, $1, $2); }
           ;

declaracao: listadeclaracao ':' tipo	{ $$ = Create_node(lineno, decl_node, "declaracao", NULL, 2, $1, $3); }

listadeclaracao: IDF 				{ $$ = Create_node(lineno, decl_list_node, "listadeclaracao", NULL, 1, create_leaf(lineno, idf_node, $1, NULL)); }
               | IDF ',' listadeclaracao	{ $$ = Create_node(lineno, decl_list_node, "listadeclaracao", NULL, 2, create_leaf(lineno, idf_node, $1, NULL), $3); }
               ;

tipo: tipounico
    | tipolista
    ;

tipounico: INT 	{ $$ = create_leaf(lineno, tipou_node, "INT", NULL); }
         | FLOAT	{ $$ = create_leaf(lineno, tipou_node, "FLOAT", NULL); }
         | DOUBLE	{ $$ = create_leaf(lineno, tipou_node, "DOUBLE", NULL); }
         | CHAR 	{ $$ = create_leaf(lineno, tipou_node, "CHAR", NULL); }
         ;

tipolista: INT '[' listadupla ']'		{ $$ = Create_node(lineno, tipol_node, "INT", NULL, 1, $3); }
         | FLOAT '[' listadupla ']'		{ $$ = Create_node(lineno, tipol_node, "FLOAT", NULL, 1, $3); }
         | DOUBLE '[' listadupla ']'	{ $$ = Create_node(lineno, tipol_node, "DOUBLE", NULL, 1, $3); }
         | CHAR '[' listadupla ']'		{ $$ = Create_node(lineno, tipol_node, "CHAR", NULL, 1, $3); }
         ;

listadupla: INT_LIT ':' INT_LIT { $$ = Create_node(lineno, listad_node, "listadupla", NULL, 2, create_int($1), create_int($3)); }
          | INT_LIT ':' INT_LIT ',' listadupla { $$ = Create_node(lineno, listad_node, "listadupla", NULL, 3, create_int($1), create_int($3), $5); }
          ;

acoes: comando ';'                                                               
    | comando ';' acoes              { $$ = Create_node(lineno, bloc_node, "acoes_list", NULL, 2, $1, $3); }
    | bloco
    | bloco acoes                    { $$ = Create_node(lineno, bloc_node, "acoes_list", NULL, 2, $1, $2); }
    ;

bloco: inicio code fim    { $$ = $2; }
    ;

inicio: '{'               { tmp = increase_symbol_table(); if(tmp)return tmp; }
     ;

fim: '}'                  { --table_atual; }
  ;

comando: lvalue '=' expr	{ $$ = Create_node(lineno, com_node, "Atribuicao", NULL, 2, $1, $3); }
       | enunciado
       ;

lvalue: IDF							{ CHECK_IDF($1); $$ = create_leaf(lineno, idf_node, $1, idf_aux); }
      | IDF '[' listaexpr ']'	{ CHECK_IDF($1); $$ = Create_node(lineno, lvalue_node, "Valor Indexado", NULL, 2, create_leaf(lineno, idf_node, $1, idf_aux), $3); }
      ;

listaexpr: expr				
	   | expr ',' listaexpr		{ $$ = Create_node(lineno, lexpr_node, "expr_list", NULL, 2, $1, $3); }
	   ;

expr: expr '+' expr	{ $$ = Create_node(lineno, plus_node, "+", NULL, 2, $1, $3); }  
    | expr '-' expr  { $$ = Create_node(lineno, minus_node, "-", NULL, 2, $1, $3); }  
    | expr '*' expr  { $$ = Create_node(lineno, mult_node, "*", NULL, 2, $1, $3); }  
    | expr '/' expr  { $$ = Create_node(lineno, div_node, "/", NULL, 2, $1, $3); }  
    | '(' expr ')'	{ $$ = $2; }
    | INT_LIT   		{ $$ = create_int($1); }
    | F_LIT    		{ $$ = create_float($1); }
    | lvalue
    | chamaproc
    ;

chamaproc: IDF '(' listaexpr ')' { CHECK_IDF($1); $$ = Create_node(lineno, proc_node, "ChamaProc", NULL, 2, create_leaf(lineno, idf_node, $1, idf_aux), $3); }
         ;

enunciado: expr
         | IF '(' expbool ')' THEN acoes fiminstcontrole		{ $$ = Create_node(lineno, if_node, "IF", NULL, 3, $3, $6, $7); }
         | WHILE '(' expbool ')' '{' acoes '}'					{ $$ = Create_node(lineno, while_node, "WHILE", NULL, 2, $3, $6); }
         ;

fiminstcontrole: END 			{ $$ = NULL; }
               | ELSE acoes END	{ $$ = $2; }
               ;

expbool: TRUE  					{ $$ = create_leaf(lineno, true_node, "TRUE", NULL); }
       | FALSE 					{ $$ = create_leaf(lineno, false_node, "FALSE", NULL); }
       | '(' expbool ')' 		{ $$ = $2; }
       | expbool AND expbool	{ $$ = Create_node(lineno, and_node, "AND", NULL, 2, $1, $3); }
       | expbool OR expbool 	{ $$ = Create_node(lineno, or_node, "OR", NULL, 2, $1, $3); }
       | NOT expbool 			{ $$ = Create_node(lineno, not_node, "NOT", NULL, 1, $2); }
       | expr '>' expr			{ $$ = Create_node(lineno, sup_node, ">", NULL, 2, $1, $3); }
       | expr '<' expr 			{ $$ = Create_node(lineno, inf_node, "<", NULL, 2, $1, $3); }
       | expr LE expr   		{ $$ = Create_node(lineno, inf_eq_node, "LE", NULL, 2, $1, $3); }
       | expr GE expr   		{ $$ = Create_node(lineno, sup_eq_node, "GE", NULL, 2, $1, $3); }
       | expr EQ expr  			{ $$ = Create_node(lineno, eq_node, "EQ", NULL, 2, $1, $3); }
       | expr NE expr  			{ $$ = Create_node(lineno, neq_node, "!=", NULL, 2, $1, $3); }
       ;
%%
 /* A partir daqui, insere-se qlqer codigo C necessario.
  */

char* progname;
extern FILE* yyin;

int main(int argc, char* argv[]) 
{
   lineno = 1;
   if (argc != 2) {
     printf("Usage: %s <input_file>. Try again!\n", argv[0]);
     exit(-1);
   }
   yyin = fopen(argv[1], "r");
   if (!yyin) {
     printf("Usage: %s <input_file>. Could not find %s. Try again!\n", 
         argv[0], argv[1]);
     exit(-1);
   }

   progname = argv[0];
   
   init_table(&s_table[0],NULL);
   desloc[0] = 0;

   if (!yyparse()) {
		printf("OKAY\n");
		print_table(s_table[0]);
		print_tree(syntax_tree,0);
  } else 
      printf("ERROR\n");

   return(0);
}

void yyerror(char* s) {
  fprintf(stderr, "%s: %s", progname, s);
  fprintf(stderr, "line %d\n", lineno);
}

Node* create_int(int x) {
	int * ad = malloc(sizeof(int));
	*ad = x;
	return create_leaf(lineno, int_node, "INT", ad);
}

Node* create_float(float x) {
	int * ad = malloc(sizeof(float));
	*ad = x;
	return create_leaf(lineno, float_node, "FLOAT", ad);
}

int increase_symbol_table() {
	if( ++table_atual >= MAX_SIZE ) {
		printf("Error: maximum block nestedness exceeded.\n");
		return NESTED_LIMIT_EXCEEDED;
	}
	desloc[table_atual] = 0;
	init_table(&s_table[table_atual],s_table[table_atual-1]);
        return 0;
}

int process_decl(Node * node) {
	int size;
	Node * aux;
	dimList * extra = NULL;
	assert(node);
	size = type_size(node->child[1]);
	if( node->child[1]->type == tipol_node ) {
		extra = matrix_info(node->child[1]->child[0],size);
		size = extra->tamanho;
	}
	aux = node->child[0];
	while( aux->num_children == 2 ) {
		if(!insert(&s_table[table_atual], create_entry(aux->child[0]->lexeme,node->child[1]->lexeme[0],size,desloc[table_atual],extra)))
                    desloc[table_atual] += size;
		aux = aux->child[1];
	}
	if(!insert(&s_table[table_atual], create_entry(aux->child[0]->lexeme,node->child[1]->lexeme[0],size,desloc[table_atual],extra)))
            desloc[table_atual] += size;
	return desloc[table_atual];
}

int type_size(Node * node) {
	assert(node);
	switch( node->lexeme[0] ) {
	case TCHAR:
		return 1;
	case TINTEGER:
	case TFLOAT:
		return 4;
	case TDOUBLE:
		return 8;
	}
	return 0;
}
	
dimList * matrix_info(Node * node, int size) {
	if( node->num_children == 2 )
		return create_dimList(*((int*)(node->child[0]->attribute)), *((int*)(node->child[1]->attribute))+1, size, NULL);
	return create_dimList(*((int*)(node->child[0]->attribute)), *((int*)(node->child[1]->attribute))+1, size, matrix_info(node->child[2],size));
}

	
