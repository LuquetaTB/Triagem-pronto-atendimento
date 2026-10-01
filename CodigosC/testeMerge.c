#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define MAX 100000

void merge(int eventos_final[], char fila_relatorio[][13], int inicio, int meio, int fim){
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    int *esquerda = malloc(n1 * sizeof(int));
    int *direita = malloc(n2 * sizeof(int));

    char esquerda_cpf[n1][13];
    char direita_cpf[n2][13];

    for(i=0; i<n1; i++){
        esquerda[i] = eventos_final[inicio + i];
        strcpy(esquerda_cpf[i], fila_relatorio[inicio + i]);
    }
    for ( j = 0; j <n2 ; j++){
        direita[j] = eventos_final[meio + 1 + j];
        strcpy(direita_cpf[j], fila_relatorio[meio +1+ j]);
    }
    
    i = 0;
    j = 0;
    k = inicio;
    while(i<n1 && j<n2){
        if(esquerda[i]>= direita[j]){
            eventos_final[k] = esquerda[i];
            strcpy(fila_relatorio[k], esquerda_cpf[i]);

            i++;
            k++;
        }
        else{
            eventos_final[k] = direita[j];
            strcpy(fila_relatorio[k], direita_cpf[j]);

            j++;
            k++;
        }
    }

    while(i<n1){
        eventos_final[k] = esquerda[i];
        strcpy(fila_relatorio[k], esquerda_cpf[i]);

        i++;
        k++;
    }
    while (j<n2){
        eventos_final[k] = direita[j];
        strcpy(fila_relatorio[k], direita_cpf[j]);
        
        j++;
        k++;
    }

    free(esquerda);
    free(direita);
    
}
void mergesort(int eventos_final[], char fila_relatorio[][13],int inicio, int fim){
    int meio;
    if(inicio<fim){
        meio =inicio + (fim - inicio)/2;
        mergesort(eventos_final, fila_relatorio, inicio,  meio);
        mergesort(eventos_final, fila_relatorio, meio + 1, fim);
        merge(eventos_final, fila_relatorio, inicio, meio, fim);
    }
}

int main(){
    static char fila_relatorio[MAX][13];
    static int eventos_final[MAX];
    int quantidade = 0;

    clock_t x, y;
    double tempo;

    FILE *arquivo = fopen("relatorio_100000.csv", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[200];

    // pula o cabeçalho
    fgets(linha, 200, arquivo);

    for (int i = 0; i < MAX; i++) {
    fgets(linha, 200, arquivo);

    char *cpf = strtok(linha, ",");
    char *eventos = strtok(NULL, "\n");

    strcpy(fila_relatorio[i], cpf);
    eventos_final[i] = atoi(eventos);
    quantidade++;
}
    printf("%d\n", quantidade);

    x = clock();
    mergesort(eventos_final, fila_relatorio, 0, quantidade -1);
    y = clock();

    tempo = (double)(y-x)/CLOCKS_PER_SEC;
    printf("tempo: %.6f segundos\n", tempo);

    return 0;
}