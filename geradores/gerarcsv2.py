import csv
import random

def gerar_cpf():
    return ''.join(str(random.randint(0, 9)) for _ in range(11))

def gerar_arquivo(quantidade):
    nome_arquivo = f"relatorio_{quantidade}.csv"

    cpfs = set()

    while len(cpfs) < quantidade:
        cpfs.add(gerar_cpf())

    with open(nome_arquivo, "w", newline="", encoding="utf-8") as arquivo:
        escritor = csv.writer(arquivo)

        escritor.writerow(["cpf", "eventos"])

        for cpf in cpfs:
            eventos = random.randint(1, 100)
            escritor.writerow([cpf, eventos])

    print(f"{nome_arquivo} criado!")


gerar_arquivo(10000)
gerar_arquivo(100000)