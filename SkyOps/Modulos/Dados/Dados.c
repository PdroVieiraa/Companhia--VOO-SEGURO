#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DADOS 256 // Tamanho máximo do dado armazenado
#define NOME_ARQUIVO "Data.bin" // Nome fixo do arquivo de dados

typedef struct {
    int id;          
    char tipo[50];   
    char dados[MAX_DADOS]; 
} DADOS_GENERICOS;

void salvarDados(DADOS_GENERICOS *novo) {
    FILE *fp = fopen(NOME_ARQUIVO, "ab");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(novo, sizeof(DADOS_GENERICOS), 1, fp);
    fclose(fp);
    printf("DADOS_GENERICOS salvo com sucesso! Tipo: %s, ID: %d\n", novo->tipo, novo->id);
}

void atualizarDADOS_GENERICOS(int id, const char *tipo, DADOS_GENERICOS *atualizado) {
    FILE *fp = fopen(NOME_ARQUIVO, "r+b");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    DADOS_GENERICOS temp;
    while (fread(&temp, sizeof(DADOS_GENERICOS), 1, fp)) {
        if (temp.id == id && strcmp(temp.tipo, tipo) == 0) {
            fseek(fp, -sizeof(DADOS_GENERICOS), SEEK_CUR);
            fwrite(atualizado, sizeof(DADOS_GENERICOS), 1, fp);
            printf("DADOS_GENERICOS atualizado com sucesso! Tipo: %s, ID: %d\n", tipo, id);
            fclose(fp);
            return;
        }
    }

    printf("DADOS_GENERICOS com Tipo: %s e ID: %d não encontrado.\n", tipo, id);
    fclose(fp);
}

void apagarDADOS_GENERICOS(int id, const char *tipo) {
    FILE *fp = fopen(NOME_ARQUIVO, "rb");
    FILE *tempFp = fopen("temp.bin", "wb");

    if (!fp || !tempFp) {
        perror("Erro ao abrir os arquivos");
        if (fp) fclose(fp);
        if (tempFp) fclose(tempFp);
        return;
    }

    DADOS_GENERICOS temp;
    int encontrado = 0;

    while (fread(&temp, sizeof(DADOS_GENERICOS), 1, fp)) {
        if (!(temp.id == id && strcmp(temp.tipo, tipo) == 0)) {
            fwrite(&temp, sizeof(DADOS_GENERICOS), 1, tempFp);
        } else {
            encontrado = 1;
        }
    }

    fclose(fp);
    fclose(tempFp);

    remove(NOME_ARQUIVO);
    rename("temp.bin", NOME_ARQUIVO);

    if (encontrado) {
        printf("DADOS_GENERICOS com Tipo: %s e ID: %d apagado com sucesso!\n", tipo, id);
    } else {
        printf("DADOS_GENERICOS com Tipo: %s e ID: %d não encontrado.\n", tipo, id);
    }
}

void carregarDADOS_GENERICOSs() {
    FILE *fp = fopen(NOME_ARQUIVO, "rb");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    DADOS_GENERICOS temp;
    printf("Dados no arquivo:\n");
    while (fread(&temp, sizeof(DADOS_GENERICOS), 1, fp)) {
        printf("Tipo: %s, ID: %d, Dados: %s\n", temp.tipo, temp.id, temp.dados);
    }

    fclose(fp);
}


/*SIMULAÇÃO NO MAIN*/

int main() {
    DADOS_GENERICOS dado1 = {1, "Tripulante", "Nome: João"};
    DADOS_GENERICOS dado2 = {2, "Passageiro", "Nome: Maria"};
    DADOS_GENERICOS dadoAtualizado = {1, "Tripulante", "Nome: João Silva"};

    // Salvar dados
    salvarDados(&dado1);
    salvarDados(&dado2);

    // Carregar dados
    carregarDADOS_GENERICOSs();

    // Atualizar um dado
    atualizarDADOS_GENERICOS(1, "Tripulante", &dadoAtualizado);
    carregarDADOS_GENERICOSs();

    // Apagar um dado
    apagarDADOS_GENERICOS(2, "Passageiro");
    carregarDADOS_GENERICOSs();

    return 0;
}
