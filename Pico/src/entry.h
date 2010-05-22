/** @file entry.h
 *  @version 1.0
 */

#ifndef _ENTRY_H_
#define _ENTRY_H_

#define TINTEGER 'I'
#define TFLOAT   'F'
#define TDOUBLE  'D'
#define TCHAR    'C'

/**
 * Tipo abstrato das entradas na tabela de Hash. (Obs.: futuramente, os campos
 * dessa struct poderao vir a ser alterados em funcao das necessidades.)
 * Na Etapa 2, nao e necessario entender o conteudo desses campos.
 * Sempre vao ser inseridos na tabela, e recuperado dela, ponteiros sobre tais
 * estruturas de dados abstratas.
 */
typedef struct entry_t {
   char* name;  /**< um string que representa o nome de uma variavel. */
   int type;    /**< representacao do tipo da variavel. */
   int size;    /**< numero de Bytes necessarios para armazenamento. */
   int desloc;  /**< Endereco da proxima variavel. */
   void* extra; /**< qualquer informacao extra. */
} entry_t;

entry_t * create_entry(char * name, int type, int size, int desloc, void * extra );

typedef struct SdimList {
	int dimensoes;
	int tamanho;
	int c_inicial;
	int proxTam;
	int l_inferior;
	int l_superior;
	struct SdimList * list;
} dimList;

dimList * create_dimList(int l_inferior, int l_superior, int bSize, dimList * son);

#endif
