#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cadastro_passageiros.h"

// Arquivo para persistência dos dados
#define ARQUIVO_PASSAGEIROS "passageiros.dat"

// Função para cadastrar um passageiro
int cadastrarPassageiro(Passageiro *novoPassageiro) {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de passageiros");
        return 0;
    }

    fwrite(novoPassageiro, sizeof(Passageiro), 1, arquivo);
    fclose(arquivo);
    return 1; // Sucesso
}

// Função para listar todos os passageiros
void listarPassageiros() {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de passageiros");
        return;
    }

    Passageiro passageiro;
    printf("Lista de Passageiros:\n");
    while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
        printf("Código: %d, Nome: %s, Endereço: %s, Telefone: %s, Fidelidade: %s, Pontos: %d\n",
               passageiro.codigo,
               passageiro.nome,
               passageiro.endereco,
               passageiro.telefone,
               passageiro.fidelidade ? "Sim" : "Não",
               passageiro.pontos);
    }
    fclose(arquivo);
}

// Função para buscar um passageiro pelo código
Passageiro* buscarPassageiro(int codigo) {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de passageiros");
        return NULL;
    }

    static Passageiro passageiro; // Necessário ser static para retornar o ponteiro
    while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
        if (passageiro.codigo == codigo) {
            fclose(arquivo);
            return &passageiro;
        }
    }
    fclose(arquivo);
    return NULL; // Não encontrado
}

// Função para salvar os dados dos passageiros (já implementada no cadastrar)
void salvarPassageirosEmArquivo() {
    printf("Os dados dos passageiros são salvos automaticamente ao cadastrar.\n");
}

// Função para carregar os dados dos passageiros (opcional se já salvarmos diretamente)
void carregarPassageirosDeArquivo() {
    printf("Os dados dos passageiros são carregados diretamente ao abrir o sistema.\n");
}
