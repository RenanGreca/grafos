//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo simples
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

#define TAM_NOME 56

typedef struct vertice *vertice;
typedef struct aresta *aresta;
typedef struct grafo *grafo;

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

grafo init_grafo(void);

vertice *init_vertices(int num_vertices);

aresta *init_arestas(int num_arestas);

int busca_aresta(aresta *arestas, int num_arestas, char *nome_head, char *nome_tail);

int busca_vertice(vertice *vertice, int num_vertices, char *nome);

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso ou
//         0 caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot, de forma que
// 1. todos os vértices são escritos antes de todas as arestas/arcos 
// 2. se uma aresta tem peso, este deve ser escrito como um atributo
//
// devolve o grafo escrito ou
//         NULL em caso de erro 

void escreve_vertices(vertice *vertices, int n_vertices, FILE *output);

void escreve_arestas(aresta *arestas, int n_arestas, int direcionado, int arestas_tem_peso, FILE *output);

grafo escreve_grafo(FILE *output, grafo g);
