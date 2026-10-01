#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
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
    long long hash = 0;
    //Pega o cpf_busca e retorna a chave que vai ser usada para cadastrar/procurar o paciente
    for(int i =0; cpf_busca[i] != '\0'; i++){
        hash = hash * 31 + cpf_busca[i];
    }

    return hash % TAM;
}
void cadastrar(char cpfs[][13], char nomes[][100], char nascimentos[][12],char cpf_busca[], char nome[], 
char nascimento[],int *qnt_cadastros, int *eventos){
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
    (*qnt_cadastros)++;
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
    static char cpfs[TAM][13];
    static char nomes[TAM][100];
    static char nascimentos[TAM][12];
    static char cpf_teste[100000][13];
    char cpf_busca[13];
    char nome[100];
    char nascimento[12];
    static paciente heap[100000];
    static relatorio relat[100000];
    int opcao, buscado, risco, quantidade = 0,  eventos = 0, qnt_relatorios = 0, qnt_cadastros = 0;

    inicializarTabela(cpfs);

    clock_t x, y;
    double tempo;

    FILE *arquivo = fopen("pacientes_100000.csv", "r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    char linha[200];

    fgets(linha, 200, arquivo);

    while(fgets(linha, 200, arquivo)){

        char *cpf;
        char *nome;
        char *nascimento;

        cpf = strtok(linha, ",");
        nome = strtok(NULL, ",");
        nascimento = strtok(NULL, "\n");

        strcpy(cpf_teste[qnt_cadastros], cpf);
        cadastrar(cpfs, nomes, nascimentos, cpf, nome, nascimento, &qnt_cadastros, &eventos);
    }
    rewind(arquivo);
    fgets(linha, 200, arquivo);
    

    x = clock();

    while(fgets(linha, 200, arquivo)){
        char *cpf;

        cpf = strtok(linha, ",");

        buscado = buscar_cadastro(cpfs, cpf, &eventos);
        if(buscado == -1){
            printf("cadastro nao encontrado\n");
        }else{
            printf("cpf encontrado %d\n", buscado);
        }
    }

    y = clock();
    fclose(arquivo);

    tempo = (double)(y-x)/CLOCKS_PER_SEC;
    printf("tempo: %f segundos", tempo);

    for(int i =0; i<qnt_cadastros; i++){
        risco = (rand()%5) +1;

        buscado = inserir_heap(heap, cpf_teste, cpf_teste[i], &quantidade, risco, &eventos);
    }

    x = clock();

    for(int i =0; i< quantidade; i++){
        paciente removido;
        buscado = remover_heap(heap, relat, &removido, &quantidade, &qnt_relatorios, &eventos);

        if(buscado == 0){
            printf("fila vazia\n");
        }else if(buscado == 1){
            printf("Paciente %s comparecer ao consultorio\n", removido.cpf);
        }
    }
    y = clock();

    tempo = (double)(y-x)/CLOCKS_PER_SEC;
    printf("tempo: %f segundos", tempo);
    
    return 0;
}