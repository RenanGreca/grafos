#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo.h"
#include <cgraph.h>

#define TAM_NOME 64

//------------------------------------------------------------------------------
typedef struct aresta {
    double peso;
    vertice head;
    vertice tail;
} *aresta;

//------------------------------------------------------------------------------
typedef struct vertice {
    int grau;    
    char nome[TAM_NOME];
    aresta *arestas;
} *vertice;

//------------------------------------------------------------------------------
typedef struct grafo {
    vertice *vertices;
    aresta *arestas;
    int direcionado;
    int  num_vertices;
    int  num_arestas;
    int arestas_tem_peso;
    char nome[TAM_NOME];
} *grafo;

//------------------------------------------------------------------------------
const long int infinito = LONG_MAX;

extern char * strdup(const char*);

// Prototypes
//------------------------------------------------------------------------------

grafo init_grafo(void);
vertice *init_vertices(int num_vertices);
aresta *init_arestas(int num_arestas);
int busca_vertice(vertice *vertices, int num_vertices, char * nome);
int busca_aresta(aresta *arestas, int num_arestas, char *nome_head, char *nome_tail);
int copia_arestas(Agedge_t *a, grafo g, int i, int *num_arestas_visitadas); 
void escreve_vertices(vertice *vertices, int n_vertices, FILE *output); 
void escreve_arestas(aresta *arestas, int n_arestas, int direcionado, 
        int tem_peso, FILE *output);
int ja_visitou(vertice *vertices_percorridos, int num_vertices_percorridos, vertice v);
void percorre_vertices(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos);
void percorre_vertices_direcionado(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos);

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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
        v[i]->nome[0] = '\0';
        v[i]->arestas = NULL;
        v[i]->grau = 0;
    }

    return v;
}

//------------------------------------------------------------------------------
aresta *init_arestas(int num_arestas) {
   ; 
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

//------------------------------------------------------------------------------
int busca_vertice(vertice *vertices, int num_vertices, char * nome) {

    for (int i=0; i<num_vertices; i++) {

        if (!strcmp(nome, vertices[i]->nome)) {

            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------------------------
int busca_aresta(aresta *arestas, int num_arestas, char *nome_head, 
	char *nome_tail) {
  
    for (int i=0; i<num_arestas; i++) {
        
		if (    arestas[i]->head == NULL ||
                arestas[i]->tail == NULL ) {
   
            return -1;
		}

        if (    !strcmp( nome_head, arestas[i]->head->nome) &&
                !strcmp( nome_tail, arestas[i]->tail->nome) ) {
   
            return i;  
        }

    }
	
    return -1;
}

//------------------------------------------------------------------------------
int copia_arestas(Agedge_t *a, grafo g, int num_vertices_visitados, int *num_arestas_visitadas) {
    
    char *buffer; 
    char *peso = strdup("peso"); 
    int v_pos;
    int a_pos;


    if ( ( a_pos = busca_aresta(g->arestas, *num_arestas_visitadas, agnameof(aghead(a)), 
                agnameof(agtail(a)) ) ) < 0 ) {

        if ( !strcmp(g->vertices[num_vertices_visitados]->nome, agnameof(aghead(a))) ) {
            
            g->arestas[*num_arestas_visitadas]->head = g->vertices[num_vertices_visitados];

            v_pos = busca_vertice(g->vertices, g->num_vertices, agnameof(agtail(a)));
            
            if (v_pos < 0 ) {
                
                return -1;
            }
            
            g->arestas[*num_arestas_visitadas]->tail = g->vertices[v_pos];
        
        } else {
         
            g->arestas[*num_arestas_visitadas]->tail = g->vertices[num_vertices_visitados];

            v_pos = busca_vertice(g->vertices, g->num_vertices, agnameof(aghead(a)));
          
            if (v_pos < 0 ) {

                return -1;
            }

            g->arestas[*num_arestas_visitadas]->head = g->vertices[v_pos];
        }
       
      
        if ( (buffer = agget(a,peso) ) ) { 
          
            g->arestas_tem_peso = 1; 
            g->arestas[*num_arestas_visitadas]->peso = atof(buffer);
        }

        free(peso);
       
        (*num_arestas_visitadas)++;
        return (*num_arestas_visitadas) - 1;
    }

    return a_pos;
}

//------------------------------------------------------------------------------
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

    // Copia Vértices
    int num_vertices_visitados = 0;
    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v)) {
       
        g->vertices[num_vertices_visitados]->grau = agdegree(graf,v,1,1); 

        g->vertices[num_vertices_visitados]->arestas = (aresta *) malloc (
            (size_t) g->vertices[num_vertices_visitados]->grau * sizeof(aresta));
        
        strcpy(g->vertices[num_vertices_visitados]->nome, agnameof(v));        
        num_vertices_visitados++;
    }

    // Copia Arestas
    int num_arestas_visitadas;
    int j, a_pos;
    num_vertices_visitados = num_arestas_visitadas = 0;
    
    for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v), ++num_vertices_visitados) {
        
        if (g->direcionado) {
     
            j = 0;  
            for (Agedge_t *a=agfstout(graf,v); a; a=agnxtout(graf,a), ++j) {
                
      //          printf("j %d g %d\n",j, g->vertices[num_vertices_visitados]->grau);
               
                a_pos = copia_arestas(a, g, num_vertices_visitados, &num_arestas_visitadas);
                
                g->vertices[num_vertices_visitados]->arestas[j] = g->arestas[a_pos];

/*                printf("v[%d] %s a[%d] %s %s\n", num_vertices_visitados, g->vertices[num_vertices_visitados]->nome, j,
                        g->vertices[num_vertices_visitados]->arestas[j]->tail->nome,
                        g->vertices[num_vertices_visitados]->arestas[j]->head->nome); 
  */              
                g->vertices[num_vertices_visitados]->arestas[j] = g->arestas[a_pos];
            }

        } else {

            j = 0;
            for (Agedge_t *a=agfstedge(graf,v); a; a=agnxtedge(graf,a,v), ++j) {
                
                a_pos = copia_arestas(a, g, num_vertices_visitados, &num_arestas_visitadas);
                
              //  printf("j %d g %d\n",j, g->vertices[i]->grau);
                g->vertices[num_vertices_visitados]->arestas[j] = g->arestas[a_pos];
              /*  printf("v[%d] %s a[%d] %s %s\n",i, g->vertices[i]->nome, j,
                        g->vertices[i]->arestas[j]->head->nome, 
                        g->vertices[i]->arestas[j]->tail->nome);
                */
            }
        }
    }

    printf("arestas visitadas: %d num_arestas em g: %d\n", num_arestas_visitadas, g->num_arestas);
    agclose(graf);
    agfree(graf,NULL);

    return g;	
}

//------------------------------------------------------------------------------
int destroi_grafo(grafo g) {

   // if ( !g || !g->vertices[0] || ! g->vertices || !g->arestas[0] || !g->arestas)
   //    return 0; 

    printf("%d",g->num_vertices);
    for (int i=0 ; i < g->num_vertices ; ++i) {
    
      //  if (!g->vertices[i]->arestas)
      //    return 0;
      //    printf("i %d\n",i); 
      //    printf("v %p\n", g->vertices[i]);
      //    printf("a %p\n", g->vertices[i]->arestas); 
      //    free(g->vertices[i]->arestas); 
    }
/*
    free(g->vertices[0]);
    free(g->vertices);
    free(g->arestas[0]);
    free(g->arestas);
    free(g);
*/
    return 1;
}

//------------------------------------------------------------------------------
void escreve_vertices(vertice *vertices, int n_vertices, FILE *output) {
    
    if (!n_vertices) {

        return;
    }

    for (int i=0; i<n_vertices; i++) {

        fprintf(output, "    \"%s\"\n", vertices[i]->nome);
        /*for( int j=0; j< vertices[i]->grau ; ++j) {
            
            fprintf(output, "    \"%s\" \"%s\"\n", 
                    vertices[i]->arestas[j]->head->nome,
                    vertices[i]->arestas[j]->tail->nome);
        }
        */
    }
}

//------------------------------------------------------------------------------
void escreve_arestas(aresta *arestas, int n_arestas, int direcionado, 
	int tem_peso, FILE *output) {

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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
char *nome(grafo g) {

    return g->nome;
}
//------------------------------------------------------------------------------
unsigned int n_vertices(grafo g) {

    return (unsigned) g->num_vertices;
}

//------------------------------------------------------------------------------
int direcionado(grafo g) {

    return g->direcionado;
}

//------------------------------------------------------------------------------
int ja_visitou(vertice *vertices_percorridos, int num_vertices_percorridos, vertice v) {

    for (int i=0; i<num_vertices_percorridos; ++i) {
   
        if ( vertices_percorridos[i] == v )
            return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------
void percorre_vertices(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos) {
    
    if (ja_visitou(vertices_percorridos, *num_vertices_percorridos, v))
        return;

    //printf("nome %s %p grau %d\n", v->nome, v, v->grau);
    vertices_percorridos[*num_vertices_percorridos] = v;
    (*num_vertices_percorridos)++; 

    for (int i=0 ; i < v->grau ; ++i) {

    //  printf("a[%d] head %s %p tail %s %p\n", i, v->arestas[i]->head->nome, 
    //  v->arestas[i]->head, v->arestas[i]->tail->nome, v->arestas[i]->tail);

        if ( v == v->arestas[i]->head ) {
       
            percorre_vertices(v->arestas[i]->tail, vertices_percorridos, num_vertices_percorridos);
        }
       
        else {

            percorre_vertices(v->arestas[i]->head, vertices_percorridos, num_vertices_percorridos);
        }
    }
}

//------------------------------------------------------------------------------
void percorre_vertices_direcionado(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos) {
    
    if (ja_visitou(vertices_percorridos, *num_vertices_percorridos, v))
        return;

    printf("nome %s %p grau %d\n", v->nome, v, v->grau);
    vertices_percorridos[*num_vertices_percorridos] = v;
    (*num_vertices_percorridos)++; 

    for (int i=0 ; i < v->grau ; ++i) {

        printf("a[%d] head %s %p tail %s %p\n", i, v->arestas[i]->head->nome, 
        v->arestas[i]->head, v->arestas[i]->tail->nome, v->arestas[i]->tail);

        percorre_vertices_direcionado(v->arestas[i]->head, vertices_percorridos, num_vertices_percorridos);
    }
}

//------------------------------------------------------------------------------
int conexo(grafo g) {

    vertice *vertices_percorridos;
    int num_vertices_percorridos = 0;

    vertices_percorridos = malloc ( n_vertices(g) * sizeof(vertice) );
    
    percorre_vertices(g->vertices[0], vertices_percorridos, &num_vertices_percorridos);
   
    free(vertices_percorridos);

    return ( num_vertices_percorridos == (int) n_vertices(g)) ? 1 : 0 ;       
}

//------------------------------------------------------------------------------
int fortemente_conexo(grafo g)  {
    
    /*
    putchar('\n');
    escreve_grafo(stdout,g);
    putchar('\n');
    */

    vertice *vertices_percorridos;
    int num_vertices_percorridos = 0;

    vertices_percorridos = malloc ( n_vertices(g) * sizeof(vertice) );
    
    percorre_vertices_direcionado(g->vertices[0], vertices_percorridos, &num_vertices_percorridos);
   
    free(vertices_percorridos);

    return ( num_vertices_percorridos == (int) n_vertices(g)) ? 1 : 0 ;       

}
//------------------------------------------------------------------------------
long int diametro(grafo g) {

    return g ? 0 : infinito;
}
//------------------------------------------------------------------------------

grafo distancias(grafo g) {

  return g;
}

