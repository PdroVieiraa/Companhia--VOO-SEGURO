#ifndef BUSCA_H
#define BUSCA_H


#define MAX 200

typedef struct{
    int numeroAssento;
    char *nomePassageiro;
}Assentos;

typedef struct{
    char *origemdoaviao; //deixar como padrao tudo minusculo
    char *destinodoaviao; //deixar como padrao tudo minusculo
    int assentos[2][MAX]; //1 ocupado e 0 desocupado
    int numAviao;
    Assentos informacaoAssento[MAX];
}Informacao;

typedef struct{
    int codigo;
    char *nome;
    Informacao *viagem;
}Passageiro;

void realizarBusca(Passageiro *passageiro, char *buscarPassageiro);
int principalBusca(void);

#endif