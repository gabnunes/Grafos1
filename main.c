#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Alunos: Gabriel Nunes Ribeiro Silva - 150126689
		   Gabriel Campos Albino - 150035187
   Disciplina: Teoria e Aplicação de Grafos - 2017/1
   Professor: Díbio
*/


//Declarações da lista

typedef struct celula_str *apontador;

typedef struct {
	int matricula, Vertice, grau;
	char *nome;
}tipoAluno;

typedef struct {
	apontador primeiro, ultimo;
}tipoLista;

typedef struct celula_str {
	tipoAluno amizades;
	apontador prox;
}celula;

//struct auxiliar para impressão decrescente dos vértices, juntamente com sua respectiva matrícula.
typedef struct {
	int grau, matricula;
}GrauVertice;

/* Função para criar uma lista. Note que essa implementação utiliza o conceito de lista com celula cabeça.
Aqui decidimos utilizar a célula cabeça para guardar as infomações do vértice. Apenas os próximos elementos se referem às amizades.
Dessa forma, ao criarmos uma lista, já introduziremos as informações(nome,matricula,grau) do vértice na célula cabeça.
 [Assim, o restante dos elementos irão compor a lista de adjacência desse vértice. */
void CriaLista(tipoLista *lista, tipoAluno x){
  lista->primeiro = (apontador) malloc(sizeof(celula));
  lista->ultimo = lista->primeiro;
  lista->primeiro->amizades = x;
  lista->primeiro->prox = NULL;
}

//Essa função tem como responsabilidade implementar a inserção dos amigos de um dado vértice em sua lista de adjacência.
void InsereLista(int numeroamigo,tipoLista *lista){
  lista->ultimo->prox=(apontador)malloc(sizeof(celula));
  lista->ultimo=lista->ultimo->prox;
  lista->ultimo->amizades.Vertice = numeroamigo;	//Insere apenas o número identificador(Vértice) do amigo.
  lista->ultimo->prox=NULL;
}

//Função que imprime, em ordem decrescente, os vértices com maior grau, juntamente com suas respectivas matrículas.
void ImprimeOrdem(GrauVertice Grau[]) {
	int i,j=1;
	printf("\nOrdem Decrescente da Quantidade de Amigos: \n");
	for(i=38;i>=0;i--) {	//Vetor está ordenado de forma crescente, então aqui utilizamos a impressão do final ao começo.
		printf("%do - %d - Grau:%d\n", j, Grau[i].matricula, Grau[i].grau);
		j++;
	}
}

/* Função para tornar o grafo criado Não-Direcionado.
Método: Através de um loop para percorrermos as 39 listas, verificamos os amigos do vetor[i], vamos até a lista de adjacência
de cada um desses amigos, e, assim, checamos se o vértice do vetor[i] se encontra na lista de adjacência desses amigos.
Caso não se encontre, adicionamos. Dessa forma, fazendo isso para todos os vértices, teremos uma simetria. 
Garantindo, assim, que o grafo se torne não-direcionado.*/
void Undirected(tipoLista vetor[]) {
	apontador aux,aux2,aux3;	/*Declaramos apontadores para percorrermos o vetor de listas.*/
	int i=0, verticechave;
	for(i=0;i<39;i++) {	/* "for" para varrermos todos os vértices.*/
		aux = vetor[i].primeiro;	/* aux = célula cabeça da lista 'i'. Logo, aux está apontando para a célula de informações de i.*/
		verticechave = i+1;	/* em verticechave armazenamos o número do vértice propriamente dito (que na verdade é i+1)*/
		aux2 = aux->prox; /* aux2 aponta para o vetor[i].primeiro->prox (Onde começa a lista de adjacência do vértice i+1)*/
		while(aux2 != NULL) { /*loop percorre a lista de adjacência do vertice i+1 (vetor[i])*/
			aux3 = vetor[(aux2->amizades.Vertice)-1].primeiro->prox; /*aux3 aponta para a lista de adjacência de amigo do vetor[i]*/
			while(aux3 != NULL && aux3->amizades.Vertice != verticechave) { /*varre a lista de adjacência desse amigo procurando o verticechave*/ 
				aux3 = aux3->prox;
			}
			if(aux3 == NULL) { /*Checa se a condição de saída foi porque chegou ao final ou se porque encontrou o verticechave*/
				InsereLista(verticechave,&vetor[(aux2->amizades.Vertice)-1]); /*Caso tenha saido porque chegou ao fim, não foi encontrado o verticechave*/
			}																  /*Então inserimos o verticechave em sua lista de adjacência.*/
			aux2 = aux2->prox;

		}
	}
}

/*BubbleSort para ordernarmos os graus dos vértices (o parâmetro é uma Struct que armazena grau e matrícula)*/
void bubble(GrauVertice vetor[]) {
	int i, j;
	GrauVertice aux;
	for(i=0;i<39;i++) {
		for(j=0;j<38;j++) {
			if(vetor[j].grau > vetor[j+1].grau) { /*Comparo os graus, mas a troca é da struct inteira, mantendo grau e matricula coerentes.*/
				aux = vetor[j];
				vetor[j] = vetor[j+1];
				vetor[j+1] = aux;
			}
		}
	}
}

/*Função que gerencia o ordenamento dos graus e prepara a impressão correta das matrículas.*/
void Ordena(tipoLista vetor[]) {
	GrauVertice graudovertice[39]; /*struct que armazena grau e matrícula*/
	int i, j=0;
	apontador aux;	/*ponteiro*/

	for(i=0;i<39;i++) {
		aux = vetor[i].primeiro->prox;	/*aux aponta para o primeiro elemento da lista de adjacência do vertice i+1(vetor[i])*/
		while(aux != NULL) { /*varre a lista de adjacência, contando a quantidade de células(nós).*/
			aux = aux->prox;
			j++;
		}
		vetor[i].primeiro->amizades.grau = j;	/*Salva a informação do grau do vértice i+1 na célula cabeça(Infos do vertice)*/
		graudovertice[i].grau = j;	/*Salva também o grau na struct auxiliar que utilizaremos para impressão*/
		graudovertice[i].matricula = vetor[i].primeiro->amizades.matricula;	/*Salva na struct a matricula do vetor[i]*/
		j=0;
	}
	bubble(graudovertice);	/*Chama a função bubble para ordernar os graus*/
	ImprimeOrdem(graudovertice);	/*Chama ImprimeOrdem para a impressão decrescente de graus e a respectiva matrícula.*/
}

/*Funcao PreencheVetor2 é responsável por transferir a lista de adjacência do vertice passado como parametro para um vetor 
denominado vetor2, que sérá utilizado na função cliquefinder. */
void PreencheVetor2(tipoLista lista[], int vetor2[], int vertice) {
	int i=0;
	apontador aux;
	aux = lista[vertice-1].primeiro->prox; //aux = lista de adjacência do vertice passado como parametro. (lista[vertice-1])
	while(aux!=NULL) {	//percorre a lista de adjacencia ate seu fim
		vetor2[i] = aux->amizades.Vertice;	//transfere vertice encontrado ao vetor "vetor2".
		aux = aux->prox;
		i++;
	}
}

/* Funcao ChecaLigacao e responsavel por checar se o vertice passado como parametro possui arestas que o ligam a todos
os outros vertices que estão no clique. Caso encontre uma aresta a um dos vertices, incrementa "counter".
A Função retornará o valor "counter", que será comparado ao tamanho do clique na função cliquefinder. Caso sejam iguais,
então o vertice passado como parametro sera adicionado ao clique. Se não forem iguais, então o vertice não possui aresta
a algum dos vertices presente no clique. */
int ChecaLigacao(tipoLista vetor[], int clique[], int vertice, int size) {
	int i=0, counter = 0;
	apontador aux;
	aux = vetor[vertice-1].primeiro->prox;	//aux = lista de adjacencia do vertice passado como parametro. (vetor[vertice-1])
	while(aux != NULL) {	//percorre a lista de adjacencia do vertice
		for(i=0;i<size;i++) {		//para cada adjacente encontrado, compara a todos os presentes no clique. Se forem iguais, incrementa "counter".
			if(aux->amizades.Vertice == clique[i]) {
				counter++;
			}
		}
		i=0;
		aux = aux->prox;
	}
	return counter;
}

/* A função cliquefinder é a responsável por encontrar os cliques maximais do grafo. Aqui implementamos nosso algoritmo.
Método: Através de um vetor de listas(listas de adjacências), para cada vértice é realizada uma checagem quanto aos
adjacentes desse vértice, e, para cada adjacente, é checado quantos vertices em comum eles (o adjacente e o vertice primario)
possuem em comum. Ao encontrar esses vertices em comum, perguntamos, individualmente, se cada vertice encontrado conecta-se a 
todos os outros que ja estao salvos no vetor clique. Se sim, também é adicionado ao clique. Assim, todos os cliques possiveis 
são encontrados. Ao encerrarmos um clique (chegarmos a um vertice que nao se encaixa), vemos o tamanho desse clique, e, caso seja
o maior ate entao, atualizamos o vetor "vetormaiorclique", que armazena o maior clique encontrado.
Apos o fim da iteração principal, teremos armazenado em "vetormaiorclique" o maior clique, e, em "dadosmaiorclique" as informacoes
desses vertices que compõem o maior clique.
Problema da implementação: cliques repetidos. */


void cliquefinder(tipoLista vetor[]) {	//vetor1 e vetor2 sao conjuntos auxiliares que armazenam a lista de adjacencia do vertice primario e seu adjacente.
	int clique[39], vetor1[39], vetor2[39], i=0,j=0,k=0,l=0,a=0,b=0,m=0, counter = 0,maiorclique = 0;
	apontador aux, aux2, aux3,aux4;	//apontadores para percorrer as listas de adjacencia
	int vetormaiorclique[10];	//vetor que armazenara o maior clique
	tipoAluno dadosmaiorclique[10];	//vetor de structs que armazenará os dados dos vértices que compuserem o maior clique.
	for(i=0;i<10;i++) {			//alocando memoria para "nome", visto que foi declarado como *nome na struct tipoAluno.
		dadosmaiorclique[i].nome = malloc(40*sizeof(char));
	}
	for(i=0;i<39;i++) {	//for() controla o vertice principal, so muda de vertice ao realizar todas possibilidades.
		aux = vetor[i].primeiro->prox;
		aux3 = aux;
		if(aux != NULL) {
			while(aux != NULL) {		//salva em vetor1 a lista de adjacencia do vertice primario.
				vetor1[k] = aux->amizades.Vertice;
				k++;
				aux = aux->prox;
			}
			while(aux3 != NULL) {
				clique[j] = vetor[i].primeiro->amizades.Vertice;  //adiciona o vertice primario ao clique
				j++;	//incrementa index do clique
				aux2 = vetor[aux3->amizades.Vertice-1].primeiro;
				clique[j] = aux2->amizades.Vertice;		//acrescenta primeiro adjacente ao clique
				j++; //incrementa index do clique
				aux2 = aux2->prox;	//acessa lista de adjacencia do primeiro adjacente
				while(aux2 != NULL) {
					l++;				//conta a quantidade de amigos o primeiro adjacente possui
					aux2 = aux2->prox;
				}
				PreencheVetor2(vetor,vetor2, aux3->amizades.Vertice); //chama funcao para armazenar em vetor2 a lista de adjacencia do primeiro adjacente
				for(a=0;a<k;a++) {	//loop para realizar comparacao entre vetor1 e vetor2 (lista de adjacencias do vertice primario e do primeiro adjacente)
					for(b=0;b<l;b++) {
						if(vetor1[a] == vetor2[b]) {   //checa se existe vertice em comum
							aux4 = vetor[(vetor1[a])-1].primeiro;
							counter = ChecaLigacao(vetor,clique, aux4->amizades.Vertice, j);  //checa se o vertice em comum possui aresta ligando a todos os vertices que estao no clique
							if(counter == j) {	//caso o vertice possua aresta a todos os membros do clique, adiciona ele no clique
								clique[j] = vetor1[a];
								j++;	//incrementa index do clique
							}
						}
					}
				}
				if(j >= maiorclique) {	//checa se o tamanho do clique encontrado e superior ao maior clique ja encontrado
					maiorclique = j;
					for(m=0;m<j;m++) {	//salva o maior clique e informacoes dos vertices que o compoem
						vetormaiorclique[m] = clique[m];
						strcpy(dadosmaiorclique[m].nome,vetor[clique[m]-1].primeiro->amizades.nome);
						dadosmaiorclique[m].matricula = vetor[clique[m]-1].primeiro->amizades.matricula;
					}				
				}
				j = 0;
				k = 0;
				l = 0;
				aux3 = aux3->prox; //passa para o proximo adjacente do vertice primario.
			}	
		}
	}
	//printa maior clique
	printf("\nTAMANHO DO MAIOR CLIQUE: %d\n", maiorclique);
	for(m=0;m<maiorclique;m++) {
		printf("%d - %s - %d\n", vetormaiorclique[m], dadosmaiorclique[m].nome, dadosmaiorclique[m].matricula);
	}

}

int main (int argc, char *argv[]) {
	FILE *fp;
	int i, counter=0, matricula, links[15], j=0;
	char line[100], *token;
	tipoAluno dados;
	tipoLista lista[39];
	fp = fopen("amigos_tag20171.txt", "r");

	while (!feof(fp)) {
		if(counter >= 39) {
			break;
		}
		fgets(line,80,fp);	/*captura a linha inteira*/
		token = strtok(line, "|");	/*utiliza strtok para ler primeira parte (matricula)*/
		matricula = atoi(token);	/*atraves do atoi armazenamos a matricula*/
		dados.matricula = matricula;	/*salvamos matricula na struct.*/
		token = strtok(NULL, "|");		/*utiliza strtok para ler segunda parte (nome)*/
		dados.nome = malloc(40*sizeof(char));	/*aloca espaço para char (declarado apenas como char* na struct)*/
		strcpy(dados.nome,token); /*atraves do strcpy armazenamos o nome na struct*/
		token = strtok(NULL, " ");	
		while(token != NULL) {		/*loop para capturarmos toda a lista de amigos, através do strktok e atoi.*/
			if(atoi(token) != -1) {
				links[j] = atoi(token);
				j++;
			}
			token = strtok(NULL, " ");
		}
		dados.Vertice = counter+1;	/*salva na struct o vertice do elemento.*/
		CriaLista(&lista[counter], dados);	/*Cria lista, passa como parametro o vertice-1 e a struct com informações sobre o vértice.*/ 
		for(i=0;i<j;i++) {	/*Inserimos na lista desse vértice os amigos que foram lidos do arquivo.*/
			InsereLista(links[i], &lista[counter]);
		}
		j=0;	
		counter++; /*conta se já foi lido todas as linhas do arquivo.	*/
	}
	Undirected(lista); /*Chama função Undirected para tornar o grafo não-direcionado.*/
	Ordena(lista);	/*Chama função Ordena, que lida com o ordenamento dos graus e a impressão apropriada.*/ 
	cliquefinder(lista);	/*Chama função cliquefinder para obter o maior clique maximal desse grafo. */
	fclose(fp);	/*fecha arquivo.*/
	return 0;
}