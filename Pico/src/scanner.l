 /* Secao das declaracoes */

%option noyywrap
  /* Sera copiado no .c final */
%{
    #include <stdlib.h>
    #include <string.h>

    /* este include eh importante... */
    #include "tokens.h"

    extern int lineno;
  
	
		/* Para testes */
		#ifdef __BFK_DO_TESTING__
			#define __BFK_TESTING__ 1
		#else
			#define __BFK_TESTING__ 0
		#endif

		#define YY_NO_INPUT
%}
 
  /* Definicoes regulares (A COMPLETAR conforme a especificacao) */
digito     [0-9]
caractere  [a-zA-Z_]

%%

 /* Secao das Regras de traducao */
 /* O que consta aqui eh puramente ilustrativo... A COMPLETAR ! 
  * Ver tambem "tokens.h".
  */

int    { return (INT); }
double { return (DOUBLE); }
real   { return (FLOAT); }
char   { return (CHAR); }

[-*+/,;:()\[\]{}<>=] { return *yytext; }

\'     { return (QUOTE); }
\"     { return (DQUOTE); }

"<="   { return (LE); }
">="   { return (GE); }
"=="   { return (EQ); }
"!="   { return (NE); }

"&"	   { return (AND); }
"|"    { return (OR); }
"!"    { return (NOT); }

if     { return (IF); }
then   { return (THEN); }
else   { return (ELSE); }
for    { return (FOR); }
next   { return (NEXT); }
while  { return (WHILE); }
repeat { return (REPEAT); }
until  { return (UNTIL); }
end    { return (END); }
true   { return (TRUE); }
false  { return (FALSE); }
print  { return (PRINTF); }

[\n]   { ++lineno; }
[ \t]+ { }
{caractere}({caractere}|{digito})* {  yylval.string=strdup(yytext); return (IDF); }
{digito}+ { yylval.number_int=atoi(yytext); return( INT_LIT ); };
{digito}+\.{digito}+((e|E)(\+|\-)?{digito}+)? { yylval.number_float=atof(yytext); return (F_LIT); }

  /* Tratamento dos erros lexicais: a regra seguinte pega tudo o que nao
   * fechou com uma Regexp anterior.
   */
. { printf("Erro lexical - caractere nao reconhecido: %c. Linha: %i\n", yytext[0],lineno);
    exit(-1); }
%%
 /* Secao dos  Procedimentos auxiliares  */

 /* Para redefinir a entrada padrao do LEX. 
  * Redefinir a variavel 'yyin' para ler de um arquivo. Por exemplo:
     yyin = fopen("Meu_programa.c", "r");
  * ira mandar ler o programa a ser compilado pelo analisador lexical
  * em 'Meu_programa.c'.
  * O default eh ler da entrada standard (o teclado).
  */

void *__BKG_PTR__ = yyunput;
	
#if 0
extern FILE *yyin;
int main(int argc, char* argv[]) {
   int token;
   if (argc != 2) {
     printf("uso: %s <input_file>. Try again!\n", argv[0]);
     exit(-1);
   }
   yyin = fopen(argv[1], "r");
   if (!yyin) {
     printf("Uso: %s <input_file>. Could not find %s. Try again!\n", 
         argv[0], argv[1]);
     exit(-1);
   }
  /* agora a entrada padrao eh o arquivo especificado como 1o argumento ao
   * executavel (argv[1]).
   * Soh chama o analisador lexical default fornecido pelo output do Flex:
   */
  while (token=yylex()) { 
     /* neste laco, obtem-se "palavra por palavra" os tokens reconhecidos
      * pelo scanner fornecido pelo Lex. Poderia ser feita a analise
      * sintatica... Sera feito depois!
      */
		if(token > 255) {
			if(__BFK_TESTING__ && token == INT_LIT)
				printf("%d ", VAL_INT);
			else if(__BFK_TESTING__ && token == F_LIT)
				printf("%g ", VAL_DOUBLE);
			printf("%d\n", token);
		}
		else printf("%c\n", token);
  }
  return(0);
}
#endif
