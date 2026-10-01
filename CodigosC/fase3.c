#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TAM 199999

//Estrutura que guarda variaveis de tipos diferentes dentro de uma so 
typedef struct{
    char cpf[13];
    int risco;
    int chegada;
}paciente;//contem tipos de dados necessarios de um paciente
typedef struct{
    char cpf[13];
    int eventos;
}relatorio;//Contem dados necessarios em um relatorio
//Funcões da tabela hash
void inicializarTabela(char cpfs[][13]){
    //Coloca o caractere '\0' em todos os indices da tabela
    for(int i = 0; i<TAM; i++){
        cpfs[i][0] = '\0';
    }
}
int funcaoHash(char cpf_busca[]){
    int hash = 0;
    //Pega o cpf_busca e retorna a chave que vai ser usada para cadastrar/procurar o paciente
    for(int i =0; cpf_busca[i] != '\0'; i++){
        hash = hash * 31 + cpf_busca[i];
    }

    return hash % TAM;
}
void cadastrar(char cpfs[][13], char nomes[][100], char nascimentos[][12],char cpf_busca[], char nome[], 
char nascimento[], int *eventos){
    (*eventos)++;
    /*Cadastra o cpf_busca de acordo com a chave gerada pela função hash. Se o indice gerado pela
     chave ja estiver cadastrado, ele vai procurando uma chave vazia ate encontar.*/
    int chave = funcaoHash(cpf_busca);
    while(cpfs[chave][0] != '\0'){
        chave = (chave + 1) % TAM;
    }

    //Coloca os valores digitados nas strings com a mesma chave para melhorar a busca
    strcpy(cpfs[chave], cpf_busca);
    strcpy(nomes[chave], nome);
    strcpy(nascimentos[chave], nascimento);
}
int buscar_cadastro(char cpfs[][13], char cpf_busca[], int *eventos){
    (*eventos)++;
    /*Procura o cadastro de acordo com a chave gerada pela função hash. Procura ate o primeiro 
    indice vazio que encontrar, se nao achar, o cadastro nao existe.*/
    int chave = funcaoHash(cpf_busca);
    while(cpfs[chave][0] != '\0'){
        if(strcmp(cpfs[chave], cpf_busca) == 0){
            return chave;
        }else{
            chave = (chave + 1) % TAM;
        }
    }
    return -1;
}
//Funçoes da Heap
void subir(paciente heap[], int i){
    int pai = (i - 1) / 2;//Calcula o pai do indice que foi passado como parametro na função

    /*Enquanto o indice for maior que 0 e o risco do "filho" for maior que o do "pai" ou igual ao do pai e a ordem de
    chegada for menor o while vai rodando e trocando o "filho" de posiçao com seu "pai"*/
    while(i > 0 && 
        (heap[i].risco < heap[pai].risco || 
        (heap[i].risco == heap[pai].risco && heap[i].chegada < heap[pai].chegada))){
        paciente temp =heap[i];
        heap[i] = heap[pai];
        heap[pai] = temp;

        i = pai;
        pai = (i - 1) / 2;
    }
}
int inserir_heap(paciente heap[], char cpfs[][13], char cpf_busca[], int *quantidade, int risco, int *eventos){
    (*eventos)++;
    //Vai buscar o cpf nos cadastros e se nao encontrar retorna -1
    int buscado = buscar_cadastro(cpfs, cpf_busca, eventos);
    (*eventos)--;

    if(buscado == -1){
        return -1;
    }

    //Ve se o cpf mandado ja esta na fila de espera
    for(int i = 0; i< *quantidade; i++){
        if(strcmp(heap[i].cpf, cpf_busca) == 0){
            return 0;
        }
    }

    //A variavel quantidade é encarregada de contar quantos tem na fila e é usada como indice dos novos pacientes
    strcpy(heap[*quantidade].cpf, cpf_busca);
    heap[*quantidade].risco = risco;
    heap[*quantidade].chegada = *eventos;
    (*quantidade)++;

    //Depois de adicionar o novo paciente a funçao "subir" é chamada para ordenar novamente a heap
    subir(heap, *quantidade -1);
    return 1;
}
void descer(paciente heap[], int *quantidade, int i){
    int menor = i;

    int esquerda = 2 * i + 1;//calcula o "filho" da esquerda 
    int direita = 2 * i + 2;//Calcula o "filho" da direita

    //Pega o menor dos 3(pai, filho e filho) respeitando todas as regras de desempate
    if(esquerda < *quantidade && 
        (heap[esquerda].risco < heap[menor].risco ||
        (heap[esquerda].risco == heap[menor].risco && heap[esquerda].chegada < heap[menor].chegada))){
        menor = esquerda;
    }
    if(direita < *quantidade && 
        (heap[direita].risco < heap[menor].risco || 
        (heap[direita].risco == heap[menor].risco && heap[direita].chegada < heap[menor].chegada))){
        menor = direita;
    }

    /*Se o menor numero for diferente do que foi passado como parametro la em cima, entao, troca o pai
    com o menor dos filhos e chama a funçao descer novamente.*/
    if(menor != i){
        paciente temp = heap[i];
        heap[i] = heap[menor];
        heap[menor] = temp;

        descer(heap, quantidade, menor);
    }
}
int remover_heap(paciente heap[], relatorio relat[], paciente *removido, int *quantidade, int *qnt_relatorios, int *eventos){
    (*eventos)++;

    //tratamento para quando a fila estiver vazia
    if(*quantidade == 0){
        return 0;
    }

    //Variavel removido recebe o paciente que sera chamado
    *removido = heap[0];

    //Coloca o paciente atendido no relatorio com o seu cpf e numero de eventos ate a chamada
    strcpy(relat[*qnt_relatorios].cpf, heap[0].cpf);
    relat[*qnt_relatorios].eventos = *eventos - heap[0].chegada;
    (*qnt_relatorios)++;

    //Topo recebe o valor que estiver na ultima "colocação" e chama a função descer para organizar novamente
    heap[0] = heap[*quantidade - 1];
    (*quantidade)--;
    
    descer(heap, quantidade, 0);

    return 1;
}
int desistir(paciente heap[], char cpf_busca[], int *quantidade, int *eventos){
    (*eventos)++;
    int i = -1;
    //pega o indice aonde se encontra o cpf do paciente que vai desistir
    for(int j =0; j<*quantidade; j++){
        if(strcmp(heap[j].cpf, cpf_busca)==0){
            i = j;
            break;
        }
    }

    //tratamento se o paciente nao estiver na fila
    if(i == -1){
        return -1;
    }
    //Coloca o ultimo na fila no lugar do paciente desistente
    heap[i] = heap[*quantidade - 1];
    (*quantidade)--;
    
    //Chama a funçao descer para organizar a partir do indice do desistente
    descer(heap, quantidade, i);
    return 0;
}
int tamanho_fila(int *quantidade, int *eventos){
    (*eventos)++;
    //"Quantidade" possui o numero de pessoas na fila
    return *quantidade;
}
void merge(relatorio relat[], int inicio, int meio, int fim){
    int i, j, k;
    int n1 = meio - inicio + 1;//Pega a primeira metade do vetor
    int n2 = fim - meio;// pega a segunda metade

    relatorio *esquerda = malloc(n1 * sizeof(relatorio));//reserva a quantidade de bytes que o vetor vai precisar
    relatorio *direita = malloc(n2 * sizeof(relatorio));// msm coisa mas com n2

    for(i=0; i<n1; i++){
        //Aqui coloca os valores nos vetor "auxiliar"
        esquerda[i] = relat[inicio + i];
        
    }
    for ( j = 0; j <n2 ; j++){
        //Mesma coisa aqui mas com os valores do meio ate o fim
        direita[j] = relat[meio + 1 + j];
    }
    
    i = 0;
    j = 0;
    k = inicio;
    //Enquanto nao forem todos os numeros de um vetor o while continua rodando
    while(i<n1 && j<n2){
        if(esquerda[i].eventos>= direita[j].eventos){
            //Aqui finalmente ocorre a comparaçao e ordenaçao dos valores
            relat[k] = esquerda[i];
            i++;
            k++;
        }
        else{
            relat[k] = direita[j];
            j++;
            k++;
        }
    }
    //Valores que ficaram nos vetores sao nseridos no vetor principal de forma ordenada
    while(i<n1){
        relat[k] = esquerda[i];
        i++;
        k++;
    }
    while (j<n2){
        relat[k] = direita[j];
        j++;
        k++;
    }

    //Libera a memoria 
    free(esquerda);
    free(direita);
}
void mergesort(relatorio relat[], int inicio, int fim){
    int meio;
    //So entra no if se o tamanho do vetor for maior que 0
    if(inicio<fim){
        meio =inicio + (fim - inicio)/2;//Pega o meio do vetor, pensando que nem sempre o inicio vai ser 0
        mergesort(relat, inicio,  meio);//Vai quebrando e quebrando a "primeira parte" ate o inicio e o fim forem 0
        mergesort(relat, meio + 1, fim);//Mesma coisa so que com a segunda parte
        merge(relat, inicio, meio, fim);//Chama a funçao aonde a ordenaçao realmente acontece
    }
}
int main(){
    char cpfs[TAM][13];
    char nomes[TAM][100];
    char nascimentos[TAM][12];
    char cpf_busca[13];
    char nome[100];
    char nascimento[12];
    paciente heap[100];
    relatorio relat[100];
    int opcao, buscado, risco, quantidade = 0,  eventos = 0, qnt_relatorios = 0;

    while(1){
        printf("=====SISTEMA DE ATENDIMENTO=====\n");
        printf("1-Cadastrar paciente\n");
        printf("2-Buscar paciente\n");
        printf("3-Dar entrada\n");
        printf("4-Chamar proximo paciente\n");
        printf("5-Desistir\n");
        printf("6-Ver tamanho da fila\n");
        printf("7-Ver relatorio do dia\n");
        scanf(" %d", &opcao);
        while(getchar() != '\n');//limpar o buffer do enter digitado

        switch (opcao){
        case 1:
            printf("Digite seu cpf_busca:\n");
            fgets(cpf_busca, 13, stdin);
            cpf_busca[strcspn(cpf_busca, "\n")] = '\0';//limpa o buffer tmb mas é especializado para strings

            printf("Digite o seu nome:\n"); 
            fgets(nome, 100, stdin);
            nome[strcspn(nome, "\n")] = '\0';

            printf("Digite sua data de nascimento:\n");
            fgets(nascimento, 12, stdin);
            nascimento[strcspn(nascimento, "\n")] = '\0';

            cadastrar(cpfs, nomes, nascimentos, cpf_busca, nome, nascimento, &eventos);
            break;
        case 2:
            printf("Informe o cpf_busca que quer buscar:\n");
            fgets(cpf_busca, 13, stdin);
            cpf_busca[strcspn(cpf_busca, "\n")]='\0';
            buscado = buscar_cadastro(cpfs, cpf_busca, &eventos);
            if(buscado == -1){
                printf("cadastro nao encontado\n");
            }else{
                printf("cpf_busca: %s\n", cpfs[buscado]);
                printf("nome: %s\n", nomes[buscado]);
                printf("nascimento: %s\n", nascimentos[buscado]);
            }
            break;
        case 3:
            printf("Digite o cpf para dar entrada no paciente\n");
            fgets(cpf_busca, 13, stdin);
            cpf_busca[strcspn(cpf_busca, "\n")] = '\0';

            printf("Digite o risco do paciente(1 a 5)\n");
            scanf("%d", &risco);

            buscado = inserir_heap(heap, cpfs, cpf_busca, &quantidade, risco, &eventos);

            if(buscado == -1){
                printf("Cpf nao encontrado\n");
            }else if(buscado == 0){
                printf("paciente ja se encontra na fila\n");
            }else if(buscado == 1){
                printf("Paciente colocado na fila\n");
            }
            break;
        case 4:
            paciente removido;

            buscado = remover_heap(heap, relat, &removido, &quantidade, &qnt_relatorios, &eventos);
            if(buscado == 0){
                printf("fila vazia\n");
            }else if(buscado == 1){
                printf("Paciente %s comparecer ao consultorio\n", removido.cpf);
            }
            break;
        case 5:
            printf("digite o cpf do paciente que quer desistir\n");
            fgets(cpf_busca, 13, stdin);
            cpf_busca[strcspn(cpf_busca, "\n")] = '\0';
            
            buscado = desistir(heap, cpf_busca, &quantidade, &eventos);
            if(buscado == -1){
                printf("paciente nao esta na fila\n");
            }else{
                printf("Paciente removido da fila");
            }
            break;
        case 6:
            buscado = tamanho_fila(&quantidade, &eventos);
            printf("%d pessoas esperando na fila", buscado);
            break;
        case 7:
            mergesort(relat, 0, qnt_relatorios - 1);

            for(int i = 0; i<qnt_relatorios; i++){
                printf("%s: %d eventos\n", relat[i].cpf, relat[i].eventos);
            }
            break;
        default:
            break;
        }
    }
    return 0;
}