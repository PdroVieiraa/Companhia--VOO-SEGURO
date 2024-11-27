#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 200

//preciso fazer uma busca apartir do nome ou codigo do passageiro, apartir disso informar e listar as viagens do mesmo

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

void realizarBusca(Passageiro *passageiro, char *buscarPassageiro){
    for(int i = 0; i < MAX; i++){
        if(strcmp(buscarPassageiro, passageiro[i].nome) == 0){
            if (passageiro[i].viagem != NULL) {
                printf("Viagem associada:\n");
                printf("  Avião: %d\n", passageiro[i].viagem->numAviao);
                printf("  Origem: %s\n", passageiro[i].viagem->origemdoaviao);
                printf("  Destino: %s\n", passageiro[i].viagem->destinodoaviao);
            } else {
                printf("Nenhuma viagem associada.\n");
            }
        }
    }
}

int principalBusca(){
    Passageiro *passageiro;

    passageiro = (Passageiro*)malloc(MAX * sizeof(Passageiro));

    if(passageiro == NULL){
        printf("Erro na alocacao de memoria dos passageiros");
        return 1; //Retornando erro
    }

    for(int i = 0; i < MAX; i++){
        passageiro[i].nome = (char*)malloc(MAX * sizeof(char));

        if(passageiro[i].nome == NULL){
            printf("Erro na alocacao de memoria do nome dos passageiros");
            return 1; //Retornando erro
        }
    }

    char *buscaPassageiro;

    buscaPassageiro = (char*)malloc(MAX * sizeof(char));

    if(buscaPassageiro == NULL){
        printf("Erro na alocacao de memoria das informaçoes dos passageiros");
        return 1; //Retornando erro
    }

    printf("Faremos a busca de suas viagens: \n\n\n");

    printf("Informe seu nome: ");
    fgets(buscaPassageiro, MAX, stdin);

    buscaPassageiro[strcspn(buscaPassageiro, "\n")] = '\0';

    realizarBusca(passageiro, buscaPassageiro);

    for (int i = 0; i < MAX; i++) {
        if (passageiro[i].viagem != NULL) {
            free(passageiro[i].viagem);
        }
    }

    return 0;
}