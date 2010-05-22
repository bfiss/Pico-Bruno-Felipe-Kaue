#include "../src/avl.h"
#include <assert.h>

int main()
{

	int i;

	entry_t	entry[12] = {{"d", 0, 1, 2, (void *)3},
								{"e", 0, 1, 2, (void *)3},
								{"a", 0, 1, 2, (void *)3},
								{"c", 0, 1, 2, (void *)3},
								{"f", 0, 1, 2, (void *)3},
								{"ga", 0, 1, 2, (void *)3},
								{"gi", 0, 1, 2, (void *)3},
								{"ge", 0, 1, 2, (void *)3},
								{"gx", 0, 1, 2, (void *)3},
								{"h", 0, 1, 2, (void *)3},
								{"l", 0, 1, 2, (void *)3},
								{"x", 0, 1, 2, (void *)3}};

	pNodoA raiz, aux;
	int find=0, n_nodo=0;

	raiz = inicializaAVL();

	for(i=0;i<12;i++)
	{
		fprintf(stdout, "\nINSERINDO %s\n",entry[i].name);
		raiz = insereAVL(raiz, &entry[i]);
		aux = findNodo(raiz, "d", &find);
		if(find)
			fprintf(stdout, "achou d\n");
		find = 0;
		aux = findNodo(raiz, "f", &find);
		if(find)
			fprintf(stdout, "achou f\n");
		find = 0;
		imprimeAVL(stdout, raiz, &n_nodo);
		fprintf(stdout, "N Nodos = %d\n",n_nodo);
		n_nodo=0;
	}	

	destroiAVL(raiz);

	return 0;
}
