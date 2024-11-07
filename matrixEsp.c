#include <stdio.h>
#include <stdlib.h>

typedef struct TipoElemento TipoElemento;
typedef struct TipoLinha TipoLinha;
typedef struct MatEsparsa MatEsparsa;

struct TipoElemento {
    int valor;
    int coluna;
    TipoElemento *prox;
};

struct TipoLinha {
    int ID_Coluna;
    TipoLinha *prox;
    TipoElemento *inicio;
};

struct MatEsparsa {
    int qtd_linhas;
    int qtd_colunas;
    TipoLinha *inicio;
};

TipoElemento* criar_elemento(int valor, int coluna) {
    TipoElemento* novo = (TipoElemento*) malloc(sizeof(TipoElemento));
    novo->valor = valor;
    novo->coluna = coluna;
    novo->prox = NULL;
    return novo;
}

TipoLinha* criar_linha(int ID_Coluna) {
    TipoLinha* nova = (TipoLinha*) malloc(sizeof(TipoLinha));
    nova->ID_Coluna = ID_Coluna;
    nova->prox = NULL;
    nova->inicio = NULL;
    return nova;
}

MatEsparsa* criar_matriz(int linhas, int colunas) {
    MatEsparsa* mat = (MatEsparsa*) malloc(sizeof(MatEsparsa));
    mat->qtd_linhas = linhas;
    mat->qtd_colunas = colunas;
    mat->inicio = NULL;
    return mat;
}

int buscar_pos(int l, int c, MatEsparsa* mat) {
    TipoLinha* linha_atual = mat->inicio;

    while (linha_atual != NULL && linha_atual->ID_Coluna < l) {
        linha_atual = linha_atual->prox;
    }

    if (linha_atual == NULL || linha_atual->ID_Coluna != l) {
        return 0;
    }

    TipoElemento* elem_atual = linha_atual->inicio;
    while (elem_atual != NULL && elem_atual->coluna < c) {
        elem_atual = elem_atual->prox;
    }

    if (elem_atual == NULL || elem_atual->coluna != c) {
        return 0;
    }

    return elem_atual->valor;
}

void inserir_elemento(MatEsparsa* mat, int linha, int coluna, int valor) {
    TipoLinha *atual = mat->inicio, *anterior = NULL;
    while (atual != NULL && atual->ID_Coluna < linha) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL || atual->ID_Coluna != linha) {
        TipoLinha* nova_linha = criar_linha(linha);
        if (anterior == NULL) {
            mat->inicio = nova_linha;
        } else {
            anterior->prox = nova_linha;
        }
        nova_linha->prox = atual;
        atual = nova_linha;
    }

    TipoElemento *elem_atual = atual->inicio, *elem_ant = NULL;
    while (elem_atual != NULL && elem_atual->coluna < coluna) {
        elem_ant = elem_atual;
        elem_atual = elem_atual->prox;
    }

    if (elem_atual == NULL || elem_atual->coluna != coluna) {
        TipoElemento* novo_elem = criar_elemento(valor, coluna);
        if (elem_ant == NULL) {
            atual->inicio = novo_elem;
        } else {
            elem_ant->prox = novo_elem;
        }
        novo_elem->prox = elem_atual;
    } else {
        elem_atual->valor = valor;
    }
}

void apaga_matriz(MatEsparsa* mat) {
    TipoLinha* linha_atual = mat->inicio;
    while (linha_atual != NULL) {
        TipoElemento* elem_atual = linha_atual->inicio;
        while (elem_atual != NULL) {
            TipoElemento* temp = elem_atual;
            elem_atual = elem_atual->prox;
            free(temp);
        }
        TipoLinha* temp_linha = linha_atual;
        linha_atual = linha_atual->prox;
        free(temp_linha);
    }
    free(mat);
}

MatEsparsa* cria_transposta(MatEsparsa* mat) {
    MatEsparsa* transposta = criar_matriz(mat->qtd_colunas, mat->qtd_linhas);
    TipoLinha* linha_atual = mat->inicio;
    while (linha_atual != NULL) {
        TipoElemento* elem_atual = linha_atual->inicio;
        while (elem_atual != NULL) {
            inserir_elemento(transposta, elem_atual->coluna, linha_atual->ID_Coluna, elem_atual->valor);
            elem_atual = elem_atual->prox;
        }
        linha_atual = linha_atual->prox;
    }
    return transposta;
}

MatEsparsa* soma_matrizes(MatEsparsa* A, MatEsparsa* B) {
    if (A->qtd_linhas != B->qtd_linhas || A->qtd_colunas != B->qtd_colunas) {
        return NULL;
    }
    MatEsparsa* C = criar_matriz(A->qtd_linhas, A->qtd_colunas);

    TipoLinha* linhaA = A->inicio;
    while (linhaA != NULL) {
        TipoElemento* elemA = linhaA->inicio;
        while (elemA != NULL) {
            inserir_elemento(C, linhaA->ID_Coluna, elemA->coluna, elemA->valor);
            elemA = elemA->prox;
        }
        linhaA = linhaA->prox;
    }

    TipoLinha* linhaB = B->inicio;
    while (linhaB != NULL) {
        TipoElemento* elemB = linhaB->inicio;
        while (elemB != NULL) {
            int valor_atual = buscar_pos(linhaB->ID_Coluna, elemB->coluna, C);
            inserir_elemento(C, linhaB->ID_Coluna, elemB->coluna, valor_atual + elemB->valor);
            elemB = elemB->prox;
        }
        linhaB = linhaB->prox;
    }
    return C;
}

MatEsparsa* multiplica_matrizes(MatEsparsa* A, MatEsparsa* B) {
    if (A->qtd_colunas != B->qtd_linhas) {
        return NULL;
    }
    MatEsparsa* C = criar_matriz(A->qtd_linhas, B->qtd_colunas);

    TipoLinha* linhaA = A->inicio;
    while (linhaA != NULL) {
        TipoElemento* elemA = linhaA->inicio;
        while (elemA != NULL) {
            TipoLinha* linhaB = B->inicio;
            while (linhaB != NULL && linhaB->ID_Coluna != elemA->coluna) {
                linhaB = linhaB->prox;
            }
            if (linhaB != NULL) {
                TipoElemento* elemB = linhaB->inicio;
                while (elemB != NULL) {
                    int valor_atual = buscar_pos(linhaA->ID_Coluna, elemB->coluna, C);
                    inserir_elemento(C, linhaA->ID_Coluna, elemB->coluna, valor_atual + elemA->valor * elemB->valor);
                    elemB = elemB->prox;
                }
            }
            elemA = elemA->prox;
        }
        linhaA = linhaA->prox;
    }
    return C;
}

void imprimir_matriz(MatEsparsa* mat) {
    TipoLinha* linha_atual = mat->inicio;
    for (int i = 0; i < mat->qtd_linhas; i++) {
        TipoElemento* elem_atual = NULL;
        if (linha_atual != NULL && linha_atual->ID_Coluna == i) {
            elem_atual = linha_atual->inicio;
            linha_atual = linha_atual->prox;
        }
        for (int j = 0; j < mat->qtd_colunas; j++) {
            if (elem_atual != NULL && elem_atual->coluna == j) {
                printf("%d ", elem_atual->valor);
                elem_atual = elem_atual->prox;
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

int main() {
    MatEsparsa* A = criar_matriz(3, 3); 
    MatEsparsa* B = criar_matriz(3, 3); 

    inserir_elemento(A, 0, 0, 1); 
    inserir_elemento(A, 1, 1, 2); 
    inserir_elemento(A, 2, 2, 3); 

    inserir_elemento(B, 0, 1, 4); 
    inserir_elemento(B, 1, 2, 5); 
    inserir_elemento(B, 2, 0, 6);

    printf("Matriz A:\n");
    imprimir_matriz(A);

    printf("Matriz B:\n");
    imprimir_matriz(B);

    MatEsparsa* soma = soma_matrizes(A, B);
    printf("Soma de A e B:\n");
    imprimir_matriz(soma);

    MatEsparsa* multiplicacao = multiplica_matrizes(A, B);
    printf("Multiplicacao de A e B:\n");
    imprimir_matriz(multiplicacao);

    MatEsparsa* transposta = cria_transposta(A);
    printf("Transposta de A:\n");
    imprimir_matriz(transposta);

    MatEsparsa* transpostaB = cria_transposta(B);
    printf("Transposta de B:\n");
    imprimir_matriz(transpostaB);

    apaga_matriz(A);
    apaga_matriz(B);
    apaga_matriz(soma);
    apaga_matriz(multiplicacao);
    apaga_matriz(transposta);
    apaga_matriz(transpostaB);

    return 0;
}
