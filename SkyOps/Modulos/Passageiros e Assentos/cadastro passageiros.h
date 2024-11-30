#ifndef CADASTRO_PASSAGEIROS_H
#define CADASTRO_PASSAGEIROS_H

typedef struct {
    int codigo;           
    char nome[100];       
    char endereco[150];
    char telefone[20];
    int fidelidade;       
    int pontos;           
} Passageiro;


int cadastrarPassageiro(Passageiro *novoPassageiro);

void listarPassageiros();

Passageiro* buscarPassageiro(int codigo);

void salvarPassageirosEmArquivo();

void carregarPassageirosDeArquivo();

#endif