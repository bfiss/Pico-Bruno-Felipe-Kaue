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
#include "lista.h"

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

#define CLEAN_TMP tmpSize < tmpCount ? (tmpSize = tmpCount) : 0; \
	          tmpCount = 0

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
	int stackSize = 0;
	int tmpSize = 0;
	int tmpCount = 0;
	int increase_symbol_table();

	struct node_tac * gera_tac(Node * node);

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
%token PRINTF

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
          ;

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

fim: '}'                  { stackSize = stackSize < desloc[table_atual] ? desloc[table_atual] : stackSize; --table_atual; }
  ;

comando: lvalue '=' expr	{ $$ = Create_node(lineno, com_node, "Atribuicao", NULL, 2, $1, $3); }
       | enunciado
       ;

lvalue: IDF			{ CHECK_IDF($1); $$ = create_leaf(lineno, idf_node, $1, idf_aux); }
      | IDF '[' listaexpr ']'	{ CHECK_IDF($1); $$ = Create_node(lineno, lvalue_node, "Valor Indexado", NULL, 2, create_leaf(lineno, idf_node, $1, idf_aux), $3); }
      ;

listaexpr: expr				{ $$ = Create_node(lineno, lexpr_node, "expr_list", NULL, 1, $1); }
	   | expr ',' listaexpr		{ $$ = Create_node(lineno, lexpr_node, "expr_list", NULL, 2, $1, $3); }
	   ;

expr: expr '+' expr	{ $$ = Create_node(lineno, plus_node, "+", NULL, 2, $1, $3); }  
    | expr '-' expr     { $$ = Create_node(lineno, minus_node, "-", NULL, 2, $1, $3); }  
    | expr '*' expr     { $$ = Create_node(lineno, mult_node, "*", NULL, 2, $1, $3); }  
    | expr '/' expr     { $$ = Create_node(lineno, div_node, "/", NULL, 2, $1, $3); }  
    | '(' expr ')'	{ $$ = $2; }
    | INT_LIT   	{ $$ = create_int($1); }
    | F_LIT    		{ $$ = create_float($1); }
    | lvalue
    | chamaproc
    ;

chamaproc: IDF '(' listaexpr ')' { CHECK_IDF($1); $$ = Create_node(lineno, proc_node, "ChamaProc", NULL, 2, create_leaf(lineno, idf_node, $1, idf_aux), $3); }
         ;

enunciado: expr
         | IF '(' expbool ')' THEN acoes fiminstcontrole	{ $$ = Create_node(lineno, if_node, "IF", NULL, 3, $3, $6, $7); }
         | WHILE '(' expbool ')' '{' acoes '}'			{ $$ = Create_node(lineno, while_node, "WHILE", NULL, 2, $3, $6); }
         | PRINTF '(' expr ')'					{ $$ = Create_node(lineno, print_node, "PRINT", NULL, 1, $3); }
         ;

fiminstcontrole: END 			{ $$ = NULL; }
               | ELSE acoes END	{ $$ = $2; }
               ;

expbool: TRUE  				{ $$ = create_leaf(lineno, true_node, "TRUE", NULL); }
       | FALSE 				{ $$ = create_leaf(lineno, false_node, "FALSE", NULL); }
       | '(' expbool ')' 		{ $$ = $2; }
       | expbool AND expbool	        { $$ = Create_node(lineno, and_node, "AND", NULL, 2, $1, $3); }
       | expbool OR expbool 	        { $$ = Create_node(lineno, or_node, "OR", NULL, 2, $1, $3); }
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
FILE * output;

int main(int argc, char* argv[]) 
{
   struct node_tac * code;
   lineno = 1;
   if (argc != 4) {
     printf("Usage: %s -o <output_file> <input_file>. Try again!\n", argv[0]);
     exit(-1);
   }
   yyin = fopen(argv[3], "r");
   if (!yyin) {
     printf("Usage: %s -o <output_file> <input_file>. Could not find %s. Try again!\n",
         argv[0], argv[3]);
     exit(-1);
   }
   output = fopen(argv[2], "w");
   if (!output) {
     printf("Usage: %s -o <output_file> <input_file>. Could not find %s. Try again!\n",
         argv[0], argv[2]);
     exit(-1);
   }

   progname = argv[0];
   
   init_table(&s_table[0],NULL);
   desloc[0] = 0;

   if (!yyparse()) {
		printf("OKAY\n");
		stackSize = stackSize < desloc[0] ? desloc[0] : stackSize;
		code = gera_tac(syntax_tree);
		CLEAN_TMP;
		fprintf(output,"%i\n%i\n",stackSize,tmpSize);
		print_tac(output,code);
		/*print_table(s_table[0]);
		print_tree(syntax_tree,0); */
                
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
	desloc[table_atual] = desloc[table_atual-1];
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
		return create_dimList(*((int*)(node->child[0]->attribute)),
                       *((int*)(node->child[1]->attribute))+1, size, NULL);
	return create_dimList(*((int*)(node->child[0]->attribute)),
                            *((int*)(node->child[1]->attribute))+1,
                           size, matrix_info(node->child[2],size));
}

int geraLabel() {
    static int x = 0;
    return x++;
}

int geraTmp() {
    tmpCount += 4;
    return tmpCount-4;
}

dimList * current = NULL;

struct node_tac * gera_tac(Node * node) {
	if(!node)
	    return NULL;
	struct node_tac * a, * b;
	Node * f, * s;
	char op[3];
	int lab[3];
	switch(node->type) {
	case program_node:
	    return gera_tac(node->child[1]);
	case bloc_node:
	    a = gera_tac(node->child[0]);
	    CLEAN_TMP;
	    b = gera_tac(node->child[1]);
	    cat_tac(&a,&b);
	    return a;
	case com_node:
	    a = gera_tac(f=node->child[0]);
	    b = gera_tac(s=node->child[1]);
	    cat_tac(&a,&b);
	    if( s->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              s->att.labelF, geraTmp(), s->att.desloc, s->att.labelT, "("));
		s->att.type = _TMP;
		s->att.desloc = tmpCount-4;
	    }
	    if( f->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_LIDX, _VAR, s->att.type,
	              f->att.labelF, f->att.desloc, s->att.desloc, f->att.labelT, "("));
	    } else {
		append_inst_tac(&a, create_inst_tac(_ATR, _VAR, s->att.type,
		_EMPTY, f->att.desloc, s->att.desloc, 0, "\0\0"));
	    }
	    return a;
	case idf_node:
	    node->att.type = _VAR;
	    node->att.desloc = ((entry_t *)(node->attribute))->desloc;
	    return NULL;
	case lvalue_node:
	    a = gera_tac(f=node->child[0]);
	    node->att.type = _COMPOUND;
	    node->att.desloc = ((entry_t *)(f->attribute))->desloc;
	    current = ((entry_t *)(f->attribute))->extra;
	    b = gera_tac(s=node->child[1]);
	    cat_tac(&a,&b);
	    current = ((entry_t *)(f->attribute))->extra;
	    append_inst_tac(&a, create_inst_tac(_ATR, s->att.type, s->att.type,
	              _VAL, s->att.desloc, s->att.desloc, current->c_inicial, "-"));
	    node->att.labelT = s->att.desloc;
	    node->att.labelF = s->att.type;
	    return a;
	case lexpr_node:
	    if( !current ) {
		printf("Erro ao ler lista de expressoes na linha %i.\n",node->num_line);
		exit(-1);
	    }
	    a = gera_tac(f=node->child[0]);
	    if( f->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              f->att.labelF, geraTmp(), f->att.desloc, f->att.labelT, "("));
		f->att.type = _TMP;
		f->att.desloc = tmpCount-4;
	    }
	    append_inst_tac(&a, create_inst_tac(_ATR, _TMP, f->att.type,
	              _VAL, geraTmp(), f->att.desloc, current->proxTam, "*"));
	    current = current->list;
	    f->att.type = _TMP;
	    f->att.desloc = tmpCount-4;
	    if( node->num_children > 1 ) {
		b = gera_tac(s=node->child[1]);
		cat_tac(&a,&b);
		append_inst_tac(&a, create_inst_tac(_ATR, _TMP, f->att.type,
	              s->att.type, geraTmp(), f->att.desloc, s->att.desloc, "+"));
	    }
	    node->att.type = _TMP;
	    node->att.desloc = tmpCount-4;
	    return a;
	case int_node:
	    node->att.type = _VAL;
	    node->att.desloc = *((int *)(node->attribute));
	    return NULL;
	case float_node:
	    node->att.type = _VAL;
	    node->att.desloc = 0;
	    return NULL;
	case proc_node:
	    node->att.type = _VAL;
	    node->att.desloc = 0;
	    return NULL;
	case plus_node:
	    op[0] = '+';
	    op[1] = '\0';
	    goto expr_com;
	case minus_node:
	    op[0] = '-';
	    op[1] = '\0';
	    goto expr_com;
	case mult_node:
	    op[0] = '*';
	    op[1] = '\0';
	    goto expr_com;
	case div_node:
	    op[0] = '/';
	    op[1] = '\0';
expr_com:
	    a = gera_tac(f=node->child[0]);
	    if( f->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              f->att.labelF, geraTmp(), f->att.desloc, f->att.labelT, "("));
		f->att.type = _TMP;
		f->att.desloc = tmpCount-4;
	    }
	    b = gera_tac(s=node->child[1]);
	    cat_tac(&a,&b);
	    if( s->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              s->att.labelF, geraTmp(), s->att.desloc, s->att.labelT, "("));
		s->att.type = _TMP;
		s->att.desloc = tmpCount-4;
	    }
	    append_inst_tac(&a, create_inst_tac(_ATR, _TMP, f->att.type,
	              s->att.type, geraTmp(), f->att.desloc, s->att.desloc, op));
	    node->att.type = _TMP;
	    node->att.desloc = tmpCount-4;
	    return a;
	case if_node:
	    lab[0] = geraLabel();
	    lab[1] = geraLabel();
	    if( node->child[2] ) {
		lab[2] = geraLabel();
	    }
	    node->child[0]->att.labelT = lab[0];
	    node->child[0]->att.labelF = lab[1];
	    a = gera_tac(f=node->child[0]);
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[0], 0, 0, "\0\0"));
	    b = gera_tac(s=node->child[1]);
	    cat_tac(&a,&b);
	    if( node->child[2] ) {
		append_inst_tac(&a, create_inst_tac(_GOTO, 0, 0, 0, lab[2], 0, 0, "\0\0"));
	    }
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[1], 0, 0, "\0\0"));
	    if( node->child[2] ) {
		b = gera_tac(s=node->child[2]);
		cat_tac(&a,&b);
		append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[2], 0, 0, "\0\0"));
	    }
	    return a;
	case while_node:
	    lab[0] = geraLabel();
	    lab[1] = geraLabel();
	    lab[2] = geraLabel();
	    node->child[0]->att.labelT = lab[1];
	    node->child[0]->att.labelF = lab[2];
	    a = NULL;
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[0], 0, 0, "\0\0"));
	    b = gera_tac(f=node->child[0]);
	    cat_tac(&a,&b);
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[1], 0, 0, "\0\0"));
	    b = gera_tac(s=node->child[1]);
	    cat_tac(&a,&b);
	    append_inst_tac(&a, create_inst_tac(_GOTO, 0, 0, 0, lab[0], 0, 0, "\0\0"));
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[2], 0, 0, "\0\0"));
	    return a;
	case print_node:
	    a = gera_tac(f=node->child[0]);
	    if( f->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              f->att.labelF, geraTmp(), f->att.desloc, f->att.labelT, "("));
		f->att.type = _TMP;
		f->att.desloc = tmpCount-4;
	    }
	    append_inst_tac(&a, create_inst_tac(_PRINT, f->att.type, 0, 0, f->att.desloc,
                                                                          0, 0, "\0\0"));
	    return a;
	case true_node:
	    a = NULL;
	    append_inst_tac(&a, create_inst_tac(_GOTO, 0, 0, 0, node->att.labelT, 0, 0, "\0\0"));
	    return a;
	case false_node:
	    a = NULL;
	    append_inst_tac(&a, create_inst_tac(_GOTO, 0, 0, 0, node->att.labelF, 0, 0, "\0\0"));
	    return a;
	case not_node:
	    node->child[0]->att.labelT = node->att.labelF;
	    node->child[0]->att.labelF = node->att.labelT;
	    return gera_tac(node->child[0]);
	case eq_node:
	    op[0] = '=';
	    op[1] = '=';
	    op[2] = '\0';
	    goto bool_com;
	case neq_node:
	    op[0] = '!';
	    op[1] = '=';
	    op[2] = '\0';
	    goto bool_com;
	case inf_node:
	    op[0] = '<';
	    op[1] = '\0';
	    goto bool_com;
	case sup_node:
	    op[0] = '>';
	    op[1] = '\0';
	    goto bool_com;
	case inf_eq_node:
	    op[0] = '<';
	    op[1] = '=';
	    op[2] = '\0';
	    goto bool_com;
	case sup_eq_node:
	    op[0] = '>';
	    op[1] = '=';
	    op[2] = '\0';
bool_com:
	    a = gera_tac(f=node->child[0]);
	    if( f->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              f->att.labelF, geraTmp(), f->att.desloc, f->att.labelT, "("));
		f->att.type = _TMP;
		f->att.desloc = tmpCount-4;
	    }
	    b = gera_tac(s=node->child[1]);
	    cat_tac(&a,&b);
	    if( s->att.type == _COMPOUND ) {
		append_inst_tac(&a, create_inst_tac(_RIDX, _TMP, _VAR,
	              s->att.labelF, geraTmp(), s->att.desloc, s->att.labelT, "("));
		s->att.type = _TMP;
		s->att.desloc = tmpCount-4;
	    }
	    append_inst_tac(&a, create_inst_tac(_IF, f->att.type, s->att.type, 0,
                             f->att.desloc, s->att.desloc, node->att.labelT, op));
	    append_inst_tac(&a, create_inst_tac(_GOTO, 0, 0, 0, node->att.labelF, 0, 0, "\0\0"));
	    return a;
	case and_node:
	    lab[0] = geraLabel();
	    f = node->child[0];
	    f->att.labelT = lab[0];
	    f->att.labelF = node->att.labelF;
	    a = gera_tac(f);
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[0], 0, 0, "\0\0"));
	    f = node->child[1];
	    f->att.labelT = node->att.labelT;
	    f->att.labelF = node->att.labelF;
	    b = gera_tac(f);
	    cat_tac(&a,&b);
	    return a;
	case or_node:
	    lab[0] = geraLabel();
	    f = node->child[0];
	    f->att.labelF = lab[0];
	    f->att.labelT = node->att.labelT;
	    a = gera_tac(f);
	    append_inst_tac(&a, create_inst_tac(_LAB, 0, 0, 0, lab[0], 0, 0, "\0\0"));
	    f = node->child[1];
	    f->att.labelT = node->att.labelT;
	    f->att.labelF = node->att.labelF;
	    b = gera_tac(f);
	    cat_tac(&a,&b);
	    return a;
	}
	return NULL;
}


	
