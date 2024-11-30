#ifndef CADASTRO_ASSENTOS_H
#define CADASTRO_ASSENTOS_H

typedef struct {
    int numero;
    int codigoVoo;
    int status;
} Assento;

int cadastrarAssento(Assento *novoAssento);

void listarAssentosPorVoo(int codigoVoo);

int verificarDisponibilidade(int codigoVoo, int numeroAssento);

void salvarAssentosEmArquivo();

void carregarAssentosDeArquivo();

#endif