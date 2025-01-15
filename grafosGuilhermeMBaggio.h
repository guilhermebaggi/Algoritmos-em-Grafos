#include <stdio.h>
#include <stdlib.h>

typedef struct grafo grafo;
typedef struct vertice vertice;
typedef struct aresta aresta;

//contém um ponteiro para ohead da lista de vertice
struct grafo{
    vertice *head;
};

/*contém um id, um ponteiro para o proximo vértice na lsita lista e
um ponteiro para o head da lsita de arestas conectadas a esse vértice*/
struct vertice{
    aresta *a;
    vertice *next;
    int id;
};

//contém o id do vértice conectado e um ponteiro para a proximo aresta 
struct aresta{
    int id_prox; 
    aresta *prox;
};

//cria um grafo vazio 
grafo *criar_grafo(){
    grafo *g = (grafo *)malloc(sizeof(grafo));
    g->head = NULL;
    return g;
}

//verifica se o grafo esta vazio
int grafo_vazio(grafo *g){
    return (g == NULL) || (g->head == NULL);
}

int existeVert(int id, grafo *g){
    if (grafo_vazio(g)){
        return 0;
    }else{
        vertice *auxV = g->head;
        while (auxV != NULL){
            if (id == auxV->id)
                return 1;
            auxV = auxV->next;
        }
    }
    return 0;
}

int existirAresta(int vert1, int vert2, grafo *g){
    
    if (existeVert(vert1, g) && existeVert(vert2, g)){
        vertice *auxV = g->head;

        while (auxV->id != vert1)
            auxV = auxV->next;

        aresta *auxA = auxV->a;

        while (auxA != NULL){
            if (auxA->id_prox == vert2)
                return 1;
            auxA = auxA->prox;
        }
    }

    return 0;
}

void inserirtVertice(int key, grafo *g){

    if (grafo_vazio(g)){
        g->head = (vertice *)malloc(sizeof(vertice));
        g->head->id = key;
    }
    else{
        if (!existeVert(key, g)){
            vertice *aux = g->head;
            while (aux->next != NULL){
                aux = aux->next;
            }
            aux->next = (vertice *)malloc(sizeof(vertice));
            aux->next->id = key;
        }
        else
            printf("vertice já existe");
    }
}

void inserir_na_lista(int id, aresta **lista) {
    aresta *novaAresta = (aresta *)malloc(sizeof(aresta)); // Aloca memoria para a nova aresta
    novaAresta->id_prox = id; // Define o identificador do vertice de destino
    novaAresta->prox = NULL; // Inicializa o proximo como NULL

    // Caso a lista esteja vazia ou o novo elemento seja o menor
    if (*lista == NULL || (*lista)->id_prox > id) {
        novaAresta->prox = *lista; // Insere no início da lista
        *lista = novaAresta;
    } else {
        aresta *aux = *lista; // Ponteiro auxiliar para percorrer a lista
        while (aux->prox != NULL && aux->prox->id_prox < id) // Encontra a posicao correta
            aux = aux->prox;

        novaAresta->prox = aux->prox; // Insere o novo elemento na posicao correta
        aux->prox = novaAresta;
    }
}

void inserirtAresta(int vert1, int vert2, grafo *g){
    if (!grafo_vazio(g)) { // Verifica se o grafo nao esta vazio
        if (!existirAresta(vert1, vert2, g)) { // Verifica se a aresta ja nao existe
            vertice *vertice1 = g->head;
            vertice *vertice2 = g->head;

            // Localiza os vertices v1 e v2 na lista de vertices
            while (vertice1 != NULL && vertice1->id != vert1)
                vertice1 = vertice1->next;

            while (vertice2 != NULL && vertice2->id != vert2)
                vertice2 = vertice2->next;

            if (vertice1 != NULL && vertice2 != NULL) { // Se ambos os vertices existem
                // Insere v2 na lista de adjacencia de v1
                inserir_na_lista(vert2, &(vertice1->a));
                // Insere v1 na lista de adjacencia de v2
                inserir_na_lista(vert1, &(vertice2->a));
                printf("Aresta (%d, %d) inserida com sucesso.\n", vert1, vert2);
            } else {
                printf("Um ou ambos os vertices nao existem no grafo.\n");
            }
        } else {
            printf("Aresta (%d, %d) ja existe.", vert1, vert2);
        }
    } else {
        printf("Grafo vazio.\n");
    }
}

void visualizar_grafo(grafo *g){
    if (grafo_vazio(g)){
        printf("Grafo vazio");
    }else{
        vertice *auxV = g->head;
        while (auxV != NULL){
            printf("%d ->", auxV->id);
            aresta *auxA = auxV->a;

            while (auxA != NULL){
                printf(" %d ->", auxA->id_prox);
                auxA = auxA->prox;
            }
            printf("\n");
            auxV = auxV->next;
        }
    }
}


int remover_na_lista(int key, aresta **lista) {
    // Verifica se a lista de adjacencias esta vazia
    if (*lista == NULL)
        return 0; 

    aresta *aux = *lista;
    aresta *anterior = NULL; // Ponteiros auxiliares para percorrer a lista

    // Caso o elemento a ser removido esteja no inicio da lista
    if (aux->id_prox == key) {
        *lista = aux->prox; // Atualiza o inicio da lista para o proximo elemento
        free(aux); // Libera a memoria do elemento removido
        return 1;
    }

    // Percorre a lista ate encontrar o elemento ou o final da lista
    while (aux != NULL && aux->id_prox != key) {
        anterior = aux; // Atualiza o ponteiro para o elemento anterior
        aux = aux->prox; // Avanca para o proximo elemento
    }

    // Se encontrou o elemento na lista
    if (aux != NULL) {
        anterior->prox = aux->prox; // Remove o elemento ajustando os ponteiros
        free(aux); // Libera a memoria do elemento removido
        return 1;
    }

    // Se o elemento nao foi encontrado, retorna 0
    return 0;
}

void remover_aresta(int v1, int v2, grafo *g) {
    if (!grafo_vazio(g)) {
        // Verifica se a aresta entre v1 e v2 existe no grafo
        if (existirAresta(v1, v2, g)) {
            vertice *vertice1 = g->head; // Ponteiro para localizar o vertice de origem (v1)
            vertice *vertice2 = g->head; // Ponteiro para localizar o vertice de destino (v2)

            // Percorre a lista de vertices ate encontrar v1
            while (vertice1 != NULL && vertice1->id != v1)
                vertice1 = vertice1->next;

            // Percorre a lista de vertices ate encontrar v2
            while (vertice2 != NULL && vertice2->id != v2)
                vertice2 = vertice2->next;

            // Verifica se ambos os vertices foram encontrados
            if (vertice1 != NULL && vertice2 != NULL) {
                // Remove v2 da lista de adjacencias do vertice v1
                if (remover_na_lista(v2, &(vertice1->a))) {
                    // Remove v1 da lista de adjacencias do vertice v2
                    if (remover_na_lista(v1, &(vertice2->a)))
                        printf("Aresta (%d, %d) removida com sucesso.\n", v1, v2);
                }
            } else
                printf("Um ou ambos os vértices não existem no grafo.\n");
        } else
            printf("Aresta (%d, %d) não existe.\n", v1, v2);
    } else
        printf("Grafo vazio.\n");
}

void remover_vertices(int id, grafo *g) {
    if (!grafo_vazio(g)) {
        if (existeVert(id, g)) {
            vertice *vT = g->head;
            vertice *vA = NULL;

            // Localizar o vErtice a ser removido
            while (vT != NULL && vT->id != id) {
                vA = vT;
                vT = vT->next;
            }

            // Remover todas as arestas associadas ao vErtice
            aresta *aT = vT->a;
            while (aT != NULL) {
                aresta *aRm = aT;
                aT = aT->prox;
                // Remover a aresta correspondente no outro vertice
                remover_aresta(id, aRm->id_prox, g);
            }

            // Atualizar os ponteiros para remover o vertice
            if (vA == NULL)  // Caso seja o primeiro vertice
                g->head = vT->next;
            else
                vA->next = vT->next;

            free(vT); // Liberar a memoria do vertice
            printf("Vertice %d removido.\n", id);
        } else
            printf("Vertice %d não existe no grafo.\n", id);
    } else
        printf("O grafo está vazio.\n");
}


