#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void print_tree(Node *n, int depth)
	{
		int i;
		for(i = 0; i < depth; i++)
			printf(" ");
		//printf("nl %i id %i l %s t %i a %p nc %i\n",	n->num_line, n->id, n->lexeme, n->type, n->attribute, n->num_children);
		printf("%s | %i | %i\n",n->lexeme, n->type, n->num_children);
		for(i = 0; i < n->num_children; i++)
			print_tree(n->child[i], depth + 1);
	}

/* Variavel que indica qual o proximo id valido */
static int __nodes_ids__ = 0;

Node* create_node(int nl, Node_type t, char* lex, 
                  void* att, int nbc, Node** children) {
	int i;
	assert(t >= program_node && t <= lexpr_node && nbc >= 0 && nbc <= MAX_CHILDREN_NUMBER);
  Node* node = malloc(sizeof(Node));
	assert(node);
	node->num_line = nl;
	node->type = t;
	node->lexeme = strdup(lex);
	node->attribute = att;
	node->num_children = nbc;
	for(i = 0; i < nbc; i++)
		node->child[i] = children[i];
	node->id = __nodes_ids__++;
	return node;
}

Node* Create_node(int nl, Node_type t, char* lexema, 
                  void* att, int nbc, ...) {
	int i;
	va_list nodes;
	assert(t >= program_node && t <= lexpr_node && nbc >= 0 && nbc <= MAX_CHILDREN_NUMBER);
  Node* node = malloc(sizeof(Node));
	assert(node);
	node->num_line = nl;
	node->type = t;
	node->lexeme = strdup(lexema);
	node->attribute = att;
	node->num_children = nbc;
	va_start(nodes,nbc);
	for(i = 0; i < nbc; i++)
		node->child[i] = va_arg(nodes, Node* );
	va_end(nodes);	
	node->id = __nodes_ids__++;
	return node;
}

Node* create_leaf(int nl, Node_type t, char* lex, void* att) {
  return create_node(nl, t, lex, att, 0, NULL);
}

int nb_of_children(Node* n) {
	assert(n);
	return n->num_children;
}


Node* child(Node* n, int i) {
  assert(n && i > 0 && i < n->num_children);
	return n->child[i];
}

int deep_free_node(Node* n) {
	int i;
	if(n) {
#ifdef __BFK_DO_TESTING__ // if we are testing
		printf("deleting...\n");
		print_tree(n, 0);
#endif
		for(i = 0; i < n->num_children; i++)
			deep_free_node(n->child[i]);
		free(n);
	}
	return 0;
}

int height(Node *n) {
	int max = 0, i, temp;
	assert(n);
	for(i = 0; i < n->num_children; i++) {
		temp = height(n->child[i]);
		if(temp > max)
			max = temp;
	}
	return 1 + max;
}

int pack_nodes(Node*** array_of_nodes, const int cur_size, Node* n) {
  assert(cur_size >= 0 && cur_size < MAX_CHILDREN_NUMBER && n);
	if(!cur_size) {
		*array_of_nodes = (Node **) malloc(sizeof(Node *) * MAX_CHILDREN_NUMBER);
		assert(*array_of_nodes);
	}
	array_of_nodes[0][cur_size] = n;
	return cur_size + 1;
}

