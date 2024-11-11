#include <stdio.h>
#include <stdlib.h>
#include"matriz_esparsa.h"

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

