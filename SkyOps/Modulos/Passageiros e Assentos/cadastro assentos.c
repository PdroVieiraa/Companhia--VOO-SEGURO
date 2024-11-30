#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cadastro_assentos.h"

// Arquivo para persistência dos dados
#define ARQUIVO_ASSENTOS "assentos.dat"

// Função para cadastrar um assento
int cadastrarAssento(Assento *novoAssento) {
    FILE *arquivo = fopen(ARQUIVO_ASSENTOS, "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de assentos");
        return 0;
    }

    fwrite(novoAssento, sizeof(Assento), 1, arquivo);
    fclose(arquivo);
    return 1; // Sucesso
}

// Função para listar todos os assentos de um voo
void listarAssentosPorVoo(int codigoVoo) {
    FILE *arquivo = fopen(ARQUIVO_ASSENTOS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de assentos");
        return;
    }

    Assento assento;
    printf("Assentos do Voo %d:\n", codigoVoo);
    while (fread(&assento, sizeof(Assento), 1, arquivo)) {
        if (assento.codigoVoo == codigoVoo) {
            printf("Número: %d, Status: %s\n",
                   assento.numero,
                   assento.status ? "Ocupado" : "Livre");
        }
    }
    fclose(arquivo);
}

// Função para verificar a disponibilidade de um assento
int verificarDisponibilidade(int codigoVoo, int numeroAssento) {
    FILE *arquivo = fopen(ARQUIVO_ASSENTOS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de assentos");
        return 0;
    }

    Assento assento;
    while (fread(&assento, sizeof(Assento), 1, arquivo)) {
        if (assento.codigoVoo == codigoVoo && assento.numero == numeroAssento) {
            fclose(arquivo);
            return assento.status == 0; // Disponível se status == 0
        }
    }
    fclose(arquivo);
    return 0; // Não encontrado ou ocupado
}

// Função para salvar os dados dos assentos em um arquivo binário
void salvarAssentosEmArquivo() {
    printf("Os dados dos assentos são salvos automaticamente ao cadastrar.\n");
}

// Função para carregar os dados dos assentos de um arquivo binário
void carregarAssentosDeArquivo() {
    printf("Os dados dos assentos são carregados diretamente ao abrir o sistema.\n");
}
