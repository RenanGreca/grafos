#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cgraph.h>
#include "grafo.h"

#define epsilon         0.00000001
#define FLTCMP(f1,f2)   ( fabs(f1-f2) < epsilon ) 

extern char * strdup(const char*);

struct vertice {
    char nome[TAM_NOME];
    aresta *arestas;
};

struct aresta {
    double peso;
    vertice head;
    vertice tail;
};

struct grafo {
    vertice *vertices;
    aresta *arestas;
    int direcionado;
    int num_vertices;
    int num_arestas;
    int arestas_tem_peso;
    char nome[TAM_NOME];
};

int copia_arestas(Agedge_t *a, grafo g, int i, int num_arestas_visitadas);

grafo init_grafo(void) {

    grafo g = (grafo) malloc ((size_t) sizeof(struct grafo));
    
    if (!g)
        return NULL;
    
    g->vertices = NULL;
    g->arestas = NULL;
    g->direcionado = 0;
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->arestas_tem_peso = 0;
    g->nome[0] = '\0';

    return g;
}

vertice *init_vertices(int num_vertices) {
    
    vertice *v = (vertice *) malloc ( (size_t) num_vertices * sizeof(vertice));
   
    if(!v)
        return NULL;

    v[0] = (vertice) malloc ( (size_t) num_vertices * sizeof(struct vertice));

    if (!v[0])
        return NULL;

    for (int i=1; i<num_vertices ; i++) {
        v[i] = v[i-1] + 1;
    }

    for (int i=0; i<num_vertices; i++) {
        v[i]->nome[0]='\0';
        v[i]->arestas = NULL;
    }

    return v;
}


aresta *init_arestas(int num_arestas) {
    
    aresta *a = (aresta *) malloc ( (size_t) num_arestas * sizeof(aresta));

    if (!a)
        return NULL;

    a[0] = (aresta) malloc ( (size_t) num_arestas * sizeof(struct aresta));
    
    if (!a[0])
        return NULL;

    for (int i=1; i<num_arestas ; i++) {
        a[i] = a[i-1] + 1;
    }

    for (int i=0; i<num_arestas; i++) {
        a[i]->peso = 0.0;
        a[i]->tail = NULL;
        a[i]->head = NULL;
    }

    return a;
}


int busca_vertice(vertice *vertices, int num_vertices, char * nome) {

    for (int i=0; i<num_vertices; i++) {

        if (!strcmp(nome, vertices[i]->nome)) {

            return i;
        }
    }

    return -1;
}

// Comparando arestas somente com nome dos vértices. Não acha arestas paralelas
int busca_aresta(aresta *arestas, int num_arestas, char *nome_head, char *nome_tail) {
  
    for (int i=0; i<num_arestas; i++) {
        
		if (arestas[i]->head== NULL) {
   
            return -1;
		}

		if (arestas[i]->tail== NULL) {
   
            return -1;
		}

        if (    !strcmp( nome_head, arestas[i]->head->nome) &&
                !strcmp( nome_tail, arestas[i]->tail->nome) ) {
   
            return i;  
        }

    }
	
    return -1;
}

int copia_arestas(Agedge_t *a, grafo g, int i, int num_arestas_visitadas) {
    
    char *buffer; 
    char *peso = strdup("peso"); 
    int v_pos;

    if ( busca_aresta(g->arestas, num_arestas_visitadas, agnameof(aghead(a)), 
                agnameof(agtail(a)) ) < 0 ) {

        if ( !strcmp(g->vertices[i]->nome, agnameof(aghead(a))) ) {
            
            g->arestas[num_arestas_visitadas]->head = g->vertices[i];

            v_pos = busca_vertice(g->vertices, g->num_vertices, agnameof(agtail(a)));
            
            if (v_pos < 0 ) {
                
                return -1;
            }
            
            g->arestas[num_arestas_visitadas]->tail = g->vertices[v_pos];
        
        } else {
         
            g->arestas[num_arestas_visitadas]->tail = g->vertices[i];

            v_pos = busca_vertice(g->vertices, g->num_vertices, agnameof(aghead(a)));
          
            if (v_pos < 0 ) {

                return -1;
            }

            g->arestas[num_arestas_visitadas]->head = g->vertices[v_pos];
        }
       
        //printf("%p\n",agget(a,strdup("peso")));
      
          
        if ( (buffer = agget(a,peso) ) ) { 
          
            g->arestas_tem_peso = 1; 
            g->arestas[num_arestas_visitadas]->peso = atof(buffer);
        }

        free(peso);
       
        num_arestas_visitadas++;
    }

    return num_arestas_visitadas;
}

grafo le_grafo(FILE *input) {

    Agraph_t *graf = agread(input, NULL);
    
    grafo g = NULL;
    g = init_grafo();

    g->direcionado = agisdirected(graf);
    g->num_vertices = agnnodes(graf);    
    g->num_arestas = agnedges(graf);    
    
    g->vertices = init_vertices(g->num_vertices);
    g->arestas = init_arestas(g->num_arestas);

    strcpy(g->nome, agnameof(graf));

    // imprime_vertices(g->vertices,g->num_vertices);
  
    // Copia vértices    
    int i = 0;
    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v)) {
        g->vertices[i]->arestas = (aresta *) malloc (
                (size_t)agdegree(graf,v,1,1)*sizeof(aresta));
        
        strcpy(g->vertices[i]->nome, agnameof(v));        
        i++;
    }

    // Copia Arestas
    int num_arestas_visitadas;
    i = num_arestas_visitadas = 0;

    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v), ++i) {
        
        if (g->direcionado) {
       
            for (Agedge_t *a=agfstout(graf,v); a; a=agnxtout(graf,a)) {
                
                num_arestas_visitadas = copia_arestas(a, g, i, num_arestas_visitadas);

                if (num_arestas_visitadas < 0)
                    return NULL;
            }

        } else {

            for (Agedge_t *a=agfstedge(graf,v); a; a=agnxtedge(graf,a,v)) {
                
                num_arestas_visitadas = copia_arestas(a, g, i, num_arestas_visitadas);
                
                if (num_arestas_visitadas < 0)
                    return NULL;
            }
        }
    }

    agclose(graf);
    agfree(graf,NULL);

	return g;	
}

int destroi_grafo(grafo g){
   
    if ( !g || !g->vertices[0] || ! g->vertices || !g->arestas[0] || !g->arestas)
       return 0; 

    for (int i=0 ; i<g->num_vertices ; ++i) {
    
        if (!g->vertices[i]->arestas)
            return 0;
    
        free(g->vertices[i]->arestas); 
    }

    free(g->vertices[0]);
    free(g->vertices);
    free(g->arestas[0]);
    free(g->arestas);
    free(g);

    return 1;
}

void escreve_vertices(vertice *vertices, int n_vertices, FILE *output) {
    
    if (!n_vertices) {

        return;
    }

    for (int i=0; i<n_vertices; i++) {

        fprintf(output, "    \"%s\"\n", vertices[i]->nome);
    }
}

void escreve_arestas(aresta *arestas, int n_arestas, int direcionado, int tem_peso, FILE *output) {

    char rep_aresta = direcionado ? '>' : '-';
    double peso;

    for (int i=0; i<n_arestas; i++) {
        
        fprintf(output, "    \"%s\" -%c \"%s\"",
                arestas[i]->tail->nome,
                rep_aresta,
                arestas[i]->head->nome
        );

        if ( tem_peso ) {
            peso = arestas[i]->peso;
            fprintf(output, " [peso=%.0lf]", peso);
        }

        fputc('\n', output);
    }
}

grafo escreve_grafo(FILE *output, grafo g) {

	if (!g || !output) {
        return NULL;
    }

    fprintf(output, "strict %sgraph \"%s\" {\n\n",
      g->direcionado ? "di" : "",
      g->nome
    );

    escreve_vertices(g->vertices, g->num_vertices, output);
    fputc('\n', output);
    
    escreve_arestas(g->arestas, g->num_arestas, g->direcionado, g->arestas_tem_peso, output);
    fprintf(output, "}\n");

	return g; 
}
