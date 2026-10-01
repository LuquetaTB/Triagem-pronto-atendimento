#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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
    int n1 = meio - inicio + 1;//Pega a primeira metade do vetor
    int n2 = fim - meio;// pega a segunda metade

    int *esquerda = malloc(n1 * sizeof(int));//reserva a quantidade de bytes que o vetor n1 vai precisar
    int *direita = malloc(n2 * sizeof(int));// msm coisa mas com n2

    char esquerda_cpf[n1][13];
    char direita_cpf[n2][13];

    for(i=0; i<n1; i++){
        //Aqui coloca os valores nos vetores "auxiliares"
        esquerda[i] = eventos_final[inicio + i];
        strcpy(esquerda_cpf[i], fila_relatorio[inicio + i]);
    }
    for ( j = 0; j <n2 ; j++){
        //Mesma coisa aqui mas com os valores do meio ate o fim
        direita[j] = eventos_final[meio + 1 + j];
        strcpy(direita_cpf[j], fila_relatorio[meio +1+ j]);
    }
    
    i = 0;
    j = 0;
    k = inicio;
    //Enquanto nao forem todos os numeros de um vetor o while continua rodando
    while(i<n1 && j<n2){
        if(esquerda[i]>= direita[j]){
            //Aqui finalmente ocorre a comparaçao e ordenaçao dos valores
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
    //Valores que ficaram nos vetores sao nseridos no vetor principal de forma ordenada
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

    //Libera a memoria 
    free(esquerda);
    free(direita);
}
void mergesort(int eventos_final[], char fila_relatorio[][13],int inicio, int fim){
    int meio;
    //So entra no if se o tamanho do vetor for maior que 0
    if(inicio<fim){
        meio =inicio + (fim - inicio)/2;//Pega o meio do vetor, pensando que nem sempre o inicio vai ser 0
        mergesort(eventos_final, fila_relatorio, inicio,  meio);//Vai quebrando e quebrando a "primeira parte" ate o inicio e o fim forem 0
        mergesort(eventos_final, fila_relatorio, meio + 1, fim);//Mesma coisa so que com a segunda parte
        merge(eventos_final, fila_relatorio, inicio, meio, fim);//Chama a funçao aonde a ordenaçao realmente acontece
    }
}
int main(){
    char cpf[13];
    char cpfs[10][13];
    char cpf_busca[13];
    char nome[100];
    char nomes[10][100];
    char nascimento[12];
    char nascimentos[10][12];
    char fila_geral[200][13];
    char fila_relatorio[200][13];
    int riscos[200];
    int eventos_final[200];
    int eventos_entrada[200];
    int buscado, quantidade = 0, risco, eventos =0, qnt_relatorios =0,qnt_cadastros =0, opcao;

    while(1){
        printf("\n=======SISTEMA DE ATENDOMENTO=======\n");
        printf("1-Cadastrar paciente\n");
        printf("2-Buscar cadastro\n");
        printf("3-Dar entrada\n");
        printf("4-Chamar proximo paciente\n");
        printf("5-Desistir\n");
        printf("6-Ver tamanho da fila\n");
        printf("7-Ver relatorio do dia\n");

        printf("Escolha uma opção:");
        scanf(" %d", &opcao);
        while(getchar() != '\n');//isso aq limpa o bufer quando eu teclar enter

        switch (opcao){
            case 1:
                printf("Digite seu cpf:\n");
                fgets(cpf, 13, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';//esse aq tmb mas ele serve mais para strings

                printf("Digite o seu nome:\n"); 
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Digite sua data de nascimento:\n");
                fgets(nascimento, 12, stdin);
                nascimento[strcspn(nascimento, "\n")] = '\0';

                cadastrar(cpfs, cpf, nomes, nome, nascimentos, nascimento, &qnt_cadastros, &eventos);
                break;
            case 2:
                printf("Informe o cpf que quer buscar:\n");
                fgets(cpf_busca, 13, stdin);
                cpf_busca[strcspn(cpf_busca, "\n")]='\0';
                buscado = buscar_cadastro(cpfs, cpf_busca, &qnt_cadastros, &eventos);
                if(buscado == -1){
                    printf("cadastro nao encontado\n");
                }else{
                    printf("cpf: %s\n", cpfs[buscado]);
                    printf("nome: %s\n", nomes[buscado]);
                    printf("nascimento: %s\n", nascimentos[buscado]);
                }
                break;
            case 3:
                printf("Digite o cpf para dar entrada:\n");
                fgets(cpf_busca, 13, stdin);
                cpf_busca[strcspn(cpf_busca, "\n")] = '\0';

                printf("Digite o risco do paciente (1 a 5):\n");    
                scanf("%d", &risco);

                buscado = dar_entrada(cpfs, fila_geral, cpf_busca, riscos, eventos_entrada,
                risco,&qnt_cadastros, &quantidade, &eventos);

                if(buscado==0){
                    printf("Cpf nao encontrado\n");
                }else if(buscado==1){
                    printf("Paciente ja se encontra na fila\n");
                }else if(buscado==2){
                    printf("Paciente colocado na fila\n");
                }
                break;
            case 4:
                buscado = chamar_proximo(riscos, cpf, fila_geral, fila_relatorio, eventos_final, eventos_entrada, &quantidade, &eventos, &qnt_relatorios);
                if(buscado == -1){
                    puts("Fila vazia");
                }else{
                    printf("Paciente %s comparecer ao consultorio\n", cpf);
                }
                break;
            case 5:
                printf("Digite o cpf do paciente que quer desistir:\n");
                fgets(cpf_busca, 13, stdin);
                cpf_busca[strcspn(cpf_busca, "\n")] = '\0';

                buscado = desistir(cpf_busca, fila_geral, riscos, eventos_entrada, &quantidade, &eventos);

                if(buscado == -1){
                    puts("Paciente nao esta na lista de espera.");
                }else{
                    puts("Paciente removido da fila.");
                }
                break;
            case 6:
                buscado = tamanho_fila(&quantidade, &eventos);
                printf("%d pessoas esperando na fila\n", buscado);
                break;
            case 7:
                mergesort(eventos_final, fila_relatorio, 0, qnt_relatorios-1);//-1 pq ele quer o indice nao o tamanho

                for(int i=0; i<qnt_relatorios;i++){
                    printf("%s: %d eventos\n", fila_relatorio[i], eventos_final[i]);
                }
                break;
            default:
                return 0;
                break;
        }
        
    }
    
    
}