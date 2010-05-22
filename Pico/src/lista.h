/** @file lista.h
 */

#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdio.h>

enum tacType { _LAB, _IF, _GOTO, _ATR, _PRINT, _LIDX, _RIDX };
enum opType  { _VAR, _TMP, _VAL, _EMPTY, _COMPOUND };

/** 
 * Estrutura de dados (que implementa uma intrucao TAC, ver Etapa 4), a ser
 * encadeada na lista. Por enquanto, o que representa é irrelevante.
 */
struct tac {
   enum tacType type;
   enum opType  tArg1;
   enum opType  tArg2;
   enum opType  tArg3;
   int     arg1;
   int     arg2;
   int     arg3;
   char    op[3];
};

/** \brief  Construtor de Instrucao TAC 
 *
 * Para testes, pode-se usar qualquer string em argumentos.
 * @param type  tipo da instru��o.
 * @param tArg1 um tipo de operando.
 * @param tArg2 um tipo de operando.
 * @param tArg3 um tipo de operando.
 * @param arg1  um int.
 * @param arg2  um int.
 * @param arg3  um int.
 * @param op    um char*.
 * @return um ponteiro sobre uma 'struct tac'.
 */
struct tac* create_inst_tac(const enum tacType type, const enum opType tArg1,
       const enum opType tArg2, const enum opType tArg3, const int arg1,
                       const int arg2, const int arg3, const char op[2]);

/** \brief Funcao que imprime o conteudo de uma instrucao TAC 
 *
 * @param out um ponteiro sobre um arquivo (aberto) aonde ira ser escrita a instrucao.
 * @param i a instrucao a ser impressa.
 */
void print_inst_tac(FILE* out, struct tac i);

/** Um elemento basico da lista. O campo 'inst' aponta para a informacao a ser
 * armazenada em um elemento da lista. O resto serve para implementar a lista
 * e seus metodos.
 */
struct node_tac {
   struct tac * inst; /**< informação a ser armazenada */
   struct node_tac* next; /**< nó anterior */
   struct node_tac* prev; /**< próximo nó */
};

/** \brief Imprime no arquivo apontado por 'out' o conteudo da lista apontada
 * por 'code'.
 *
 * @param out um ponteiro sobre um arquivo (aberto) aonde ira ser escrita a lista (uma linha por elemento).
 * @param code o ponteiro para a lista a ser impressa.
 */
void print_tac(FILE* out, struct node_tac * code);

/** Insere no fim da lista 'code' o elemento 'inst'. 
 * @param code lista (possivelmente vazia) inicial, em entrada. Na saida, contem
 *         a mesma lista, com mais um elemento inserido no fim.
 * @param inst  o elemento inserido no fim da lista.
 */
void append_inst_tac(struct node_tac ** code, struct tac * inst);

/** Concatena a lista 'code_a' com a lista 'code_b'.
 * @param code_a lista (possivelmente vazia) inicial, em entrada. Na saida, contem 
 *         a mesma lista concatenada com 'code_b'.
 * @param code_b a lista concatenada com 'code_a'.
 */
void cat_tac(struct node_tac ** code_a, struct node_tac ** code_b);

#endif
