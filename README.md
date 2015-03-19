<body>
<div class="document" id="trabalho-de-implementacao-1">
<h1 class="title">Trabalho de Implementação 1</h1>

<div class="section" id="ci065-ci755-algoritmos-e-teoria-dos-grafos">
<h1>CI065 / CI755 - Algoritmos e Teoria dos Grafos</h1>
<p>Para a representação de grafos na entrada e saída ao longo da
disciplina vamos utilizar a linguagem de descrição de grafos <a class="reference external" href="http://en.wikipedia.org/wiki/DOT_%28graph_description_language%29">dot</a>
que é implementada pelo pacote de software <a class="reference external" href="http://www.graphviz.org/">GraphViz</a>. Para uma amostra da linguagem <em>dot</em>
veja esta <a class="reference external" href="http://www.graphviz.org/Gallery.php">galeria de grafos</a>.</p>
<p>Usaremos as rotinas de leitura da biblioteca <a class="reference external" href="http://www.graphviz.org/pdf/cgraph.3.pdf">cgraph</a> (que faz parte do pacote
GraphViz) para fazer a leitura dos arquivos na linguagem <em>dot</em>.</p>
<p>A biblioteca <em>cgraph</em> implementa (entre outras coisas) uma estrutura
de dados para a manipulação de grafos e um interpretador (&quot;parser&quot;)
da linguagem <em>dot</em>.</p>
<p>O objetivo deste trabalho é implementar uma interface com a biblioteca
<em>cgraph</em> que permita</p>
<ul class="simple">
<li>ler grafos descritos na linguagem <em>dot</em> em toda sua flexibilidade,
sem ter o trabalho de implementar um interpretador para a linguagem, e</li>
<li>usar a estrutura de dados montada pela biblioteca <em>cgraph</em> para
construir sua própria estrutura de dados para a representação de
grafos.</li>
</ul>
<p>As rotinas e estruturas de dados implementadas neste trabalho serão
utilizadas nos trabalhos posteriores, com eventuais adaptações, conforme
a necessidade.</p>
<p>O pacote <em>GraphViz</em> é <a class="reference external" href="http://www.graphviz.org/Download.php">distribuido livremente</a> para diversas plataformas.</p>
<p>Em particular, nas distribuições de <a class="reference external" href="http://en.wikipedia.org/wiki/Linux">GNU/Linux</a> baseadas na distribuição
<a class="reference external" href="https://www.debian.org/">Debian</a>, basta instalar o pacote
<a class="reference external" href="https://packages.debian.org/wheezy/libgraphviz-dev">libgraphviz-dev</a>.
Recomenda-se também instalar os pacotes  <a class="reference external" href="https://packages.debian.org/wheezy/graphviz">graphviz</a> (utilitários,
especialmente para desenhar grafos) e <a class="reference external" href="https://packages.debian.org/wheezy/graphviz-doc">graphviz-doc</a> (documentação
adicional).</p>
<p>Estes pacotes estão instalados nos servidores da rede do Departamento
de Informática.</p>
<hr class="docutils" />
<p>Para auxiliar a execução do trabalho, é fornecido o arquivo <a class="reference external" href="trabalho-1.tar.gz">trabalho-1.tar.gz</a> cujo conteúdo, uma vez expandido, é o seguinte.</p>
<table class="docutils field-list" frame="void" rules="none">
<col class="field-name" />
<col class="field-body" />
<tbody valign="top">
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">trabalho-1/grafo.h</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body"><p class="first">a especificação do que deve ser implementado</p>
</td>
</tr>
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">trabalho-1/grafo.c</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body"><p class="first">&quot;esqueleto&quot; da implementação a ser feita</p>
</td>
</tr>
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">trabalho-1/exemplo.c</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body"><p class="first">um programa para ilustrar o uso da
biblioteca <em>cgraph</em></p>
</td>
</tr>
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">trabalho-1/teste.c</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body"><p class="first">um programa para testar sua implementação</p>
</td>
</tr>
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">trabalho-1/*.dot</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body"><p class="first">grafos no formato <em>dot</em></p>
</td>
</tr>
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">trabalho-1/makefile</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body"><p class="first">um <em>makefile</em> que</p>
<ul class="last simple">
<li>compila <tt class="docutils literal">exemplo.c</tt> e <tt class="docutils literal">teste.c</tt> com as
opções de compilação que serão usadas na
correção</li>
<li>usa programas do pacote <em>GraphViz</em> para
gerar imagens dos grafos em <tt class="docutils literal">*.dot</tt></li>
</ul>
</td>
</tr>
</tbody>
</table>
<hr class="docutils" />
<p>O  trabalho deve  ser  entregue sob  a  forma de  um  arquivo de  nome
<tt class="docutils literal"><span class="pre">fulano-sicrano.tar.gz</span></tt>,  sendo que  <tt class="docutils literal">fulano</tt> e  <tt class="docutils literal">sicrano</tt> devem
ser     substituídos     pelos    <a class="reference external" href="http://www.inf.ufpr.br/renato/ci065">identificadores     dos     autores</a>.</p>
<p>O arquivo <tt class="docutils literal"><span class="pre">fulano-sicrano.tar.gz</span></tt>, uma  vez expandido, deve criar
(somente) os seguintes arquivos.</p>
<table class="docutils field-list" frame="void" rules="none">
<col class="field-name" />
<col class="field-body" />
<tbody valign="top">
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">fulano-sicrano/readme.txt</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body">arquivo  que   deve  ser   usado  para
comunicar tudo que seja relevante para
a correção do trabalho.</td>
</tr>
<tr class="field"><th class="field-name" colspan="2"><tt class="docutils literal"><span class="pre">fulano-sicrano/grafo.c</span></tt>:</th></tr>
<tr class="field"><td>&nbsp;</td><td class="field-body">a   implementação   do  especificado   em
<tt class="docutils literal"><span class="pre">trabalho-1/grafo.h</span></tt>.</td>
</tr>
</tbody>
</table>
<hr class="docutils" />
<p>O arquivo  <tt class="docutils literal"><span class="pre">fulano-sicrano.tar.gz</span></tt> deve  ser entregue como  anexo de
mensagem  enviada  para <a class="reference external" href="mailto:renato.carmo.rc&#64;gmail.com">renato.carmo.rc&#64;gmail.com</a>.   O  &quot;<tt class="docutils literal">Subject:</tt>&quot;
desta mensagem deve ser &quot;<tt class="docutils literal">Entrega do trabalho 1</tt>&quot;.</p>
<p>O prazo para a entrega é  <strong>29/3/2015, às 23h59min</strong>.</p>
<hr class="docutils" />
</div>
<div class="section" id="perguntas-frequentes">
<span id="faq"></span><h1>Perguntas Frequentes</h1>
<ol class="arabic">
<li><p class="first"><em>O trabalho pode ser feito em grupo?</em></p>
<p>O trabalho pode ser feito em duplas ou individualmente.</p>
<p>No caso de  trabalhos individuais o nome do arquivo  a ser entregue
deve  ser   <tt class="docutils literal">fulano.tar.gz</tt>,  sendo   que  <tt class="docutils literal">fulano</tt>   deve  ser
substituido        pelo        <a class="reference external" href="http://www.inf.ufpr.br/renato/ci065">identificador       do        autor</a>.</p>
</li>
<li><p class="first"><em>Se eu fizer este trabalho em dupla/individualmente terei que fazer os
próximos da mesma forma?</em></p>
<p>Não. A cada trabalho você poderá escolher fazer individualmente ou
em dupla com outro aluno.</p>
</li>
<li><p class="first"><em>O arquivo</em>  <tt class="docutils literal">grafo.c</tt> <em>pode conter  outras funções/variáveis/tipos
além daqueles presentes no esqueleto fornecido?</em></p>
<p>Pode (e, para boa organização do código, deve).  Como usual, a
especificação do trabalho descreve somente a interface a ser
implementada. A maneira de implementar é livre.</p>
</li>
<li><p class="first"><em>O que será levado em conta na correção?</em></p>
<p>Na correção serão levados em conta (entre outros) os seguintes elementos.</p>
<ol class="arabic simple">
<li>Conformidade com a especificação.</li>
<li>Correção da implementação.</li>
<li>Organização e clareza  do código (nomes de  funções e variáveis,
comentários etc).</li>
<li>Eficiência da implementação.</li>
</ol>
</li>
<li><p class="first"><em>Por que a especificação de entrega é importante?</em></p>
<p>Porque o trabalho entregue será  pré-processado por um programa que
depende de a especificação de entrega ser corretamente observada.</p>
</li>
<li><p class="first"><em>O que acontece se a  especificação de entrega não for corretamente
observada?</em></p>
<p>Seu trabalho  só será corrigido  quando o professor tiver  tempo de
fazer manualmente  o pré-processamento. Por isso,  haverá um severo
desconto na sua nota, proporcional ao trabalho de pré-processamento
que tenha que ser feito manualmente.</p>
</li>
<li><p class="first"><em>Descobri um  erro depois que  entreguei o trabalho.  Posso entregar
uma versão corrigida?</em></p>
<p>Você  pode entregar  o  trabalho mais  de uma  vez.   A última  versão
entregue dentro do prazo é a que será corrigida.</p>
</li>
<li><p class="first"><em>Meu trabalho tem um bug. O que vai acontecer com minha nota?</em></p>
<p>Haverá algum desconto, dependendo da  gravidade do bug.  O desconto
será  menor  se  o  bug for  relatado  no  arquivo  <tt class="docutils literal">readme.txt</tt>,
indicando que você estava ciente do problema quando entregou.</p>
</li>
<li><p class="first"><em>Tenho outra pergunta/dúvida a respeito do trabalho.</em></p>
<p>Envie mensagem para a <a class="reference external" href="mailto:Algoritmos%20e%20Teoria%20dos%20Grafos%20%3Cci065&#64;listas.inf.ufpr.br%3E">lista da disciplina</a>.</p>
</li>
</ol>
</div>
</div>
</body>