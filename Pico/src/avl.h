/** @file avl.h
 *  @version 1.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "entry.h"

/** Entrada na tabela de hash
 * Trata-se de uma AVL
 */
typedef struct hash_entry_t{
  struct entry_t *entry; /**< conte煤do do n贸 */
  struct hash_entry_t *dir, /**< filho 脿 direita do n贸*/
  					  *esq; /**< filho 脿 esquerda do n贸*/
	int fatorB; /**< fator de balanceamento do n贸*/
}
 /** n贸 da 谩rvore*/
 hash_entry_t,
 /** ponteiro para um n贸*/
 *pNodoA;

/** \brief Inicializar a arvore AVL.
 *
 * @return ponteiro para a raiz da arvore.
 */
pNodoA inicializaAVL();

/** \brief Destroi a arvore, desalocando memoria ocupada por ela.
 *
 * @param arv raiz da arvore a ser destruida.
 */
void destroiAVL(pNodoA arv);

/** \brief Inserve elemento na arvore.
 *
 * @param arv raiz da arvore.
 * @param symbol entrada a ser inserida.
 * @param success ponteiro utilizado para especificar se a inser玢o foi bem sucedida ou n鉶.
 * @return ponteiro para a raiz da arvore.
 */
pNodoA insereAVL(pNodoA arv, entry_t* symbol, int * success);

/** \brief Encontra um elemento na arvore.
 *
 * @param arv raiz da arvore.
 * @param name nome do elemento a ser pesquisado.
 * @param find endereco de um inteiro que armazena se o elemento foi encontrado (1 encontrado, 0 nao).
 * @return ponteiro para o nodo da arvore que contem o elemento pesquisado.
 */
pNodoA findNodo(pNodoA arv, char* name, int* find);

/** \brief Imprime a arvore em ordem crescente de ordenamento.
 *
 * @param out saida para imprimir a arvore.
 * @param arv raiz da arvore.
 * @param n_nodo endereco de um contador do numero de nodos.
 */
void imprimeAVL(FILE* out, pNodoA arv, int* n_nodo);

/** \brief Imprime a arvore com saida formatada em camadas.
 *
 * @param arv raiz da arvore.
 * @param count valor da camada inicial.
 */
void printNiveis(pNodoA arv, int count);

/** \brief Realizada rotacao a esquerda na arvore.
 *
 * @param arv raiz da arvore.
 */
pNodoA rotEsquerda(pNodoA arv);

/** \brief Realizada rotacao a direita na arvore.
 *
 * @param arv raiz da arvore.
 */
pNodoA rotDireita(pNodoA arv);

/** \brief Realizada rotacao dupla a esquerda na arvore.
 *
 * @param arv raiz da arvore.
 */
pNodoA rotDupEsquerda(pNodoA arv);

/** \brief Realizada rotacao dupla a direita na arvore.
 *
 * @param arv raiz da arvore.
 */
pNodoA rotDupDireita(pNodoA arv);

/** \brief Realizada balanceamento a esquerda na arvore.
 *
 * @param arv raiz da arvore.
 */
pNodoA balanceamentoEsq(pNodoA arv);

/** \brief Realizada balanceamento a direita na arvore.
 *
 * @param arv raiz da arvore.
 */
pNodoA balanceamentoDir(pNodoA arv);

/** \brief Testa de o nodo 茅 vazio.
 *
 * @param arv n贸 a ser testado
 */
int NodoVazio(pNodoA arv);

/** \brief Calcula a altura da arvore.
 *
 * @param arv raiz da 谩rvore
 */
int altura(pNodoA arv);

/** \brief Calcula o fator de balanceamento da arvore.
 *
 * @param arv raiz da 谩rvore
 */
int calculaFatorB(pNodoA arv);
