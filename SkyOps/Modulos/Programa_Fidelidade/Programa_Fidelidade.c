#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PASSAGEIROS 100
#define ARQUIVO_FIDELIDADE "fidelidade_manual.bin"

// Estrutura para armazenar os dados do passageiro
typedef struct {
    char id[20];
    char nome[50];
    int pontos;
} Passageiro;

Passageiro passageiros[MAX_PASSAGEIROS];
int total_passageiros = 0;

// Função para carregar os dados do arquivo
void carregarPassageiros() {
    FILE *arquivo = fopen(ARQUIVO_FIDELIDADE, "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de fidelidade encontrado. Iniciando sistema vazio.\n");
        return;
    }
    fread(&total_passageiros, sizeof(int), 1, arquivo);
    fread(passageiros, sizeof(Passageiro), total_passageiros, arquivo);
    fclose(arquivo);
    printf("Dados de fidelidade carregados com sucesso!\n");
}

// Função para salvar os dados no arquivo
void salvarPassageiros() {
    FILE *arquivo = fopen(ARQUIVO_FIDELIDADE, "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar os dados");
        return;
    }
    fwrite(&total_passageiros, sizeof(int), 1, arquivo);
    fwrite(passageiros, sizeof(Passageiro), total_passageiros, arquivo);
    fclose(arquivo);
    printf("Dados de fidelidade salvos com sucesso!\n");
}

// Função para verificar se o código único já existe
int buscarPassageiro(const char *id) {
    for (int i = 0; i < total_passageiros; i++) {
        if (strcmp(passageiros[i].id, id) == 0) {
            return i; // Retorna o índice do passageiro
        }
    }
    return -1; // Não encontrado
}

// Função para validar se o código único é numérico
int validarIdNumerico(const char *id) {
    for (int i = 0; id[i] != '\0'; i++) {
        if (!isdigit(id[i])) {
            return 0; // Caracter inválido encontrado
        }
    }
    return 1; // ID válido
}

// Função para registrar pontos para um passageiro
void registrarPontos() {
    char id[20], nome[50];

    do {
        printf("Digite o código único do passageiro (somente números): ");
        scanf("%s", id);
        if (!validarIdNumerico(id)) {
            printf("Código inválido! Use apenas números.\n");
        }
    } while (!validarIdNumerico(id));

    int indice = buscarPassageiro(id);

    if (indice != -1) {
        // Passageiro já existe, adiciona pontos
        passageiros[indice].pontos += 10;
        printf("10 pontos adicionados para o passageiro %s (%s).\n", passageiros[indice].nome, passageiros[indice].id);
    } else {
        // Novo passageiro, cria registro
        if (total_passageiros >= MAX_PASSAGEIROS) {
            printf("Limite de passageiros atingido!\n");
            return;
        }

        printf("Digite o nome do passageiro: ");
        scanf(" %[^\n]", nome);

        strcpy(passageiros[total_passageiros].id, id);
        strcpy(passageiros[total_passageiros].nome, nome);
        passageiros[total_passageiros].pontos = 10;
        total_passageiros++;

        printf("Novo passageiro cadastrado com 10 pontos: %s (%s).\n", nome, id);
    }

    salvarPassageiros(); // Salvar alterações no arquivo
}

// Função para consultar os pontos de um passageiro
void consultarPontos() {
    char id[20];

    printf("Digite o código único do passageiro: ");
    scanf("%s", id);

    int indice = buscarPassageiro(id);

    if (indice != -1) {
        printf("Passageiro: %s (%s)\n", passageiros[indice].nome, passageiros[indice].id);
        printf("Pontos acumulados: %d\n", passageiros[indice].pontos);
    } else {
        printf("Passageiro não encontrado!\n");
    }
}

// Função para remover pontos de um passageiro
void removerPontos() {
    char id[20];
    int pontosARemover;

    printf("Digite o código único do passageiro: ");
    scanf("%s", id);

    int indice = buscarPassageiro(id);

    if (indice != -1) {
        printf("Passageiro encontrado: %s (%s)\n", passageiros[indice].nome, passageiros[indice].id);
        printf("Pontos atuais: %d\n", passageiros[indice].pontos);

        do {
            printf("Digite o número de pontos a remover: ");
            scanf("%d", &pontosARemover);

            if (pontosARemover < 0) {
                printf("Você não pode remover uma quantidade negativa de pontos.\n");
            } else if (pontosARemover > passageiros[indice].pontos) {
                printf("Quantidade de pontos a remover excede os pontos disponíveis.\n");
            }
        } while (pontosARemover < 0 || pontosARemover > passageiros[indice].pontos);

        passageiros[indice].pontos -= pontosARemover;
        printf("%d pontos removidos. Pontos restantes: %d\n", pontosARemover, passageiros[indice].pontos);

        salvarPassageiros(); // Salvar alterações no arquivo
    } else {
        printf("Passageiro não encontrado!\n");
    }
}

// Função para validar a entrada do menu principal
int lerOpcaoMenu() {
    char entrada[10];
    int opcao;
    do {
        printf("\n--- Programa de Fidelidade Manual ---\n");
        printf("1. Registrar pontos\n");
        printf("2. Consultar pontos\n");
        printf("3. Remover pontos\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%s", entrada);

        if (strlen(entrada) == 1 && isdigit(entrada[0])) {
            opcao = entrada[0] - '0'; // Converte o caractere para inteiro
            if (opcao >= 1 && opcao <= 4) {
                return opcao; // Retorna a opção válida
            }
        }
        printf("Opção inválida! Por favor, escolha uma opção entre 1 e 4.\n");
    } while (1);
}

// Menu principal
int main() {
    carregarPassageiros();

    int opcao;

    do {
        opcao = lerOpcaoMenu();

        switch (opcao) {
            case 1:
                registrarPontos();
                break;
            case 2:
                consultarPontos();
                break;
            case 3:
                removerPontos();
                break;
            case 4:
                salvarPassageiros();
                printf("Saindo...\n");
                break;
        }
    } while (opcao != 4);

    return 0;
}
