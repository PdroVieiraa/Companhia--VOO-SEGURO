
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char nome[50];
    char endereco[100];
    char telefone[15];
    int fidelidade; 
} Passageiro;

typedef struct {
    int numero;
    char classe[20];
    int ocupado; 
} Assento;

void cadastrarPassageiro(FILE *arquivo) {
    Passageiro p;
    static int codigoAtual = 1; 

    p.codigo = codigoAtual++;
    printf("Digite o nome do passageiro: ");
    scanf(" %[^
]s", p.nome);
    printf("Digite o endereco do passageiro: ");
    scanf(" %[^
]s", p.endereco);
    printf("Digite o telefone do passageiro: ");
    scanf(" %[^
]s", p.telefone);
    p.fidelidade = 0;

    fwrite(&p, sizeof(Passageiro), 1, arquivo);
    printf("Passageiro cadastrado com sucesso!\n");
}

void gerarAssentos(FILE *arquivo, int quantidade) {
    Assento a;
    for (int i = 1; i <= quantidade; i++) {
        a.numero = i;
        printf("Defina a classe do assento %d (Ex.: Economica, Executiva): ", i);
        scanf(" %[^
]s", a.classe);
        a.ocupado = 0; 

        fwrite(&a, sizeof(Assento), 1, arquivo);
    }
    printf("Assentos gerados e salvos com sucesso!\n");
}

int main() {
    FILE *arquivoPassageiros, *arquivoAssentos;
    int opcao;

    arquivoPassageiros = fopen("passageiros.dat", "ab");
    if (arquivoPassageiros == NULL) {
        printf("Erro ao abrir arquivo de passageiros!\n");
        return 1;
    }

    arquivoAssentos = fopen("assentos.dat", "ab");
    if (arquivoAssentos == NULL) {
        printf("Erro ao abrir arquivo de assentos!\n");
        fclose(arquivoPassageiros);
        return 1;
    }

    do {
        printf("\n=== Menu ===\n");
        printf("1. Cadastrar Passageiro\n");
        printf("2. Gerar Assentos\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarPassageiro(arquivoPassageiros);
                break;
            case 2: {
                int quantidade;
                printf("Quantos assentos deseja gerar? ");
                scanf("%d", &quantidade);
                gerarAssentos(arquivoAssentos, quantidade);
                break;
            }
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);

    fclose(arquivoPassageiros);
    fclose(arquivoAssentos);

    return 0;
}
