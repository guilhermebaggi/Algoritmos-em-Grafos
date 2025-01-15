#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafosGuilhermeMBaggio.h"

int main() {
    grafo *g = criar_grafo();

    int op = 0;

    do {
        printf("\nEscolha uma das opcoes: \n");
        printf("1 - Inserir Vertice\n");
        printf("2 - Inserir Aresta\n");
        printf("3 - Visualizar grafo\n");
        printf("4 - Remover Vertice\n");
        printf("5 - Remover Aresta\n");
        printf("6 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);

        int n, i;
        int id, ant, prox;

        switch (op) {
            case 1:
                printf("\nQuantidade de Vertices que deseja inserir: ");
                scanf("%d", &n);

                for (i = 1; i <= n; i++) {
                    printf("vertice %d: ", i);
                    scanf("%d", &id);

                    inserirtVertice(id, g);
                }
                break;

            case 2:
                printf("\nQuantidade de arestas que deseja inserir: ");
                scanf("%d", &n);

                for (i = 1; i <= n; i++) {
                    printf("\nvertice anterior e proximo: ");
                    scanf("%d %d", &ant, &prox);

                    inserirtAresta(ant, prox, g);
                }
                break;

            case 3:
                visualizar_grafo(g);
                break;

            case 4:
                printf("\nId do vertice que deseja remover: ");
                scanf("%d", &id);

                remover_vertices(id, g);
                break;

            case 5:
                printf("\nId do vertice anterior e do proximo respectivamente: ");
                scanf("%d %d", &ant, &prox);

                remover_aresta(ant, prox, g);
                break;

            case 6:
                break;

            default:
                printf("Opcao Invalida!\n");
                break;
        }
    } while (op != 6);

    return 0;
}
