# Sistema de triagem no pronto atendimento

Projeto desenvolvido em C para gerenciamento de pacientes em um sistema de triagem, com cadastro, busca, entrada e chamada de paciente, desistencia, tamanho da fila e relatório.

O projeto consiste em um sistema de triagem e pronto atendimento, com operaçoes para para cadastrar novos pacientes usando seus dados(cpf, nome e nascimento), buscar um paciente e fazer o devido tratamento caso ele não esteja cadastrado, gerenciar a fila de atendimento respeitando as regras de prioridade e de chegada, registrar todos os pacientes atendidos e organizar eles com base no tempo (eventos) que levou para eles serem atendidos.

O projeto também aborda a análise de desempenho de diferentes estruturas e algoritmos de dados, comparando suas implementações conforme o tamanho da entrada.

## Funcionalidades

- Cadastro de pacientes
- Busca de pacientes pelo CPF
- Entrada de pacientes na fila de atendimento
- Atendimento de pacientes conforme o nível de risco
- Desistência de pacientes da fila
- Consulta do tamanho da fila
- Geração de relatório dos pacientes atendidos
- Ordenação dos dados para geração do relatório
- Execução de testes com diferentes quantidades de registros
- Análise de desempenho dos algoritmos utilizados

 ## Estruturas e algoritmos utilizados

### Fase 1
- Vetores para armazenamento dos dados
- Busca linear para localização de pacientes
- Fila de atendimento implementada com vetores
- Ordenação por Bubble Sort 

### Fase 2
- Insertion Sort
- Merge Sort
- Comparação de desempenho entre os algoritmos
- Testes com diferentes tamanhos de entrada

### Fase 3
- Tabela Hash para cadastro e busca de pacientes
- Heap para gerenciamento da fila de atendimento
- Comparação de desempenho com as implementações anteriores

## como executar

### Pré requisitos
- GCC -- compilador para os programas em C
- Python -- para os geradores
- Terminal

### Gerador de arquivos CSV
Os arquivos CSV utilizados nos testes são gerados pelos scripts Python disponíveis na pasta de geradores.
Entre na pasta dos geradores e execute o script correspondente:
python3 nome_do_gerador.py

### Arquivo principal
Entre no diretório que contém os arquivos do projeto e compile o programa principal:
gcc main.c -o 

### Executando os testes
Antes de executar os testes é necessario verificar duas coisas:
1. A constante MAX define o tamanho dos vetores. Em qualquer teste (Menos o teste da fase 3, esse é diferente), voce deve mudar o valor dessa constante para o numero de cadastros/Relatorios que voce quer testar (10.000 ou 100.000). Na fase 3 voce deve mudar mudar o numero da constante TAM para o numero primo mais proximo do dobro de testes qeu vc vai fazer (ex: 100.000 seria = 199.999(esse seria o valor da constante TAM)). Ainda na fase 3, troque o tamanho das structs(linha 256 e 257) para o numero de testes que voce vai fazer.
2. Na linha: FILE *arquivo = fopen("pacientes_100000.csv", "r"); coloque o nome do arquivo csv que voce vai usar

Melhor utilizar os csv gerados pelo codigo disponiibilizado em "geradores", principalmente para os testes de testeMerge.c e testeInsert.c

o geradorcsv.py é para gerar o csv dos testes: testeMain2.c e testeMain3.c
o geradorcsv2.py é para gerar o csv dos tstes: testeInsert.c e testeMerge.c

### Teste com diferentes tamanhos
Para executar os testes com 10.000 e 100.000 registros, não é necessário criar um código diferente para cada tamanho.

Basta:
1. Alterar o valor de MAX
2. Alterar o valor das variaveis (se precisar)
3. alterar o nome do arquivo csv no fopen()
4. salvar alteraçoes
5. Compilar codigo
6. executar o teste
