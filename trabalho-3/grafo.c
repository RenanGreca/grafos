#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cgraph.h>
#include <limits.h>
#include "grafo.h"

#define TAM_NOME 56

extern char * strdup(const char*);
const long infinito = LONG_MAX;

struct lista {

	int tam;
	no n;

};

struct no {

	void *conteudo;
	no prox;

};

typedef struct aresta *aresta;

struct vertice {

	char nome[TAM_NOME];	
    aresta *arestas;
    int grau;

};

struct aresta {

    long peso;
    vertice head;
    vertice tail;

};

struct grafo {

    vertice *vertices;
    aresta *arestas;
	int direcionado;
	unsigned int num_vertices;
	unsigned int num_arestas;
	int tem_peso;
	char nome[TAM_NOME];

};

int copia_arestas(Agedge_t *a, grafo g, int i, int *num_arestas_visitadas); 

// LISTA ------------------------------------------------------------------------------
no primeiro_no(lista l) {

	return ( (l->n == NULL) ? NULL : l->n );
}

// NO ------------------------------------------------------------------------------
no proximo_no(no n) {

	return ( (n->prox == NULL) ? NULL : n->prox );
}

void *conteudo(no n){

	return n->conteudo;
}

// VERTICE ------------------------------------------------------------------------------
char *nome_vertice(vertice v){

	return v->nome;
}

// GRAFO ------------------------------------------------------------------------------
char *nome(grafo g) {
	return g->nome;
}

unsigned int n_vertices(grafo g) {
	return g->num_vertices;
}

int direcionado(grafo g) {
	return g->direcionado;
}

/*int conexo(grafo g) {

}*/

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
	g->tem_peso = 0;
	g->nome[0] = '\0';

	return g;
}

//------------------------------------------------------------------------------
vertice *init_vertices(unsigned int num_vertices) {

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

aresta *init_arestas(unsigned int num_arestas) {
    
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

int busca_vertice(vertice *vertices, unsigned int num_vertices, char * nome) {

    for (int i=0; i<num_vertices; i++) {

        if (!strcmp(nome, vertices[i]->nome)) {

            return i;
        }
    }

    return -1;
}

int busca_aresta(aresta *arestas, unsigned int num_arestas, char *nome_head, char *nome_tail) {
  
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
          
            g->tem_peso = 1; 
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

int destroi_grafo(void *g){

	grafo gr = (grafo) g;
   
    if ( !gr || !gr->vertices[0] || ! gr->vertices || !gr->arestas[0] || !gr->arestas)
       return 0; 

    for (unsigned int i=0 ; i<gr->num_vertices ; ++i) {
    
        if (!gr->vertices[i]->arestas)
            return 0;
    
        free(gr->vertices[i]->arestas); 
    }

    free(gr->vertices[0]);
    free(gr->vertices);
    free(gr->arestas[0]);
    free(gr->arestas);
    free(gr);

    return 1;
}

void escreve_vertices(vertice *vertices, unsigned int n_vertices, FILE *output) {
    
    if (!n_vertices) {

        return;
    }

    for (int i=0; i<n_vertices; i++) {

        fprintf(output, "    \"%s\"\n", vertices[i]->nome);
    }
}

void escreve_arestas(aresta *arestas, unsigned int n_arestas, int direcionado, int tem_peso, FILE *output) {

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
    
    escreve_arestas(g->arestas, g->num_arestas, g->direcionado, g->tem_peso, output);
    fprintf(output, "}\n");

	return g; 
}

//------------------------------------------------------------------------------
int destroi_lista(lista l, int destroi(void *)) {

  if ( !l )

    return 0;

  no prox;

  for (no n=primeiro_no(l); n; n=prox) {

    prox = proximo_no(n);

    if (destroi)

      destroi(conteudo(n));

    free(n);
  }

  free(l);
  
  return 1;
}

//------------------------------------------------------------------------------
int ja_visitou(vertice *vertices_percorridos, int num_vertices_percorridos, vertice v) {

    for (int i=0; i<num_vertices_percorridos; ++i) {
   
        if ( vertices_percorridos[i] == v )
            return 1;
    }

    return 0;
}

lista componentes(grafo g) {
    vertice *vertices_percorridos;
    int num_vertices_percorridos = 0;

    vertices_percorridos = malloc ( n_vertices(g) * sizeof(vertice) );

    while (1) { 
        vertice v = in(g, vertices_percorridos, num_vertices_percorridos);
        if (v == NULL) {
            break;
        }

        percorre_vertices(v, vertices_percorridos, &num_vertices_percorridos);
    }
   
    free(vertices_percorridos);

    return ( num_vertices_percorridos == (int) n_vertices(g)) ? 1 : 0 ;       
}

vertice in(grafo g, vertice *percorridos, int num_vertices_percorridos) {
    int is_in=0;

    for(int i=0; i<g->num_vertices; ++i) {
        for (int j=0; (j<num_vertices_percorridos) && (is_in==0); ++j) {
            if (!strcmp(g->vertices[i],percorridos[j])) {
                is_in = 1;
            }
        }
        if (!is_in) {
            return g->vertice[i];
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
void percorre_vertices(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos) {
    
    if (ja_visitou(vertices_percorridos, *num_vertices_percorridos, v))
        return;

    //printf("nome %s %p grau %d\n", v->nome, v, v->grau);
    vertices_percorridos[*num_vertices_percorridos] = v;
    (*num_vertices_percorridos)++; 

    for (int i=0 ; i < v->grau ; ++i) {

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
    
    printf("Já visitou? %d\n", ja_visitou(vertices_percorridos, *num_vertices_percorridos, v));

    if (ja_visitou(vertices_percorridos, *num_vertices_percorridos, v))
        return;

    printf("Vértice: nome %s %p grau %d\n", v->nome, v, v->grau);
    vertices_percorridos[*num_vertices_percorridos] = v;
    (*num_vertices_percorridos)++; 


    for (int i=0 ; i < v->grau ; ++i) {

        printf("Antes do if do %s, %d\n", v->nome, i);

        if (v->arestas[i] != NULL) {
            if (v->arestas[i]->tail != NULL) { 
                if (v->arestas[i]->tail->nome != NULL) { 
                    printf("%s\n", v->arestas[i]->tail->nome);
                    if (!strcmp(v->arestas[i]->tail->nome, v->nome)) {
                        printf("Aresta: a[%d] head %s %p tail %s %p\n", i, v->arestas[i]->head->nome, 
                            v->arestas[i]->head, v->arestas[i]->tail->nome, v->arestas[i]->tail);

                        percorre_vertices_direcionado(v->arestas[i]->head, vertices_percorridos, num_vertices_percorridos);
                    }
                }
            }
        }
        printf("Depois do if\n");
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

int main(void) {
	
	grafo g = le_grafo(stdin);

	escreve_grafo(stdout, g);
	
	printf("nome %s\n", nome(g));
	
	printf("n_vertices %d\n", n_vertices(g));

	printf("direcionado %d\n", direcionado(g));

    if (direcionado(g)) {
       printf("conexo %d\n", fortemente_conexo(g));    
    } else {
	   printf("conexo %d\n", conexo(g));
    }

	return ! destroi_grafo(g);

}