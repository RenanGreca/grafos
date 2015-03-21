#include <stdlib.h>
#include <stdio.h>
#include <cgraph.h>
#include "grafo.h"

//------------------------------------------------------------------------------
// estrutura de dados para representar um grafo simples
// 
// o grafo pode ser
// - direcionado ou não
// - com pesos nas arestas ou não
// 
// além dos vértices e arestas, o grafo tem um nome
// 
// os vértices do grafo também tem nome
// 
// os nomes do grafo e dos vértices são strings quaisquer
// 
// num grafo com pesos nas arestas todas as arestas tem peso, que é um double
// 
// o peso default de uma aresta é 0.0

struct vertice {
    char nome[TAM_NOME];
    aresta *arestas;
};

// Armazena espaço para peso da aresta mesmo que não tenha.
// Possível melhorar. Talvez com Unions
struct aresta {
    double peso;
    vertice head;
    vertice tail;
};

// Warning fix padding
struct grafo {
    vertice *vertices;
    aresta *arestas;
    int direcionado;
    int num_vertices;
    int num_arestas;
    char nome[TAM_NOME];
};

//------------------------------------------------------------------------------
// lê um grafo no formato dot de input, usando as rotinas de libcgraph
// 
// desconsidera todos os atributos do grafo lido
// exceto o atributo "peso" nas arestas onde ocorra
// 
// num grafo com pesos nas arestas todas as arestas tem peso, que é um double
// 
// o peso default de uma aresta num grafo com pesos é 0.0
// 
// devolve o grafo lido ou
//         NULL em caso de erro 

grafo init_grafo(void) {
 	   
   	printf("Iniciando Grafo\n");	 
    grafo g = (grafo) malloc (sizeof(struct grafo));
    
    g->vertices = NULL; 
    g->arestas = NULL; 
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->nome[0] = '\0';

   	printf("Iniciou Grafo\n");	 
    return g;   
}

vertice *init_vertices(int num_vertices) {

   	printf("Iniciando Vertices\n");	 
    vertice *v = (vertice *) malloc ( (size_t) num_vertices * sizeof(vertice));
   	printf("1\n");	 
	v[0] = (vertice) malloc ( (size_t) num_vertices * sizeof(struct vertice));     
   	printf("2\n");	 
	for (int i=0; i<num_vertices; i++) {
		v[i] = v[i-1] +	num_vertices;
	}
   	printf("3\n");	 
	
    for (int i=0; i<num_vertices; i++) {
        v[i]->nome[0] = '\0';
        v[i]->arestas = NULL;
    }

   	printf("Iniciou Vertices\n");	 
    return v;
}

aresta *init_arestas(int num_arestas) {
    
   	printf("Iniciando Arestas\n");	 
    aresta *a = (aresta *) malloc ( (size_t) num_arestas * sizeof(aresta));
    a[0] = (aresta) malloc ( (size_t) num_arestas * sizeof(struct aresta));
	for (int i=0; i<num_arestas; i++) {
		a[i] = a[i-1] +	num_arestas;
	}
    
    for (int i=0; i<num_arestas; ++i) {
        a[i]->peso = 0.0;
        a[i]->head = NULL;
        a[i]->tail = NULL;
    }

   	printf("Iniciou Arestas\n");	 
    return a;
}
/*
aresta adiciona_aresta(aresta arestas, int num_arestas, 
        double peso, vertice head, vertice head) {
    
    aresta ptr_a = arestas+num_arestas;
    ptr_a.peso = peso;
    ptr_a.head = head;
    ptr_a.tail = tail;

    return ptr_a;
}
*/

aresta busca_aresta(aresta *arestas, int num_arestas, aresta a) {
   printf("Buscando\n");	 
    for (int i=0; i<num_arestas; i++) {
        
        // Assume que entre dois vértices só pode haver uma aresta
        // Precisa checar head com tail e vice-versa?
        if ( (a->head  == arestas[i]->head) &&
                (a->tail == arestas[i]->tail) &&
                //Warning. Comparando floats
                (a->peso == arestas[i]->peso) ) {
       
   			printf("Achou\n");	 
            return arestas[i];  
        }
    }
    
   	printf("Achou\n");	 
    return NULL;    
}

grafo le_grafo(FILE *input) {
 
	printf("Lendo\n");
    Agraph_t *graf = agread(input, NULL);
    
    grafo g = init_grafo();

    g->direcionado = agisdirected(graf);
    g->num_vertices = agnnodes(graf);    
    g->num_arestas = agnedges(graf);    

    g->vertices = init_vertices(g->num_vertices);
    g->arestas = init_arestas(g->num_arestas);
/*
    // Copia de Agraph_t
    aresta *ptr_av = NULL;
    aresta *ptr_ag = g->arestas;
    aresta *ptr_agn = NULL;
    vertice *ptr_v = g->vertices;
	aresta tmp_aresta = (aresta) malloc (sizeof(aresta));
    int i;
    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v)) {
        g.vertices[i]->nome = agnameof(v);        
       
        ptr_av = ptr_v.arestas;   
        i=1;
        for (Agedge_t *a=agfstedge(graf,v); a; a=agnxtedge(graf,a,v)) {
            ptr_v.arestas = realloc(ptr_v.arestas, i*sizeof(aresta));
			tmp_aresta.peso = agget(a, (char *) "peso");
			tmp_aresta.head = ptr_v; 
			tmp_aresta.tail = NULL; 
            if ( (ptr_agn = busca_aresta(g->arestas, g->num_arestas, tmp_aresta)) ) {
                ptr_ag.peso = agget(a, (char *) "peso");
                ptr_ag.head = ptr_v; 
                ptr_av=ptr_ag;      
                ptr_ag++;
            }               
           
            ptr_agn->tail = ptr_v;       
            ptr_av = ptr_agn;       
            ptr_av++;
            i++;
        }
        
        ptr_v++;
    }
  */  
	
	printf("Leu\n");
	return g;	
   
}
//------------------------------------------------------------------------------
// desaloca toda a memória utilizada em g
// 
// devolve 1 em caso de sucesso ou
//         0 em caso de erro

int destroi_grafo(grafo g) {
	printf("Desalocando\n");
	// Falta desalocar as arestas de cada vértice 
	free(g->vertices[0]);
	free(g->vertices);
	free(g->arestas[0]);
	free(g->arestas);
	free(g);
	printf("Desalocou\n");
	return 0;
}
//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot, de forma que
// 
// 1. todos os vértices são escritos antes de todas as arestas/arcos 
// 2. se uma aresta tem peso, este deve ser escrito como um atributo
//
// devolve o grafo escrito ou
//         NULL em caso de erro 

grafo escreve_grafo(FILE *output, grafo g) {
	printf("Escrevendo\n");
 	fprintf(output, "Fim\n");
	printf("Escreveu\n");
	return g; 
}

