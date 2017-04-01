#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void PreencheVetor2(tipoLista lista[], int vetor2[], int vertice) {
	int i=0;
	apontador aux;
	aux = lista[vertice-1].primeiro->prox;
	while(aux!=NULL) {
		vetor2[i] = aux->amizades.Vertice;
		aux = aux->prox;
		i++;
	}
}

int ChecaLigacao(tipoLista vetor[], int clique[], int vertice, int size) {
	int i=0, counter = 0;
	apontador aux;
	aux = vetor[vertice-1].primeiro->prox;
	while(aux != NULL) {
		for(i=0;i<size;i++) {
			if(aux->amizades.Vertice == clique[i]) {
				counter++;
			}
		}
		i=0;
		aux = aux->prox;
	}
	return counter;
}

void cliquefinder(tipoLista vetor[]) {
	int clique[39], vetor1[39], vetor2[39], i=0,j=0,k=0,l=0,a=0,b=0,m=0, counter = 0;
	apontador aux, aux2, aux3,aux4;
	for(i=0;i<39;i++) {
		aux = vetor[i].primeiro->prox;
		aux3 = aux;
		if(aux != NULL) {
			while(aux != NULL) {
				vetor1[k] = aux->amizades.Vertice;
				k++;
				aux = aux->prox;
			}
			while(aux3 != NULL) {
				clique[j] = vetor[i].primeiro->amizades.Vertice;
				j++;
				aux2 = vetor[aux3->amizades.Vertice-1].primeiro;
				clique[j] = aux2->amizades.Vertice;		
				j++;
				aux2 = aux2->prox;	
				while(aux2 != NULL) {
					l++;
					aux2 = aux2->prox;
				}
				PreencheVetor2(vetor,vetor2, aux3->amizades.Vertice);
				for(a=0;a<k;a++) {
					for(b=0;b<l;b++) {
						if(vetor1[a] == vetor2[b]) {
							aux4 = vetor[(vetor1[a])-1].primeiro;
							counter = ChecaLigacao(vetor,clique, aux4->amizades.Vertice, j);
							if(counter == j) {
								clique[j] = vetor1[a];
								j++;
							}
						}
					}
				}
				if(j >= 6) {
					printf("\nCLIQUE SIZE: %d: ", j);
					for(m=0;m<j;m++) {
						printf("%s - %d \n", vetor[clique[m]-1].primeiro->amizades.nome, vetor[clique[m]-1].primeiro->amizades.matricula);
					}
					printf("\n");
				}
				j = 0;
				k = 0;
				l = 0;

				aux3 = aux3->prox;
			}	
		}
	}
}

int main (int argc, char *argv[]) {
	FILE *fp;
	int i, counter=0, matricula, links[15], j=0;
	char line[100], *token;
	tipoAluno dados;
	tipoLista lista[39];
	fp = fopen("amigos.txt", "r");

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
	cliquefinder(lista);
	fclose(fp);	/*fecha arquivo.*/
	return 0;
}