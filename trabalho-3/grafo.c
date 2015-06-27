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
	return ( (l == NULL) ? NULL : l->n );
}

void prepend(lista l, no n) {
    no ln = primeiro_no(l);
    l->n = n;
    n->prox = ln;
    l->tam++;
}

void append(lista l, no n) {
    no ln = primeiro_no(l);
    if (ln == NULL) {
        l->n = n;
        l->tam++;
        return;
    }

    while (ln->prox != NULL) {
        ln = proximo_no(ln);
    }
    ln->prox = n;
    l->tam++;
}

// NO ------------------------------------------------------------------------------
no proximo_no(no n) {
	return ( (n == NULL) ? NULL : n->prox );
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

	for (unsigned int i=1; i<num_vertices ; i++) {
		v[i] = v[i-1] + 1;
	}

	for (unsigned int i=0; i<num_vertices; i++) {
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

    for (unsigned int i=0; i<num_vertices; i++) {

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

int visitou_aresta(aresta *arestas_percorridas, int num_arestas_percorridas, aresta a) {

    for (int i=0; i<num_arestas_percorridas; ++i) {

        if (arestas_percorridas[i] == a )
            return 1;
    }
    return 0;
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
            return g->vertices[i];
        }
    }
    printf("Não achou\n");
    return NULL;
}

void percorre_componente(vertice v, grafo g, vertice *vertices_percorridos, int *num_vertices_percorridos, aresta *arestas_percorridas, int *num_arestas_percorridas) {

    if (ja_visitou(vertices_percorridos, *num_vertices_percorridos, v))
        return;

    g->vertices[g->num_vertices] = v;
    printf("%s\n",g->vertices[g->num_vertices]->nome);
    g->num_vertices++;

    vertices_percorridos[*num_vertices_percorridos] = v;
    (*num_vertices_percorridos)++;

    for (int i=0 ; i < v->grau ; ++i) {

        if( visitou_aresta(arestas_percorridas, 
                *num_arestas_percorridas, v->arestas[i])) {
            return;
        }

        arestas_percorridas[*num_arestas_percorridas] = v->arestas[i];
        (*num_arestas_percorridas)++; 

        g->arestas[g->num_arestas] = v->arestas[i];
        g->num_arestas++;

        printf("Vertice: %s, grau %d, iter %d\n", v->nome, v->grau, i);
        printf("Aresta: head %s tail %s\n", v->arestas[i]->head, v->arestas[i]->tail);

        if ( v == v->arestas[i]->head ) {
            percorre_componente(v->arestas[i]->tail, g, vertices_percorridos, num_vertices_percorridos, arestas_percorridas, num_arestas_percorridas);
        } else if ( v == v->arestas[i]->tail ) {
            percorre_componente(v->arestas[i]->head, g, vertices_percorridos, num_vertices_percorridos, arestas_percorridas, num_arestas_percorridas);
        }
    }

}

lista componentes(grafo g) {
    lista l = (lista) malloc((size_t) sizeof(struct lista));
    l->tam = 0;

    vertice *vertices_percorridos;
    aresta *arestas_percorridas;
    int num_vertices_percorridos = 0;
    int num_arestas_percorridas = 0;

    vertices_percorridos = malloc ( n_vertices(g) * sizeof(vertice) );
    arestas_percorridas = malloc (g->num_arestas * sizeof(aresta) );

    while (1) { 
        vertice v = in(g, vertices_percorridos, num_vertices_percorridos);
        if (v == NULL) {
            break;
        }

        grafo gr = (grafo) malloc((size_t) sizeof(struct grafo));
        gr = init_grafo();
        gr->direcionado = direcionado(g);
        gr->tem_peso = g->tem_peso;

        gr->num_vertices = 0;
        gr->num_arestas = 0;    
        
        gr->vertices = init_vertices(g->num_vertices);
        gr->arestas = init_arestas(g->num_arestas);

        strcpy(gr->nome, "componente");

        percorre_componente(v, gr, vertices_percorridos, &num_vertices_percorridos, arestas_percorridas, &num_arestas_percorridas);

        escreve_grafo(stdout, gr);

        no n = (no) malloc((size_t) sizeof(struct no));
        n->conteudo = (grafo) gr;
        printf("Nome n: %s\n", ((grafo)n->conteudo)->nome);
        append(l, n);

        printf("Nome l: %s\n", ((grafo) ((no) l->n)->conteudo)->nome);
    }

    free(vertices_percorridos);
    free(arestas_percorridas);

    return l;
    //return ( num_vertices_percorridos == (int) n_vertices(g)) ? 1 : 0 ;       
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
    
    if (ja_visitou(vertices_percorridos, *num_vertices_percorridos, v))
        return;

    vertices_percorridos[*num_vertices_percorridos] = v;
    (*num_vertices_percorridos)++; 


    for (int i=0 ; i < v->grau ; ++i) {

        // Um monte de ifs pra evitar erro de acesso à memória
        if (v->arestas[i] != NULL) {
            if (v->arestas[i]->tail == v) { 
                if (!strcmp(v->arestas[i]->tail->nome, v->nome)) {

                    percorre_vertices_direcionado(v->arestas[i]->head, vertices_percorridos, num_vertices_percorridos);
                }
            }
        }
            
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

void escreve_lista_de_grafos(lista l) {
    no n = primeiro_no(l);
    for (int i=0; i<l->tam; ++i) {
        grafo g = (grafo) n->conteudo;
        printf("Nome: %s\n", g->nome);
        //escreve_grafo(stdout, (grafo) n->conteudo);
        n = proximo_no(n);
    }
}

void escreve_lista_de_vertices(lista l) {
    no n = primeiro_no(l);
    for (int i=0; i<l->tam; ++i) {
        vertice v = (vertice) n->conteudo;
        printf("Nome: %s\n", v->nome);
        //escreve_grafo(stdout, (grafo) n->conteudo);
        n = proximo_no(n);
    }
}

void visita(vertice v, lista l, vertice *marcas, int *num_marcas) {
    
    if (ja_visitou(marcas, *num_marcas, v))
        return;
    
    marcas[*num_marcas] = v;
    (*num_marcas)++; 

    for (int i=0 ; i < v->grau ; ++i) {
        
        //printf("Vertice: %s, %p\n", v->nome, v->arestas[i]  );

        if ( v == v->arestas[i]->tail ) {
            visita(v->arestas[i]->head, l, marcas, num_marcas);
        } /*else {
            visita(v->arestas[i]->head, l, marcas, num_marcas);
        }*/

    }

    no n = (no) malloc((size_t) sizeof(struct no));
    n->conteudo = (vertice) v;

    prepend(l, n);
}

lista ordena(grafo g) {
    
    if (!g->direcionado) {
        printf("Não direcionado!\n");
        return NULL;
    }

    lista l = (lista) malloc((size_t) sizeof(struct lista));
    l->tam = 0;

    vertice *marcas;
    //vertice *marcas_permanentes;
    int num_marcas = 0;
    //int num_marcas_permanentes = 0;

    marcas = malloc ( n_vertices(g) * sizeof(vertice) );
    //arestas_percorridas = malloc ( g->num_arestas * sizeof(aresta) );

    while(1) {
        vertice v = in(g, marcas, num_marcas);
        if (v == NULL) {
            break;
        }
        visita(v, l, marcas, &num_marcas);
    }

    return l;
}

int main(void) {
	
	grafo g = le_grafo(stdin);

	//escreve_grafo(stdout, g);
	
	printf("nome %s\n", nome(g));
	
	printf("n_vertices %d\n", n_vertices(g));

	printf("direcionado %d\n", direcionado(g));

    /*if (direcionado(g)) {
       printf("conexo %d\n", fortemente_conexo(g));    
    } else {
       printf("conexo %d\n", conexo(g));
    }*/

    lista l = ordena(g);
    if (l != NULL) {
        escreve_lista_de_vertices(l);
    }

    //lista l = componentes(g);
    //escreve_lista_de_grafos(l);

    return 1;

	return ! destroi_grafo(g);

}
