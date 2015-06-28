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

void prepend(lista l, no n);
void append(lista l, no n);
int indice(grafo g, vertice v);
int indice_a(grafo g, aresta a);
int ja_visitou(vertice *vertices_percorridos, int num_vertices_percorridos, vertice v);
int visitou_aresta(aresta *arestas_percorridas, int num_arestas_percorridas, aresta a);
vertice vertice_not_in(grafo g, int *marcas);

grafo init_grafo(void);
vertice *init_vertices(unsigned int num_vertices);
aresta *init_arestas(unsigned int num_arestas);

int busca_vertice(vertice *vertices, unsigned int num_vertices, char *nome);
int busca_aresta(aresta *arestas, unsigned int num_arestas, char *nome_head, char *nome_tail);

int copia_arestas(Agedge_t *a, grafo g, int i, int *num_arestas_visitadas); 

void escreve_vertices(vertice *vertices, unsigned int n_vertices, FILE *output);
void escreve_arestas(aresta *arestas, unsigned int n_arestas, int direcionado, int tem_peso, FILE *output);

void escreve_lista_de_grafos(lista l);
void escreve_lista_de_vertices(lista l);

vertice in(grafo g, vertice *percorridos, int num_vertices_percorridos);
void percorre_componente(grafo g, vertice v, grafo comp, int *marcas_v, int *marcas_a);

void acha_cortes(grafo g, vertice pai, vertice v, int *visitado, int *profundidade, int *lowpoint, int *corte, int p);
grafo remove_vertices(grafo g, int *corte);

void percorre_vertices(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos);
void percorre_vertices_direcionado(vertice v, vertice *vertices_percorridos, int *num_vertices_percorridos);

int visita(grafo g, vertice v, lista l, int *marcas_temp, int *marcas_perm);

void arestas_do_vertice(grafo g, vertice v, aresta *a);
aresta aresta_mais_barata(grafo g, grafo c);

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
    if (l->tam == 0) {
        l->n = n;
        l->tam++;
        return;
    }

    no ln = primeiro_no(l);

    int i;
    for (i=0; i<l->tam-1; ++i) {  
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

int indice(grafo g, vertice v) {
    for (unsigned int i = 0; i<g->num_vertices; ++i) {
        if (g->vertices[i] == v) {
            return (int) i;
        }
    }
    return -1;
}

int indice_a(grafo g, aresta a) {
    for (unsigned int i = 0; i<g->num_arestas; ++i) {
        if (g->arestas[i] != NULL) {
            //printf("Aresta 1: %s - %s\n", g->arestas[i]->head->nome, g->arestas[i]->tail->nome);
            //printf("Aresta 2: %s - %s\n", a->head->nome, a->tail->nome);
            if (g->arestas[i] == a) {
                return (int) i;
            }
        }
    }
    return -1;
}

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

vertice vertice_not_in(grafo g, int *marcas) {
    for (unsigned int i=0; i<g->num_vertices; ++i) {
        if (!marcas[i]) {
            return g->vertices[i];
        }
    }
    return NULL;
}

// FUNÇÕES DE INICIALIZAÇÃO
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

    for (unsigned int i=1; i<num_arestas ; i++) {
        a[i] = a[i-1] + 1;
    }

    for (unsigned int i=0; i<num_arestas; i++) {
        a[i]->peso = 0.0;
        a[i]->tail = NULL;
        a[i]->head = NULL;
    }

    return a;
}

// FUNÇÕES DE BUSCA
//------------------------------------------------------------------------------
int busca_vertice(vertice *vertices, unsigned int num_vertices, char * nome) {

    for (unsigned int i=0; i<num_vertices; i++) {

        if (!strcmp(nome, vertices[i]->nome)) {

            return (int) i;
        }
    }

    return -1;
}

int busca_aresta(aresta *arestas, unsigned int num_arestas, char *nome_head, char *nome_tail) {
  
    for (unsigned int i=0; i<num_arestas; i++) {
        
		if (arestas[i]->head== NULL) {
   
            return -1;
		}

		if (arestas[i]->tail== NULL) {
   
            return -1;
		}

        if (    !strcmp( nome_head, arestas[i]->head->nome) &&
                !strcmp( nome_tail, arestas[i]->tail->nome) ) {
   
            return (int) i;  
        }

    }
	
    return -1;
}

int copia_arestas(Agedge_t *a, grafo g, int num_vertices_visitados, int *num_arestas_visitadas) {
    
    char *buffer; 
    char *peso = strdup("peso"); 
    int v_pos;
    int a_pos;


    if ( ( a_pos = busca_aresta(g->arestas, (unsigned int) *num_arestas_visitadas, agnameof(aghead(a)), 
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
            g->arestas[*num_arestas_visitadas]->peso = atol(buffer);
        }

        free(peso);
       
        (*num_arestas_visitadas)++;
        return (*num_arestas_visitadas) - 1;
    }

    return a_pos;
}

// LEITURA DE UM GRAFO .DOT
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

// FUNÇÕES DE IMPRESSÃO
//------------------------------------------------------------------------------
void escreve_vertices(vertice *vertices, unsigned int n_vertices, FILE *output) {
    
    if (!n_vertices) {

        return;
    }

    for (unsigned int i=0; i<n_vertices; i++) {
        if (vertices[i] != NULL) {
            fprintf(output, "    \"%s\"\n", vertices[i]->nome);
        }
    }
}

void escreve_arestas(aresta *arestas, unsigned int n_arestas, int direcionado, int tem_peso, FILE *output) {

    char rep_aresta = direcionado ? '>' : '-';
    long peso;

    for (unsigned int i=0; i<n_arestas; i++) {
        
        if (arestas[i] != NULL) {
           fprintf(output, "    \"%s\" -%c \"%s\"",
                    arestas[i]->tail->nome,
                    rep_aresta,
                    arestas[i]->head->nome
            );

            if ( tem_peso ) {
                peso = arestas[i]->peso;
                fprintf(output, " [peso=%ld]", peso);
            }

            fputc('\n', output);
        }
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

void escreve_lista_de_grafos(lista l) {
    no n = primeiro_no(l);
    for (int i=0; i<l->tam; ++i) {
        //grafo g = (grafo) n->conteudo;
        escreve_grafo(stdout, (grafo) n->conteudo);
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

// EXTERMINATE!
//------------------------------------------------------------------------------
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

// FUNÇÕES PARA COMPONENTES
//------------------------------------------------------------------------------
vertice in(grafo g, vertice *percorridos, int num_vertices_percorridos) {
    int is_in=0;

    for(unsigned int i=0; i<g->num_vertices; ++i) {
        is_in = 0;
        for (int j=0; (j<num_vertices_percorridos) && (is_in==0); ++j) {
            if (!strcmp(g->vertices[i]->nome,percorridos[j]->nome)) {
                is_in = 1;
            }
        }
        if (!is_in) {
            return g->vertices[i];
        }
    }
    return NULL;
}

void arestas_do_vertice(grafo g, vertice v, aresta *a) {
    int j=0;
    for (unsigned int i=0; i<g->num_arestas; ++i) {
        if (g->arestas[i] != NULL && g->arestas[i] != NULL) {
            if (g->arestas[i]->head == v || g->arestas[i]->tail == v) {
                a[j] = g->arestas[i];
                j++;
            }
        }
    }
}

void percorre_componente(grafo g, vertice v, grafo comp, int *marcas_v, int *marcas_a) {

    int j = indice(g, v);

    if (j < 0) 
        return;
    if (marcas_v[j])
        return;
    
    comp->vertices[comp->num_vertices] = v;
    comp->num_vertices++;

    marcas_v[j] = 1;

    aresta arestas[v->grau];
    arestas_do_vertice(g, v, arestas);

    for (int i=0 ; i < v->grau ; ++i) {
        if (arestas[i] != NULL) {
            int k = indice_a(g, arestas[i]);
            if (k < 0)
                continue;
            if (marcas_a[k])
                continue;

            comp->arestas[comp->num_arestas] = arestas[i];
            comp->num_arestas++;

            marcas_a[k] = 1;

            if ( v == arestas[i]->head ) {
                percorre_componente(g, arestas[i]->tail, comp, marcas_v, marcas_a);
            } else if ( v == arestas[i]->tail ) {
                percorre_componente(g, arestas[i]->head, comp, marcas_v, marcas_a);
            }

        }
    }

}

lista componentes(grafo g) {
    lista l = (lista) malloc((size_t) sizeof(struct lista));
    l->tam = 0;

    int marcas_v[g->num_vertices];
    memset(marcas_v, 0, (g->num_vertices)*sizeof(int));

    int marcas_a[g->num_arestas];
    memset(marcas_a, 0, (g->num_arestas)*sizeof(int));

    /*vertice *vertices_percorridos;
    aresta *arestas_percorridas;
    int num_vertices_percorridos = 0;
    int num_arestas_percorridas = 0;

    vertices_percorridos = malloc ( n_vertices(g) * sizeof(vertice) );
    arestas_percorridas = malloc (g->num_arestas * sizeof(aresta) );*/

    while (1) { 
        vertice v = vertice_not_in(g, marcas_v);
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

        percorre_componente(g, v, gr, marcas_v, marcas_a);

        no n = (no) malloc((size_t) sizeof(struct no));
        n->conteudo = (grafo) gr;
        append(l, n);

    }

    return l;
}

// FUNÇÕES PARA BLOCOS
//------------------------------------------------------------------------------
void acha_cortes(grafo g, vertice pai, vertice v, int *visitado, int *profundidade, int *lowpoint, int *corte, int p) {
    int i = indice(g, v);

    if (i < 0) {
        return;
    }


    visitado[i] = 1;
    profundidade[i] = p;
    lowpoint[i] = p;

    int num_filhos = 0;
    int is_corte = 0;

    for (int j = 0; j < v->grau; ++j) {
        int k;
        if ( v == v->arestas[j]->head ) {
            k = indice(g, v->arestas[j]->tail);
        } else {
            k = indice(g, v->arestas[j]->head);
        }

        if (!visitado[k]) {
            acha_cortes(g, v, g->vertices[k], visitado, profundidade, lowpoint, corte, p+1);
            num_filhos++;
            if (lowpoint[k] >= profundidade[i]) {
                is_corte = 1;
            }
            lowpoint[i] = (lowpoint[i] < lowpoint[k] ? lowpoint[i] : lowpoint[k]);
        } else if (g->vertices[k] != pai) {
            lowpoint[i] = (lowpoint[i] < profundidade[k] ? lowpoint[i] : profundidade[k]);
        }
    }
    if ((pai != NULL && is_corte) || (pai == NULL && num_filhos > 1)) {
        corte[i] = 1;
    }

}

grafo remove_vertices(grafo g, int *corte) {

    grafo gr = (grafo) malloc((size_t) sizeof(struct grafo));
    gr = init_grafo();
    gr->direcionado = direcionado(g);
    gr->tem_peso = g->tem_peso;

    gr->num_vertices = 0;
    gr->num_arestas = 0;    
    
    gr->vertices = init_vertices(g->num_vertices);
    gr->arestas = init_arestas(g->num_arestas);

    strcpy(gr->nome, "bloco");

    for (unsigned int i=0; i<g->num_vertices; ++i) {
        if (!corte[i]) {
            gr->vertices[gr->num_vertices] = g->vertices[i];
            gr->num_vertices++;
        }
    }

    for (unsigned int k=0; k<g->num_arestas; ++k) {
        
        int i = indice(g, g->arestas[k]->head);
        int j = indice(g, g->arestas[k]->tail);

        if (!corte[i] && !corte[j]) {
            gr->arestas[gr->num_arestas] = g->arestas[k];
            gr->num_arestas++;
        }

        /*if (g->arestas[j] != NULL) {
            if ((g->arestas[j]->head == g->vertices[i]) ||
                (g->arestas[j]->tail == g->vertices[i])) {
                g->arestas[j] = NULL;
            }
        }*/
    }

    return gr;
}

lista blocos(grafo g) {
    if (g->direcionado) {
        printf("Função blocos não pode ser executada pois grafo é direcionado!\n");
        return NULL;
    }

    lista l = (lista) malloc((size_t) sizeof(struct lista));
    l->tam = 0;

    int corte[g->num_vertices];
    memset(corte, 0, (g->num_vertices)*sizeof(int));
    int visitado[g->num_vertices];
    memset(visitado, 0, (g->num_vertices)*sizeof(int));
    int profundidade[g->num_vertices];
    memset(profundidade, 0, (g->num_vertices)*sizeof(int));
    int lowpoint[g->num_vertices];
    memset(lowpoint, 0, (g->num_vertices)*sizeof(int));
    int p = 0;

    acha_cortes(g, NULL, g->vertices[0], visitado, profundidade, lowpoint, corte, p);

    grafo gr = remove_vertices(g, corte);

    /*no n = (no) malloc((size_t) sizeof(struct no));
    n->conteudo = gr;
    l->n = n;
    l->tam++;*/

    return componentes(gr);
}

// FUNÇÕES PARA CONEXO
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

int conexo(grafo g) {

    vertice *vertices_percorridos;
    int num_vertices_percorridos = 0;

    vertices_percorridos = malloc ( n_vertices(g) * sizeof(vertice) );
    
    percorre_vertices(g->vertices[0], vertices_percorridos, &num_vertices_percorridos);
   
    free(vertices_percorridos);

    return ( num_vertices_percorridos == (int) n_vertices(g)) ? 1 : 0 ;       
}

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

// FUNÇÕES PARA ORDENAÇÃO TOPOLÓGICA
//------------------------------------------------------------------------------
int visita(grafo g, vertice v, lista l, int *marcas_temp, int *marcas_perm) {
    
    int j = indice(g, v);

    if (marcas_temp[j])
        printf("Função ordena não pode ser executada pois grafo não é acíclico\n");
        return 0; // não é grafo acíclico

    if (marcas_perm[j])
        return 1;

    marcas_temp[j] = 1;    

    for (int i=0 ; i < v->grau ; ++i) {        
        if (v->arestas[i] != NULL) {
            if ( v == v->arestas[i]->tail ) {
                int a = visita(g, v->arestas[i]->head, l, marcas_temp, marcas_perm);
                if (!a) {
                    return 0;
                }
            }
        }
    }

    marcas_perm[j] = 1;
    marcas_temp[j] = 0;

    no n = (no) malloc((size_t) sizeof(struct no));
    n->conteudo = (vertice) v;

    prepend(l, n);
    return 1;
}

lista ordena(grafo g) {
    
    if (!g->direcionado) {
        printf("Função ordena não pode ser executada pois grafo não é direcionado!\n");
        return NULL;
    }

    lista l = (lista) malloc((size_t) sizeof(struct lista));
    l->tam = 0;

    int marcas_temp[g->num_vertices];
    memset(marcas_temp, 0, (g->num_vertices)*sizeof(int));
    int marcas_perm[g->num_vertices];
    memset(marcas_perm, 0, (g->num_vertices)*sizeof(int));

    while(1) {
        vertice v = vertice_not_in(g, marcas_perm);
        if (v == NULL) {
            break;
        }
        int a =  visita(g, v, l, marcas_temp, marcas_perm);
        if (a == 0) {
            printf("Função ordena não pode ser executada pois grafo não é acíclico!\n");
            // Não é um grafo acíclico
            return NULL;
        }
    }

    return l;
}

aresta aresta_mais_barata(grafo g, grafo c) {
    aresta arestas[g->num_arestas];
    int n_arestas = 0;

    for (unsigned int i=0; i<c->num_vertices; ++i) {
        vertice v = c->vertices[i];
        aresta a[v->grau];
        arestas_do_vertice(g, v, a);

        long menor_peso = infinito;
        aresta barata;
        for (int j=0; j<v->grau; ++j) {
            if (a[j]->peso < menor_peso) {
                menor_peso = a[j]->peso;
                barata = a[j];
            }
        }

        arestas[n_arestas] = barata;
        n_arestas++;        

    }

    long menor_peso = infinito;
    aresta barata;
  
    for (int i=0; i<n_arestas; ++i) {
        if (arestas[i]->peso < menor_peso) {
            menor_peso = arestas[i]->peso;
            barata = arestas[i];
        }
    }
    return barata;
}

grafo arvore_geradora_minima(grafo g) {
    if (direcionado(g)) {
        printf("Função árvore geradora mínima não pode ser executada pois grafo é direcionado!\n");
        return NULL;
    }
    if (!conexo(g)) {
        printf("Função árvore geradora mínima não pode ser executada pois grafo não é conexo!\n");
        return NULL;
    }

    int marcas_a[g->num_arestas];
    memset(marcas_a, 0, (g->num_arestas)*sizeof(int));

    grafo gr = (grafo) malloc((size_t) sizeof(struct grafo));
    gr = init_grafo();
    gr->direcionado = direcionado(g);
    gr->tem_peso = g->tem_peso;

    gr->num_vertices = g->num_vertices;
    gr->num_arestas = 0;    
    
    gr->vertices = g->vertices;
    gr->arestas = init_arestas(g->num_arestas);

    strcpy(gr->nome, "árvore");

    lista l = componentes(gr);
    int num_componentes = l->tam;

    while(num_componentes > 1) {
        no n = primeiro_no(l);
        for (int i=0; i<l->tam; ++i) {
            grafo c = (grafo) n->conteudo;
            
            aresta barata = aresta_mais_barata(g, c);
            int k = indice_a(g, barata);

            if (!marcas_a[k]) {
                gr->arestas[gr->num_arestas] = barata;
                gr->num_arestas++;
                marcas_a[k] = 1;
            }

            n = proximo_no(n);
        }

        l = componentes(gr);
        num_componentes = l->tam;
    }

    return gr;
}

int main(void) {
	
	grafo g = le_grafo(stdin);
    lista l;

	//escreve_grafo(stdout, g);
	
	printf("Nome: %s\n", nome(g));
	
	printf("n_vertices: %d\n", n_vertices(g));

	printf("direcionado: %d\n", direcionado(g));

    if (direcionado(g)) {
       printf("conexo: %d\n", fortemente_conexo(g));    
    } else {
       printf("conexo: %d\n", conexo(g));
    }

    printf("-------------------------------------\n");
    printf("Ordena:\n");
    l = ordena(g);
    if (l != NULL) {
        escreve_lista_de_vertices(l);
    }

    printf("-------------------------------------\n");
    printf("Blocos:\n");
    l = blocos(g);
    if (l != NULL) {
        escreve_lista_de_grafos(l);
    }

    printf("-------------------------------------\n");
    printf("Componentes:\n");
    l = componentes(g);
    if (l != NULL) {
        escreve_lista_de_grafos(l);
    }

    printf("-------------------------------------\n");
    printf("Árvore Geradora Mínima:\n");
    grafo arvore = arvore_geradora_minima(g);
    if (arvore != NULL) {
        escreve_grafo(stdout, arvore);
    }

    //lista l = componentes(g);
    //escreve_lista_de_grafos(l);

    return 1;

	return ! destroi_grafo(g);

}
