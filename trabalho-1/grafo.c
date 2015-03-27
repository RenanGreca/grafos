#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cgraph.h>
#include "grafo.h"

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
        v[i]->nome[0]='\0';
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

aresta busca_aresta(aresta *arestas, int num_arestas, double peso, 
        char *nome_head, char *nome_tail) {
  
    for (int i=0; i<num_arestas; i++) {
        
        // Assume que entre dois vértices só pode haver uma aresta
        // Precisa checar head com tail e vice-versa?
       
        printf("\ta %d peso %f head %s tail%s\n", i, arestas[i]->peso, 
                arestas[i]->head->nome , arestas[i]->tail->nome);

        if ( (( (strcmp( nome_head, arestas[i]->head->nome) &&
                strcmp( nome_tail, arestas[i]->tail->nome) ) ||
                (strcmp( nome_head, arestas[i]->tail->nome) &&
                strcmp( nome_tail, arestas[i]->tail->nome)) ) &&
                //Warning. Comparando floats
                (peso == arestas[i]->peso)) ) {
       
            return arestas[i];  
        }
    }
    
    return NULL;    
}

void imprime_vertices (vertice *v, int num_vertices) {
    
    for (int i=0; i<num_vertices; i++) {
        printf("\"%s\"\n", v[i]->nome);
    }
}


grafo le_grafo(FILE *input) {

	printf("Lendo\n");
    Agraph_t *graf = agread(input, NULL);
    
    grafo g = NULL;
    g = init_grafo();

    g->direcionado = agisdirected(graf);
    g->num_vertices = agnnodes(graf);    
    
    g->vertices = init_vertices(g->num_vertices);
    g->arestas = init_arestas(agnedges(graf));

    strcpy(g->nome, "meu grafo");

    // imprime_vertices(g->vertices,g->num_vertices);
        
    // Copia de Agraph_t
    
    int i,j,k;
    i = j = k = 0;

    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v)) {
        strcpy(g->vertices[i]->nome, agnameof(v));   
        i++;     
    }
    
    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v)) {
        
        g->vertices[i]->arestas = (aresta *) malloc (
                (size_t)agdegree(graf,v,1,1)*sizeof(aresta));
        
        strcpy(g->vertices[i]->nome, agnameof(v));        
        printf("\"%s\"\n", g->vertices[i]->nome);        
        
        j=0;
        aresta tmp_a = NULL;
        for (Agedge_t *a=agfstedge(graf,v); a; a=agnxtedge(graf,a,v)) {
		    
            //printf("%f\n",g->vertices[i]->arestas[j]->peso);
            printf("vertice %d agedge %d peso %f head %s tail %s\n",i,j,
                    atof(agget(a,"peso")), agnameof(aghead(a)), agnameof(agtail(a)) );

            if ( ( tmp_a = busca_aresta(g->arestas, g->num_arestas, 
                    atof( agget(a, "peso") ), 
                    agnameof(aghead(a)), agnameof(agtail(a)) )))
            {
                printf("Achou\n");
                if ( strcmp(g->vertices[i]->nome, agnameof(aghead(a))) ) 
                    tmp_a->head = g->vertices[i];
                else
                    tmp_a->tail = g->vertices[i];

                g->vertices[i]->arestas[j] = tmp_a;              
            }
            else {
                printf("Nao Achou\n");
                g->arestas[k]->peso = atof(agget(a,"peso"));
              
                printf("%s\n",g->vertices[i]->nome);  
                if ( strcmp(g->vertices[i]->nome, agnameof(aghead(a))) ) 
                    g->arestas[k]->head = g->vertices[i];
                else    
                    g->arestas[k]->tail = g->vertices[i];
               
                g->vertices[i]->arestas[j] = g->arestas[k];              
                k++;
                g->num_arestas = k;
            }

            j++;
        }
        
        i++;
    }

    free (graf);    
	printf("Leu\n");
	return g;	
}
//------------------------------------------------------------------------------
// desaloca toda a memória utilizada em g
// 
// devolve 1 em caso de sucesso ou
//         0 em caso de erro

int destroi_grafo(grafo g){
    free(g->vertices[0]);
    free(g->vertices);
    free(g->arestas[0]);
    free(g->arestas);
    free(g);
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

void escreve_vertices(vertice *vertices, int n_vertices) {
    
    if (!n_vertices) {
        return;
    }

    for (int i=0; i<n_vertices; i++) {
        printf("    \"%s\"\n", vertices[i]->nome);
    }
}

void escreve_arestas(aresta *arestas, int n_arestas, int direcionado) {
    char rep_aresta = direcionado ? '>' : '--';

    for (int i=0; i<n_arestas; i++) {
        double peso = arestas[i]->peso;
        
        printf("    \"%s\" -%c \"%s\"",
                arestas[i]->head->nome,
                rep_aresta,
                arestas[i]->tail->nome
        );

        if ( peso )
          printf(" [peso=%.2lf]", peso);

        printf("\n");
    }
}

grafo escreve_grafo(FILE *output, grafo g) {
	if (!g) {
        return NULL;
    }
    printf("Escrevendo\n");
    //printf("Num vertices: %d\n", g->num_vertices);
    //printf("Num arestas: %d\n", g->num_arestas);

    /*direcionado = g->direcionado;
    n_vertices = g->num_vertices;
    n_arestas = g->num_arestas;

    arestas = g->arestas;*/

    printf("strict %sgraph \"%s\" {\n\n",
      g->direcionado ? "di" : "",
      g->nome
    );

    escreve_vertices(g->vertices, g->num_vertices);
    printf("\n");
    escreve_arestas(g->arestas, g->num_arestas, g->direcionado);
    printf("}\n");

 	fprintf(output, "Fim\n");
	printf("Escreveu\n");
	return g; 
}