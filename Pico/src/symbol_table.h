/** @file symbol_table.h
 *  @version 1.1
 */
#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stdio.h>
#include "entry.h"
#include "avl.h"

#ifdef __BFK_DO_TESTING__
	#define TABLE_SIZE 3 /**< tamanho da tabela hash para testes (valores podem ser pequenos para testas estouro)*/
#else
	#define TABLE_SIZE 1000000 /**< tamanho da tabela hash */
#endif

/** \brief Encapsulacao de um tipo abstrato que se chamara 'symbol_t'
 *
 * Voce deve inserir, entre o 'typedef' e o 'symbol_t', a estrutura de dados
 * abstrata que voce ira implementar.
 *
 */

typedef struct __s_symbol_t {
	hash_entry_t *entry[TABLE_SIZE]; /**< implementação da symbol_table */
	struct __s_symbol_t * father;
} s_symbol_t,
  /** a pointer to a symbol_table */
  *symbol_t;


/** \brief Inicializar a tabela de Hash.
 *
 * @param table uma referencia sobre uma tabela de simbolos.
 * @param father uma referencia para a tabela de simbolos pai.
 * @return o valor 0 se deu certo.
 */
int init_table(symbol_t* table, symbol_t father) ;

/** \brief Destruir a tabela de Hash.
 *
 * 'free_table' eh o destrutor da estrutura de dados. Deve ser chamado pelo 
 * usuario no fim de seu uso de uma tabela de simbolos.
 * @param table uma referencia sobre uma tabela de simbolos.
 */
void free_table(symbol_t* table) ;

/** \brief Retornar um ponteiro sobre a entrada associada a 'name'.
 *
 * Essa funcao deve consultar a tabela de simbolos para verificar se se encontra
 * nela uma entrada associada a um char* (string) fornecido em entrada. Para
 * a implementacao, sera necessario usar uma funcao que mapeia um char* a
 * um numero inteiro. Aconselha-se, por exemplo, consultar o livro do dragao
 * (Aho/Sethi/Ulman), Fig. 7.35 e a funcao HPJW.
 *
 * @param table uma tabela de simbolos.
 * @param name um char* (string).
 * @return um ponteiro sobre a entrada associada a 'name', ou NULL se 'name'
 *         nao se encontrou na tabela.
 */
entry_t* lookup(symbol_t table, char* name) ;

/** \brief Inserir uma entrada em uma tabela.
 *
 * @param table uma tabela de simbolos.
 * @param entry uma entrada.
 * @return um numero negativo se nao se conseguiu efetuar a insercao, zero se
 *   deu certo.
 */
int insert(symbol_t* table, entry_t* entry) ;

/** \brief Imprimir o conteudo de uma tabela.
 *
 * A formatacao exata e deixada a carga do programador. Deve-se listar todas
 * as entradas contidas na tabela atraves de seu nome (char*). Deve retornar
 * o numero de entradas na tabela.
 *
 * @param table uma tabela de simbolos.
 * @return o numero de entradas na tabela.
 */
int print_table(symbol_t table);

/** \brief Imprimir o conteudo de uma tabela em um arquivo.
 *
 * A formatacao exata e deixada a carga do programador. Deve-se listar todas
 * as entradas contidas na tabela atraves de seu nome (char*). Deve retornar
 * o numero de entradas na tabela. A saida deve ser dirigida para um arquivo,
 * cujo descritor e passado em parametro.
 *
 * @param out um descrito de arquivo (FILE*).
 * @param table uma tabela de simbolos.
 * @return o numero de entradas na tabela.
 */
int print_file_table(FILE* out, symbol_t table);

/** \brief funcao de hash
 *
 * djb2 - do site http://www.cse.yorku.ca/~oz/hash.html
 * @param str string para se calcular o valor hash
 * @return valor hash da string
 */
unsigned long hash(char *str);

#endif
