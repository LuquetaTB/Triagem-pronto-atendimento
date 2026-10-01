import csv
import random
from datetime import date, timedelta

nomes = [
    "Joao Silva",
    "Maria Souza",
    "Pedro Santos",
    "Ana Oliveira",
    "Lucas Pereira",
    "Julia Costa",
    "Gabriel Rodrigues",
    "Mariana Almeida",
    "Rafael Ferreira",
    "Beatriz Lima"
]


def gerar_cpf():
    return ''.join(str(random.randint(0, 9)) for _ in range(11))


def gerar_data():
    inicio = date(1950, 1, 1)
    fim = date(2005, 12, 31)

    dias = (fim - inicio).days
    data = inicio + timedelta(days=random.randint(0, dias))

    return data.strftime("%d/%m/%Y")


quantidade = int(input("Quantidade de registros: "))

nome_arquivo = f"pacientes_{quantidade}.csv"

with open(nome_arquivo, "w", newline="", encoding="utf-8") as arquivo:

    escritor = csv.writer(arquivo)

    escritor.writerow(["cpf", "nome", "nascimento"])

    for i in range(quantidade):

        cpf = gerar_cpf()

        nome = random.choice(nomes)

        nascimento = gerar_data()

        escritor.writerow([cpf, nome, nascimento])


print(f"Arquivo '{nome_arquivo}' criado com sucesso!")