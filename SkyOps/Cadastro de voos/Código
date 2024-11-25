

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VOOS 100
#define MAX_TRIPULACAO 5
#define ARQUIVO_VOOS "voos.bin"

// Estruturas de dados
typedef struct {
    char nome[50];
    char funcao[20]; // "Piloto", "Copiloto", "Comissário"
} Tripulacao;

typedef struct {
    char codigo[10];
    char data[11];    // Formato: DD/MM/AAAA
    char hora[6];     // Formato: HH:MM
    char origem[50];
    char destino[50];
    float tarifa;
    Tripulacao tripulacao[MAX_TRIPULACAO];
    int qtd_tripulacao;
    char aviao[50];
} Voo;

Voo voos[MAX_VOOS];
int total_voos = 0;

// Funções auxiliares para validação
int validarData(const char *data) {
    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(data[i])) return 0;
    }
    int dia = atoi(data);
    int mes = atoi(data + 3);
    int ano = atoi(data + 6);
    if (mes < 1 || mes > 12 || dia < 1 || dia > 31 || ano < 1900) return 0;
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > 28 + bissexto) return 0;
    }
    return 1;
}

int validarHora(const char *hora) {
    if (strlen(hora) != 5 || hora[2] != ':') return 0;
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (!isdigit(hora[i])) return 0;
    }
    int horas = atoi(hora);
    int minutos = atoi(hora + 3);
    return horas >= 0 && horas < 24 && minutos >= 0 && minutos < 60;
}

int validarTarifa(const char *tarifa_str, float *tarifa) {
    char *endptr;
    *tarifa = strtof(tarifa_str, &endptr);
    if (*endptr != '\0' || *tarifa < 0) return 0; // Verifica se houve conversão válida e se o valor é positivo
    return 1;
}

// Função para verificar se o código já existe
int codigoJaExiste(const char *codigo) {
    for (int i = 0; i < total_voos; i++) {
        if (strcmp(voos[i].codigo, codigo) == 0) {
            return 1; // Código já existe
        }
    }
    return 0; // Código não encontrado
}

// Função para salvar os voos em arquivo binário
void salvarVoos() {
    FILE *arquivo = fopen(ARQUIVO_VOOS, "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }
    fwrite(&total_voos, sizeof(int), 1, arquivo);
    fwrite(voos, sizeof(Voo), total_voos, arquivo);
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

// Função para carregar os voos do arquivo binário
void carregarVoos() {
    FILE *arquivo = fopen(ARQUIVO_VOOS, "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de dados encontrado. Iniciando sistema vazio.\n");
        return;
    }
    fread(&total_voos, sizeof(int), 1, arquivo);
    fread(voos, sizeof(Voo), total_voos, arquivo);
    fclose(arquivo);
    printf("Dados carregados com sucesso!\n");
}

// Função para adicionar um novo voo
void adicionarVoo() {
    if (total_voos >= MAX_VOOS) {
        printf("Limite de voos atingido!\n");
        return;
    }

    Voo novoVoo;

    do {
        printf("Código do voo: ");
        scanf("%s", novoVoo.codigo);
        if (codigoJaExiste(novoVoo.codigo)) {
            printf("Código já existe! Por favor, insira um código único.\n");
        }
    } while (codigoJaExiste(novoVoo.codigo));

    do {
        printf("Data do voo (DD/MM/AAAA): ");
        scanf("%s", novoVoo.data);
        if (!validarData(novoVoo.data)) {
            printf("Data inválida! Use o formato DD/MM/AAAA e insira apenas números válidos.\n");
        }
    } while (!validarData(novoVoo.data));

    do {
        printf("Hora do voo (HH:MM): ");
        scanf("%s", novoVoo.hora);
        if (!validarHora(novoVoo.hora)) {
            printf("Hora inválida! Use o formato HH:MM e insira apenas números válidos.\n");
        }
    } while (!validarHora(novoVoo.hora));

    printf("Origem: ");
    scanf(" %[^\n]", novoVoo.origem);

    printf("Destino: ");
    scanf(" %[^\n]", novoVoo.destino);

    char tarifa_str[20];
    do {
        printf("Tarifa (apenas números): ");
        scanf("%s", tarifa_str);
        if (!validarTarifa(tarifa_str, &novoVoo.tarifa)) {
            printf("Tarifa inválida! Insira um número positivo válido.\n");
        }
    } while (!validarTarifa(tarifa_str, &novoVoo.tarifa));

    printf("Informe o nome do avião: ");
    scanf(" %[^\n]", novoVoo.aviao);

    // Adicionar tripulação
    novoVoo.qtd_tripulacao = 0;
    int pilotos = 0, copilotos = 0;

    while (novoVoo.qtd_tripulacao < MAX_TRIPULACAO) {
        Tripulacao membro;

        printf("\nNome do membro da tripulação: ");
        scanf(" %[^\n]", membro.nome);

        printf("Função (Piloto, Copiloto, Comissário): ");
        scanf(" %[^\n]", membro.funcao);

        if (strcmp(membro.funcao, "Piloto") == 0) {
            pilotos++;
        } else if (strcmp(membro.funcao, "Copiloto") == 0) {
            copilotos++;
        }

        novoVoo.tripulacao[novoVoo.qtd_tripulacao++] = membro;

        printf("Deseja adicionar mais membros? (S/N): ");
        char continuar;
        scanf(" %c", &continuar);
        if (continuar == 'N' || continuar == 'n') {
            break;
        }
    }

    if (pilotos < 1 || copilotos < 1) {
        printf("O voo deve ter pelo menos um piloto e um copiloto!\n");
        return;
    }

    voos[total_voos++] = novoVoo;
    printf("Voo cadastrado com sucesso!\n");

    salvarVoos(); // Salvar os dados atualizados
}

// Função para listar todos os voos
void listarVoos() {
    if (total_voos == 0) {
        printf("Nenhum voo cadastrado.\n");
        return;
    }

    for (int i = 0; i < total_voos; i++) {
        printf("\nVoo %d:\n", i + 1);
        printf("Código: %s\n", voos[i].codigo);
        printf("Data: %s\n", voos[i].data);
        printf("Hora: %s\n", voos[i].hora);
        printf("Origem: %s\n", voos[i].origem);
        printf("Destino: %s\n", voos[i].destino);
        printf("Tarifa: %.2f\n", voos[i].tarifa);
        printf("Avião: %s\n", voos[i].aviao);
        printf("Tripulação:\n");
        for (int j = 0; j < voos[i].qtd_tripulacao; j++) {
            printf("  Nome: %s, Função: %s\n", voos[i].tripulacao[j].nome, voos[i].tripulacao[j].funcao);
        }
    }
}

// Menu principal
int main() {
    carregarVoos(); // Carregar os dados do arquivo ao iniciar o programa

    int opcao;

    do {
        printf("\n--- Sistema de Cadastro de Voos ---\n");
        printf("1. Adicionar voo\n");
        printf("2. Listar voos\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarVoo();
                break;
            case 2:
                listarVoos();
                break;
            case 3:
                salvarVoos();
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);

    return 0;
}
