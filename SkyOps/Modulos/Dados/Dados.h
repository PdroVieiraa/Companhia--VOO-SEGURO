#ifndef DADOS_H
#define DADOS_H

#define MAX_DADOS 256 
#define NOME_ARQUIVO "Data.bin"

typedef struct {
    int id;          
    char tipo[50];   
    char dados[MAX_DADOS]; 
} DADOS_GENERICOS;

void salvarDados(DADOS_GENERICOS *novo);
void atualizarDADOS_GENERICOS(int id, const char *tipo, DADOS_GENERICOS *atualizado);
void apagarDADOS_GENERICOS(int id, const char *tipo);
void carrregarDados(DADOS_GENERICOS *dados, int id, const char *tipo);

#endif