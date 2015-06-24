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

int copia_arestas(Agedge_t *a, grafo g, int i, int num_arestas_visitadas);

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
       
      
          
        if ( (buffer = agget(a,peso) ) ) { 
          
            g->tem_peso = 1; 
            g->arestas[num_arestas_visitadas]->peso = atol(buffer);
        }

        free(peso);
       
        num_arestas_visitadas++;
    }

    return num_arestas_visitadas;
}

//------------------------------------------------------------------------------
grafo le_grafo(FILE *input) {

	Agraph_t *graf = agread(input, NULL);

	grafo g = NULL;
	g = init_grafo();

	g->direcionado = agisdirected(graf);
	g->num_vertices = (unsigned) agnnodes(graf);    
	g->num_arestas = (unsigned) agnedges(graf);    

	g->vertices = init_vertices(g->num_vertices);
	g->arestas = init_arestas(g->num_arestas);

	strcpy(g->nome, agnameof(graf));

	// Copia Vértices
	int i = 0;
	for (Agnode_t *v=agfstnode(graf); v; v=agnxtnode(graf,v)) {
		g->vertices[i]->grau = agdegree(graf,v,1,1);

		g->vertices[i]->arestas = (aresta *) malloc (
				(size_t)g->vertices[i]->grau*sizeof(aresta));

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

int main(void) {
	
	grafo g = le_grafo(stdin);

	escreve_grafo(stdout, g);
	
	printf("nome %s\n", nome(g));
	
	printf("n_vertices %d\n", n_vertices(g));

	printf("direcionado %d\n", direcionado(g));

	printf("conexo %d\n", conexo(g));

	return ! destroi_grafo(g);

	}
