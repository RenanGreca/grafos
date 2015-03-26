#include <stdio.h>
#include <stdlib.h>

#define TAM_NOME 56

typedef struct vertice *vertice;
typedef struct aresta  *aresta;
typedef struct grafo  *grafo;

struct aresta {
    double peso;
    vertice head;
    vertice tail;
};

struct vertice {
    char nome[TAM_NOME];
    aresta *arestas;
};

struct grafo {
    vertice *vertices;
    aresta *arestas;
    int direcionado;
    int num_vertices;
    int num_arestas;
    char nome[TAM_NOME];
};

grafo init_grafo(void);
vertice *init_vertices(int num_vertices);
aresta *init_arestas(int num_arestas);
void imprime_vertices(vertice *v, int num_vertices);
void destroi_grafo(grafo g);

grafo init_grafo(void) {

    grafo g = (grafo) malloc ((size_t) sizeof(struct grafo));
    g->vertices = NULL;
    g->arestas = NULL;
    g->direcionado = 0;
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->nome[0] = '\0';
    return g;
}

vertice *init_vertices(int num_vertices) {
    
    vertice *v = (vertice *) malloc ( (size_t) num_vertices * sizeof(vertice));
    v[0] = (vertice) malloc ( (size_t) num_vertices * sizeof(struct vertice));

    for (int i=1; i<num_vertices ; i++)
        v[i] = v[i-1] + 1;

    for (int i=0; i<num_vertices; i++) {
        v[i]->nome[0]='c';
        v[i]->nome[1]='\0';
        v[i]->arestas = NULL;
    }

    return v;
}

aresta *init_arestas(int num_arestas) {
    
    aresta *a = (aresta *) malloc ( (size_t) num_arestas * sizeof(aresta));
    a[0] = (aresta) malloc ( (size_t) num_arestas * sizeof(struct aresta));

    for (int i=1; i<num_arestas ; i++)
        a[i] = a[i-1] + 1;

    for (int i=0; i<num_arestas; i++) {
        a[i]->peso = 0.0;
        a[i]->tail = NULL;
        a[i]->head = NULL;
    }

    return a;
}

void imprime_vertices (vertice *v, int num_vertices) {
    
    for (int i=0; i<num_vertices; i++) {
        printf("v[%d] = %s\n", i, v[i]->nome);
    }
}

void destroi_grafo(grafo g){
    free(g->vertices[0]);
    free(g->vertices);
    free(g->arestas[0]);
    free(g->arestas);
    free(g);
}


int main(void) {


    grafo g = NULL;
    g = init_grafo();
    g->num_vertices=114;
    g->num_arestas=300;
    
    g->vertices = init_vertices(g->num_vertices);
    g->arestas = init_arestas(g->num_arestas);

    imprime_vertices(g->vertices, g->num_vertices);    

    destroi_grafo(g);

    return 0;
}
