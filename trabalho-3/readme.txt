CI065 - Algoritmos e Teoria dos Grafos

GRR2011???? - Cainã Costa Trevisan
GRR20114506 - Renan Domingos Merlin Greca


1. Dependências.

	Para compilar o programa grafo.c, é necessário ter instalada a biblioteca graphviz, incluindo a parte graphviz-dev.

2. Compilando e executando o programa.

	Para compilar, basta usar o make:

	$ make

	Para executar o programa, faça o seguinte:
	
	$ ./grafo < [arquivo dot]

	Utilizando um grafo no formato .dot.

3. Sobre os algoritmos utilizados e implementados.

	As funcionalidades implementadas em grafo.c são as seguintes:

	3.1 - conexo(grafo g)
		Esta função retorna 1 caso g seja conexo ou 0 caso contrário.
		Para isso, ela faz uma busca em profundidade e verifica se essa busca percorreu todos os vértices do grafo. Se sim, o grafo é conexo.

	3.2 - fortemente_conexo(grafo g)
		Esta função retorna 1 caso g seja fortemente conexo ou 0 caso contrário.
		Funciona de maneira similar à conexo(), mas leva em consideração a direcionalidade de g.

	3.3 - arvore_geradora_minima(grafo g)
		Esta função retorna a árvore geradora mínima de g.
		Para isso, foi usado um algoritmo que remove as arestas de g e as adiciona a medida que são encontrados os caminhos mais baratos. Esse algoritmo é baseado no de Borůvka (1926).

	3.4 - componentes(grafo g)
		Esta função retorna os componentes de g como uma lista de grafos.
		Para isso, ela encontra um vértice e faz uma busca em profundidade a partir desse vértice, adicionando todos os vértices e arestas encontrados a um novo grafo gr. Esse grafo é adicionado à lista e então repete-se a operação usando como raiz da busca um novo vértice que ainda não foi adicionado a qualquer um dos componentes já listados. Isso continua até que não haja mais vértices que não pertençam a componentes.

	3.5 - blocos(grafo g)
		Esta função retorna os blocos de g como uma lista de grafos.
		Para isso, ela primeiro encontra todos os vértices de corte de g utilizando um algoritmo baseado no de Hopcroft e Tarjan (1973). Após isso, cria-se uma cópia de g que carece de seus vértices de corte e então utiliza-se a função componentes() para listar os blocos.

	3.6 - ordena(grafo g)
		Esta função retorna os vértices de g em ordem topológica como uma lista de vértices.
		Para isso, é utilizado um algoritmo que visita os vértices recursivamente e adiciona-os a uma lista. Este algoritmo é baseado no de Cormen, Leiserson, Rivest e Stein (2001).

	3.7 - arborescencia_caminhos_mínimos(grafo g, vertice r)

	3.8 - distancias(grafo g)

	3.9 - diametro(grafo g)

