#include <stdio.h>
#include <stdlib.h>

typedef struct tipo_elemento tipo_elemento;
typedef struct tipo_linha tipo_linha;
typedef struct Tipo_Mat_Esparsa Tipo_Mat_Esparsa;

struct tipo_elemento {
    int valor;
    int linha;
    tipo_elemento *proximo;
};

struct tipo_linha {
    int ID_Coluna;
    tipo_linha *proximo;
    tipo_elemento *inicio;
};

struct Tipo_Mat_Esparsa {
    int qtd_linhas;
    int qtd_colunas;
    tipo_linha *inicio;
};

tipo_elemento* criar_elemento(int valor, int linha) {
    tipo_elemento* novo = (tipo_elemento*) malloc(sizeof(tipo_elemento));
    novo->valor = valor;
    novo->linha = linha;
    novo->proximo = NULL;
    return novo;
}

tipo_linha* criar_linha(int ID_Coluna) {
    tipo_linha* nova = (tipo_linha*) malloc(sizeof(tipo_linha));
    nova->ID_Coluna = ID_Coluna;
    nova->proximo = NULL;
    nova->inicio = NULL;
    return nova;
}

Tipo_Mat_Esparsa* criar_matriz(int linhas, int colunas) {
    Tipo_Mat_Esparsa* mat = (Tipo_Mat_Esparsa*) malloc(sizeof(Tipo_Mat_Esparsa));
    mat->qtd_linhas = linhas;
    mat->qtd_colunas = colunas;
    mat->inicio = NULL;
    return mat;
}

int buscar_pos(int l, int c, Tipo_Mat_Esparsa* mat) {
    tipo_linha* linha_atual = mat->inicio;

    while (linha_atual != NULL && linha_atual->ID_Coluna < c) {
        linha_atual = linha_atual->proximo;
    }

    if (linha_atual == NULL || linha_atual->ID_Coluna != c) {
        return 0;
    }

    tipo_elemento* elem_atual = linha_atual->inicio;
    while (elem_atual != NULL && elem_atual->linha < l) {
        elem_atual = elem_atual->proximo;
    }

    if (elem_atual == NULL || elem_atual->linha != l) {
        return 0;
    }

    return elem_atual->valor;
}

void inserir_elemento(Tipo_Mat_Esparsa* mat, int linha, int coluna, int valor) {
    
    tipo_linha *atual = mat->inicio, *anterior = NULL;
    while (atual != NULL && atual->ID_Coluna < coluna) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL || atual->ID_Coluna != coluna) {
        tipo_linha* nova_linha = criar_linha(coluna);
        if (anterior == NULL) {
            mat->inicio = nova_linha;
        } else {
            anterior->proximo = nova_linha;
        }
        nova_linha->proximo = atual;
        atual = nova_linha;
    }

    tipo_elemento *elem_atual = atual->inicio, *elem_ant = NULL;
    while (elem_atual != NULL && elem_atual->linha < linha) {
        elem_ant = elem_atual;
        elem_atual = elem_atual->proximo;
    }

    if (elem_atual == NULL || elem_atual->linha != linha) {
        tipo_elemento* novo_elem = criar_elemento(valor, linha);
        if (elem_ant == NULL) {
            atual->inicio = novo_elem;
        } else {
            elem_ant->proximo = novo_elem;
        }
        novo_elem->proximo = elem_atual;
    } else {
        elem_atual->valor = valor;
    }
}

void apaga_matriz(Tipo_Mat_Esparsa* mat) {
    tipo_linha* linha_atual = mat->inicio;
    while (linha_atual != NULL) {
        tipo_elemento* elem_atual = linha_atual->inicio;
        while (elem_atual != NULL) {
            tipo_elemento* temp = elem_atual;
            elem_atual = elem_atual->proximo;
            free(temp);
        }
        tipo_linha* temp_linha = linha_atual;
        linha_atual = linha_atual->proximo;
        free(temp_linha);
    }
    free(mat);
}

Tipo_Mat_Esparsa* cria_transposta(Tipo_Mat_Esparsa* mat) {

    Tipo_Mat_Esparsa* transposta = criar_matriz(mat->qtd_colunas, mat->qtd_linhas);
    tipo_linha* linha_atual = mat->inicio;
    while (linha_atual != NULL) {
        tipo_elemento* elem_atual = linha_atual->inicio;
        while (elem_atual!= NULL) {
            inserir_elemento(transposta, linha_atual->ID_Coluna, elem_atual->linha,elem_atual->valor);
            elem_atual = elem_atual->proximo;
        }
        linha_atual = linha_atual->proximo;
        
    }
    return transposta;
}

Tipo_Mat_Esparsa* soma_matrizes(Tipo_Mat_Esparsa* A, Tipo_Mat_Esparsa* B) {

    if (A->qtd_linhas != B->qtd_linhas || A->qtd_colunas != B->qtd_colunas) {
        return NULL;
    }

    Tipo_Mat_Esparsa* C = criar_matriz(A->qtd_linhas, A->qtd_colunas);

    for(int i = 0; i < A->qtd_linhas; i++)
    {
        for(int j = 0; j < A->qtd_colunas ; j++)
        {
            int item1 = buscar_pos(i, j, A);
            int item2 = buscar_pos(i, j, B);
            int soma = item1 + item2;

            if (soma != 0)
            {
                inserir_elemento(C, i, j, soma);
            }
        }
    }
    return C;
}

Tipo_Mat_Esparsa* multiplica_matrizes(Tipo_Mat_Esparsa* A, Tipo_Mat_Esparsa* B) {
    if (A->qtd_colunas != B->qtd_linhas) {
        return NULL;
    }

    Tipo_Mat_Esparsa* C = criar_matriz(A->qtd_linhas, B->qtd_colunas);

    tipo_linha* linhaA = A->inicio;
    while (linhaA != NULL) {
        tipo_elemento* elemA = linhaA->inicio;
        while (elemA != NULL) {
            tipo_linha* linhaB = B->inicio;
            while (linhaB != NULL && linhaB->ID_Coluna != elemA->linha) {
                linhaB = linhaB->proximo;
            }
            if (linhaB != NULL) {
                tipo_elemento* elemB = linhaB->inicio;
                while (elemB != NULL) {
                    int valor_atual = buscar_pos(linhaA->ID_Coluna, elemB->linha, C);
                    inserir_elemento(C, linhaA->ID_Coluna, elemB->linha, valor_atual + elemA->valor * elemB->valor);
                    elemB = elemB->proximo;
                }
            }
            elemA = elemA->proximo;
        }
        linhaA = linhaA->proximo;
    }
    return C;
}

void imprimir_matriz(Tipo_Mat_Esparsa* mat) {
    tipo_linha* linha_atual = mat->inicio;
    for (int i = 0; i < mat->qtd_linhas; i++) {
        tipo_elemento* elem_atual = NULL;
        if (linha_atual != NULL && linha_atual->ID_Coluna == i) {
            elem_atual = linha_atual->inicio;
            linha_atual = linha_atual->proximo;
        }
        for (int j = 0; j < mat->qtd_colunas; j++) {
            if (elem_atual != NULL && elem_atual->linha == j) {
                printf("%d ", elem_atual->valor);
                elem_atual = elem_atual->proximo;
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

int main() {
    Tipo_Mat_Esparsa* A = criar_matriz(3, 3); 

    inserir_elemento(A, 0, 2, 1); 
    inserir_elemento(A, 1, 1, 2); 
    inserir_elemento(A, 2, 1, 3); 

    printf("Matriz A:\n");
    imprimir_matriz(A);

    Tipo_Mat_Esparsa* transposta = cria_transposta(A);
    printf("Transposta de A:\n");
    imprimir_matriz(transposta);

   Tipo_Mat_Esparsa* soma = soma_matrizes(A, transposta);
    printf("Soma de A e B:\n");
    imprimir_matriz(soma);

    Tipo_Mat_Esparsa* multiplicacao = multiplica_matrizes(A, transposta);
    printf("Multiplicacao de A e B:\n");
    imprimir_matriz(multiplicacao);

    apaga_matriz(A);
    apaga_matriz(soma);
    apaga_matriz(multiplicacao);
    apaga_matriz(transposta);
  

    return 0;
}

