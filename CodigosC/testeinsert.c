#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define MAX 100000
void relatorio_do_dia(char fila_relatorio[][13], int eventos_final[],int *qnt_relatorios){
    int aux;
    char temp[13];
    int j;
    for (int i = 1; i < *qnt_relatorios; i++) {

        aux = eventos_final[i];

        // guardar o CPF correspondente
        char temp[13];
        strcpy(temp, fila_relatorio[i]);

        j = i - 1;

        while (j >= 0 && eventos_final[j] < aux) {

            eventos_final[j + 1] = eventos_final[j];
            strcpy(fila_relatorio[j + 1], fila_relatorio[j]);

            j--;
        }

        eventos_final[j + 1] = aux;
        strcpy(fila_relatorio[j + 1], temp);
    }
}
int main(){
    static char fila_relatorio[MAX][13];
    static int eventos_final[MAX];
    int qnt_relatorios =0;

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
    qnt_relatorios++;
}
    printf("%d\n", qnt_relatorios);
    fclose(arquivo);

    x = clock();
    relatorio_do_dia(fila_relatorio, eventos_final, &qnt_relatorios);
    y = clock();

    tempo = (double)(y-x)/CLOCKS_PER_SEC;

    printf("tempo em sehundos: %.6f\n", tempo);

    return 0;
}