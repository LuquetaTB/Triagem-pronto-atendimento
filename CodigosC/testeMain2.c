#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define MAX 100000


void cadastrar(char cpfs[][13], char cpf[], char nomes[][100], char nome[], char 
nascimentos[][12], char nascimento[], int *qnt_cadastros, int *eventos){
    (*eventos)++;
    /*Pega as informações do paciente e coloca em diferentes vetores com o mesmo indice, 
    para facilitar na hora da busca dessas informações.*/
    strcpy(cpfs[*qnt_cadastros], cpf);
    strcpy(nomes[*qnt_cadastros], nome);
    strcpy(nascimentos[*qnt_cadastros], nascimento);

    (*qnt_cadastros)++;
}

int buscar_cadastro(char cpfs[][13], char cpf_busca[],int *qnt_cadastros, int *eventos){
    (*eventos)++;   
    /*Procura pelo cpf no cadastro geral, e, caso encontre gera o indice aonde foi
    encontrado aquele cpf.*/ 
    for(int i=0; i<*qnt_cadastros; i++){
        if(strcmp(cpfs[i], cpf_busca) == 0){
            return i;
        }
    }
    //caso nao encontre, retorna "-1".
    return -1;
}

int dar_entrada(char cpfs[][13], char fila_geral[][13],
char cpf_busca[],int riscos[],int eventos_entrada[], int risco,int *qnt_cadastros, int *quantidade, int *eventos){
    (*eventos)++;
    int encontrou = 0;
    /*For vai buscar pelo cpf informado se ele esta cadastrado na, caso nao esteja a
    função retorna 0.*/
    for(int i = 0; i < *qnt_cadastros; i++){

        if(strcmp(cpfs[i], cpf_busca) == 0){
        encontrou = 1;
        break;
        }
    }

    if(!encontrou){
        return 0;
    }
    /*Esse for vai buscar o cpf informado na fila geral, para ver se o paciente ja nao se
    encontra nela.*/
    for(int i = 0; i<*quantidade; i++){
        if(strcmp(fila_geral[i], cpf_busca) == 0){
            return 1;
        }
    }
    /*Caso nenhum dos returns anteriores seja chamado, o paciente sera colocado na fila
    com seu risco e a quantidade de eventos atual,tudo com o mesmo indice para facilitar a
    procura*/
    strcpy(fila_geral[*quantidade], cpf_busca);
    riscos[*quantidade] = risco;
    eventos_entrada[*quantidade] = *eventos;
    (*quantidade)++;
    return 2;

}

int chamar_proximo(int riscos[], char cpf[], char fila_geral[][13],char fila_relatorio[][13], int eventos_final[], 
    int eventos_entrada[], int *quantidade, int *eventos, int *qnt_relatorios){
    (*eventos)++;
    int teste = 6;
    int indice = -1;
    /*O for procura pelo menor risco e guarda a sua primeira ocorrencia na variavel indice*/
    for(int i=0; i<*quantidade; i++){
        if(riscos[i]<teste){
            teste = riscos[i];
            indice = i;
        }
    }

    if(indice == -1){
        return -1;
    }
    /*Guarda o paciente chamado na lista de pacientes ja atendidos/relatorio, junto com 
    a quantidade de eventos que gastou para ser atendido.*/
    strcpy(fila_relatorio[*qnt_relatorios], fila_geral[indice]);
    eventos_final[*qnt_relatorios] = *eventos - eventos_entrada[indice];
    (*qnt_relatorios)++;

    strcpy(cpf, fila_geral[indice]);
    /*Todos que estavam na fila dps do paciente chamado sao recolocados com um indice a 
    menos para tapar o buraco deixado por ele.*/
    for(int i=indice; i<*quantidade - 1; i++){
        riscos[i]= riscos[i+1];
        strcpy(fila_geral[i], fila_geral[i+1]);
        eventos_entrada[i] = eventos_entrada[i+1];
    }

    (*quantidade)--;
    return 0;
}

int desistir(char cpf_busca[], char fila_geral[][13], int riscos[], int eventos_entrada[], int *quantidade, int *eventos){
    (*eventos)++;
    /*Ve se o paciente desistente realmente esta na fila.*/
    int indice = -1;
    for(int i =0; i<*quantidade; i++){
        if(strcmp(fila_geral[i], cpf_busca)==0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        return -1;
    }
    /*Mesma coisa do for de chamar_paciente. ele muda todos para um indice a menos*/
    for(int i=indice;i<*quantidade-1; i++ ){
        riscos[i]= riscos[i+1];
        strcpy(fila_geral[i], fila_geral[i+1]);
        eventos_entrada[i] =eventos_entrada[i+1];
    }

    (*quantidade)--;
    return 0;
}

int tamanho_fila(int *quantidade,int *eventos){
    (*eventos)++;
    /*A variavel quantidade é incrementada ou decrementada toda vez que eu coloco
    um paciente na fila, chamo ou ele desiste. Entao, é so retornar a variavel
    ja atualizada*/
    return *quantidade;
}
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
    char cpf[13];
    static char cpfs[MAX][13];
    char cpf_busca[13];
    char nome[100];
    static char nomes[MAX][100];
    char nascimento[12];
    static char nascimentos[MAX][12];
    static char fila_geral[MAX][13];
    static char fila_relatorio[MAX][13];
    static int riscos[MAX];
    static int eventos_final[MAX];
    static int eventos_entrada[MAX];
    int buscado, quantidade = 0, risco, eventos =0, qnt_relatorios =0,qnt_cadastros =0, opcao;

    clock_t x, y;
    double tempo;

    FILE *arquivo = fopen("pacientes_100000.csv", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[200];

    // pula o cabeçalho
    fgets(linha, 200, arquivo);

    while (fgets(linha, 200, arquivo) != NULL) {

        char *cpf;
        char *nome;
        char *nascimento;

        cpf = strtok(linha, ",");
        nome = strtok(NULL, ",");
        nascimento = strtok(NULL, "\n");

        cadastrar(cpfs, cpf, nomes, nome, nascimentos, nascimento,
          &qnt_cadastros, &eventos);
    }

    fclose(arquivo);

    x = clock();

    for (int i = 0; i < qnt_cadastros; i++) {
        buscado= buscar_cadastro(cpfs, cpfs[i], &qnt_cadastros, &eventos);
    }

    y = clock();

    tempo = (double)(y-x)/CLOCKS_PER_SEC;
    printf("Tempo em %f segundos\n", tempo);

    for(int i =0; i<qnt_cadastros; i++){
        risco = (rand() % 5) + 1;

        dar_entrada(cpfs, fila_geral, cpfs[i], riscos, eventos_entrada, risco, &qnt_cadastros, 
        &quantidade, &eventos);
    }

    x = clock();

    for(int i =0; i<qnt_cadastros; i++){
        buscado= chamar_proximo(riscos, cpf, fila_geral, fila_relatorio, eventos_final, eventos_entrada, &quantidade
        , &eventos, &qnt_relatorios);
    }

    y = clock();

    tempo = (double)(y-x)/CLOCKS_PER_SEC;
    printf("Tempo em %f segundos\n", tempo);

    
    return 0;

}