/** @file node.h
 *  @version 1.1
 */

#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h> 
#include <stdarg.h>
#include "lista.h"

typedef int Node_type; /**< tipo do nó */

/** @name Constantes
 *  Serie de constantes que servirao para definir tipos de nos (na arvore),
 * a partir da etapa 4 - irrelevante por enquanto.
 */
/** @{ */ 
#define program_node   299 /* code */
#define idf_node       300 /* IDF */
#define int_node       301 /* INT_LIT */
#define float_node     302 /* F_LIT */
#define str_node       303
#define empty_node     304
#define proc_node      305 /* chamaproc */
#define param_node     306
#define decl_node      307 /* declaracao */
#define decl_list_node 308 /* listadeclaracao */
#define op_node        309
#define nop_node       310
#define return_node    311
#define if_node        312 /* IF '(' expbool ')' THEN acoes fiminstcontrole */
#define while_node     313 /* WHILE '(' expbool ')' '{' acoes '}' */
#define print_node     314 /* PRINTF */
#define cond_node      315 /* expbool */
#define affect_node    316
#define or_node        317 /* expbool OR expbool */
#define and_node       318 /* expbool AND expbool */
#define eq_node        319 /* expr EQ expr */
#define neq_node       320 /* expr NE expr */
#define inf_node       321 /* expr '<' expr */
#define sup_node       322 /* expr '>' expr */
#define inf_eq_node    323 /* expr LE expr */
#define sup_eq_node    324 /* expr GE expr */
#define plus_node      325 /* expr '+' expr */
#define minus_node     326 /* expr '-' expr */
#define mult_node      327 /* expr '*' expr */
#define div_node       328 /* expr '/' expr */
#define mod_node       329
#define umenos_node    330
#define not_node       331 /* NOT expbool */
#define char_node      332
#define bloc_node      333 /* acoes */
#define true_node      335 /* TRUE */
#define false_node     336 /* FALSE */
#define else_node      337 /* fiminstcontrole - NAO USA MAIS */
#define decs_node      338 /* declaracoes */
#define type_node      339 /* tipo */
#define tipou_node     340 /* tipounico */
#define tipol_node     341 /* tipolista */
#define listad_node    342 /* listadupla */
#define com_node       343 /* comando */
#define lvalue_node    344 /* lvalue */
#define enunc_node     345 /* enunciado */
#define expr_node      346 /* expr */
#define lexpr_node     347 /* listaexpr */

/** @} */ 

/** número máximo de filhos de uma struct _node */
#define MAX_CHILDREN_NUMBER 10

struct attributes {
	enum opType type;
	int desloc;
	int labelT;
	int labelF;
};

/** Estrutura de dados parcial para o no da arvore.
 *  Trata-se de uma arvore generalizada: qualquer no pode ter de 0 ateh
 *  MAX_CHILDREN_NUMBER filhos. */
typedef struct _node {
   int num_line;   /**< numero de linha (irrelevante por enquanto).*/
   int id;         /**< rótulo do nó. Cada no deve ter um 'id' distinto. */
   char* lexeme;   /**< irrelevante por enquanto. */
   Node_type type; /**< Um dos valores definidos acima pelos # defines. */
   void* attribute;/**< Qualquer coisa por enquanto. */
   struct attributes att;
   /* Fim das informacoes armazenadas em cada no.
    * A seguir, completar essa estrutura de dados com o necessário para
    * a implementacao dos metodos especificados.
    */
	int num_children;/**< Numero de filhos do nodo. */
	struct _node *child[MAX_CHILDREN_NUMBER];/**< Vetor de ponteiros para os filhos dos nodos. */
} Node;

/** Constructor of a Node.
 *
 * @param nl  line number of the instruction that originates the node.
 * @param t   node type (one of the values # define'd above). Must return an
 *             error if the type in not correct.
 * @param lexema  whatever string you want to associate to the node.
 * @param att  a semantical attribute.
 * @param nbc  number of children nodes (<= MAX_CHILDREN_NUMBER and >= 0). Must
 *      return an error if nbc it not correct.
 * @param children  array of children nodes (of size 'nbc').
 * @return a (pointer on a) new Node.
 */
Node* create_node(int nl, Node_type t, char* lexema, 
                  void* att, int nbc, Node** children) ;
				  
/** Constructor of a Node.
 *
 * @param nl  line number of the instruction that originates the node.
 * @param t   node type (one of the values # define'd above). Must return an
 *             error if the type in not correct.
 * @param lexema  whatever string you want to associate to the node.
 * @param att  a semantical attribute.
 * @param nbc  number of children nodes (<= MAX_CHILDREN_NUMBER and >= 0). Must
 *      return an error if nbc it not correct.
 * @param ...  array of children nodes (of size 'nbc').
 * @return a (pointer on a) new Node.
 */
Node* Create_node(int nl, Node_type t, char* lexema, 
                  void* att, int nbc, ...) ;

/** Constructor of a leaf Node (without any child).
 * @param nl  line number of the instruction that originates the node.
 * @param t   node type (one of the values # define'd above).  Must return an
 *             error if the type in not correct.
 * @param lexema  whatever string you want to associate to the node.
 * @param att  a semantica attribute (can be NULL for now).
 * @return a (pointer) on a new Node.
 */
Node* create_leaf(int nl, Node_type t, char* lexema, void* att) ;

/** accessor to the number of children of a Node.
 * @param n  node
 * @return the number of children
 * @warning n shouldn't be NULL
 */
int nb_of_children(Node* n);

/** accessor to the i'th child of a Node.
 * @param n  the node to be consulted. Must return en error if 'n' is NULL.
 * @param i  the number of the child that one wants. Must be strictly lower 
 *       than n->num_children and larger than 0. Must return an error if i is
 *       not correct.
 * @return a pointer on a Node.
 */
Node* child(Node* n, int i) ;

/** Pushes 'n' Nodes on bottom of an array of Node* which contains originally
 * 'cur_size' entries. Returns the new number of entries (ie 'cur_size+1') in
 * the array 'array_of_nodes'.
 * This function is convenient to be used with create_node (see its last
 * argument).
 * On the first call, 'array_of_nodes' should not be allocated, and 'cur_size'
 * should be zero.
 * Implementation limit: since these Node* structures are meant to be used
 * in a compiler, one expects an upper limit of MAX_CHILDREN_NUMBER nodes
 * to be packed. Trying to pack more than this limit must raise an error.
 * Typical use: Node** children; pf1 = create_leaf(1, int_node, "1", NULL);
 * pf3 = create_leaf(1, int_node, "2", NULL); pack_nodes(&children, 0, pf1);
 *  pack_nodes(&children, 1, pf3);
 *  @param array_of_nodes  the array where the node will be pushed
 *  @param cur_size  the current size of the array
 *  @param n  the node to be pushed
 *  @return the new number of entries (cursize + 1)
 *  @warning cur_size should be less than MAX_CHILDREN_NUMBER 
 */
int pack_nodes(Node*** array_of_nodes, int cur_size, Node* n);

/** Destructor of a Node. Desallocates (recursively) all the tree rooted at
 * 'n'.
 * @param n  the root node
 */
int deep_free_node(Node* n) ;

/** returns the height of the tree rooted by 'n'.
 *  The height of a leaf is 1. 
 * @param n  the root node
 * @return the height of the tree
 */
int height(Node *n) ;

/** pretty-prints a tree
 * @param n  the root node
 * @param depth  the current depth in the tree
 */
void print_tree(Node *n, int depth);

#endif
