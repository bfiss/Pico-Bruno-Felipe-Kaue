#define __BFK_DEBUG__
#include "../src/node.h"
#include <assert.h>
#include <stdio.h>

int main()
{

	Node *nodea = NULL, **nodes, *nodeb, *nodeab, *nodec, *nodecab;

	int i;
	char lexema[] = "Testando0";

	Node* node[30];

/*
BATERIA DE TESTES 1
*/

	printf("TESTES 1\n\n");

	for(i=0;i<11;i++)
	{
		lexema[8]=i+48;
		node[i] = create_leaf(i, program_node, lexema, NULL);
		assert(nb_of_children(node[i]) == 0);
		assert(height(node[i]) == 1);
	}

	for(i=0;i<10;i++)	
		pack_nodes(&nodes, i, node[i]);

	node[11] = create_node(10, idf_node, "PAI1a", (void *)2, 10, nodes);
	//print_tree(node[11], 0);
	//printf("\n");
	assert(height(node[11]) == 2);

	for(i=12;i<20;i++)
	{
		lexema[8]=i+48;
		node[i] = create_leaf(i, program_node, lexema, NULL);
		assert(nb_of_children(node[i]) == 0);
		assert(height(node[i]) == 1);
	}

	for(i=12;i<20;i++)	
		pack_nodes(&nodes, i-12, node[i]);

	node[21] = create_node(21, idf_node, "PAI1b", (void *)2, 8, nodes);
	//print_tree(node[21], 0);
	//printf("\n");
	assert(height(node[21]) == 2);

	pack_nodes(&nodes, 0, node[11]);
	pack_nodes(&nodes, 1, node[21]);

	node[22] = create_node(22, idf_node, "PAI2", (void *)2, 2, nodes);	

	//print_tree(node[22], 0);
	//printf("\n");
	assert(height(node[22]) == 3);

	for(i=23;i<30;i++)
	{
		pack_nodes(&nodes, 0, node[i-1]);
		node[i] = create_node(i, idf_node, "PAIUp", (void *)2, 1, nodes);
		assert(height(node[i]) == 4+(i-23));
	}

	print_tree(node[29], 0);
	deep_free_node(node[29]);
	printf("\n");

/*
BATERIA DE TESTES 0
*/

	printf("TESTES 0\n\n");

	//nb_of_children(nodea); // deve retornar um erro
	//child(nodea, 0); // deve retornar um erro
	//pack_nodes(NULL, 0, nodea); // deve retornar um erro
	deep_free_node(nodea);
	//height(nodea); // deve retornar um erro
	
	//create_leaf(1, program_node - 1, NULL, NULL); // deve retornar um erro
	//create_leaf(1, false_node + 1, NULL, NULL); // deve retornar um erro
	nodea = create_leaf(1, program_node, "a", NULL);
	print_tree(nodea, 0);
	printf("\n");
	assert(nb_of_children(nodea) == 0);
	assert(height(nodea) == 1);
	//child(nodea, 0); // deve retornar um erro
	//pack_nodes(NULL, MAX_CHILDREN_NUMBER, nodea); // deve retornar um erro
	//pack_nodes(NULL, -1, nodea); // deve retornar um erro
	pack_nodes(&nodes, 0, nodea);

	nodeb = create_leaf(2, false_node, "b", (void *)1);
	print_tree(nodeb, 0);
	printf("\n");
	
	pack_nodes(&nodes, 1, nodeb);
	//create_node(3, idf_node, "ab", (void *)2, -1, nodes); // deve retornar um erro
	//create_node(3, idf_node, "ab", (void *)2, MAX_CHILDREN_NUMBER + 1, nodes); // deve retornar um erro
	nodeab = create_node(3, idf_node, "ab", (void *)2, 2, nodes);
	print_tree(nodeab, 0);
	printf("\n");
	assert(height(nodeab) == 2);
	//child(nodeab, 0); // deve retornar um erro
	//child(nodeab, nb_of_children(nodeab)); // deve retornar um erro
	assert(child(nodeab, 1) == nodeb);

	nodec = create_leaf(4, true_node, "c", (void *)3);
	pack_nodes(&nodes, 0, nodec);
	pack_nodes(&nodes, 1, nodeab);
	
	nodecab = create_node(5, int_node, "cab", (void *)4, 2, nodes);
	print_tree(nodecab, 0);
	printf("\n");
	assert(height(nodecab) == 3);
	
	deep_free_node(nodecab);

	return 0;
}
