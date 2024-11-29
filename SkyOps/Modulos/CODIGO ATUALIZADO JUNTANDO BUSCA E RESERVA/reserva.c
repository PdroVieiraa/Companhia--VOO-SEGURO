#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX 200
#define MIN_TEMPO 1000
#define MAX_TEMPO 5000

//preciso fazer com que  sistema reserve um voo para um passageiro, e mostrar os voos, cadeiras apartir do destino do passageiro//

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

void realizarBusca(Passageiro *passageiro){
    char *buscaPassageiro;

    buscaPassageiro = (char*)malloc(MAX * sizeof(char));

    if(buscaPassageiro == NULL){
        printf("Erro na alocacao de memoria das informaçoes dos passageiros");
        return; //Retornando erro
    }

    printf("Faremos a busca de suas viagens: \n\n\n");

    printf("Informe seu nome: ");
    fgets(buscaPassageiro, MAX, stdin);

    buscaPassageiro[strcspn(buscaPassageiro, "\n")] = '\0';

    int encontrado = 0;
    
    for(int i = 0; i < MAX; i++){
        if(strcmp(buscaPassageiro, passageiro[i].nome) == 0){
            encontrado = 1;

            if (passageiro[i].viagem != NULL) {
                printf("Viagem associada:\n");
                printf("  Avião: %d\n", passageiro[i].viagem->numAviao);
                printf("  Origem: %s\n", passageiro[i].viagem->origemdoaviao);
                printf("  Destino: %s\n", passageiro[i].viagem->destinodoaviao);
            } else {
                printf("Nenhuma viagem associada.\n");
            }
            break;
        }
    }

    if(!encontrado){
        printf("Passageiro nao encontrado.\n");
    }

    free(buscaPassageiro);
}

void reserva(Informacao *aviao, int numAviao){
    char *destino, *origem, *nomePassageiro;
    int assentoEscolhido;


    destino = (char*)malloc(MAX * sizeof(char));

    if(destino == NULL){
        printf("Erro ao realizar a alocacao de memoria do DESTINO");
        return;
    }

    origem = (char*)malloc(MAX * sizeof(char));

    if(origem == NULL){
        printf("ERRO ao realizar a alocacao de memoria da ORIGEM");
        return;
    }

    nomePassageiro = (char*)malloc(MAX * sizeof(char));

    if(nomePassageiro == NULL){
        printf("ERRO ao realizar a alocacao de memoria do Nome de Passageiro");
        return;
    }

    printf("Informe sua ORIGEM: ");
    fgets(origem, MAX, stdin);

    origem[strcspn(origem, "\n")] = '\0';

    printf("Informe seu DESTINO: ");
    fgets(destino, MAX, stdin);

    destino[strcspn(destino, "\n")] = '\0';
    origem[strcspn(origem, "\n")] = '\0';

    for(int i = 0; i < MAX; i++){
        destino[i] = tolower(destino[i]);
        origem[i] = tolower(origem[i]);
    }

 for (int i = 0; i < numAviao; i++) {
        if (strcmp(aviao[i].origemdoaviao, origem) == 0 &&
            strcmp(aviao[i].destinodoaviao, destino) == 0) {
            printf("Aviao %d encontrado!\n", aviao[i].numAviao);
            printf("Assentos disponiveis:\n");

            // Lista assentos disponíveis
            for (int j = 0; j < MAX; j++) {
                if (aviao[i].assentos[0][j] == 0) {
                    printf("Assento %d\n", j);
                }
            }

            printf("Informe o número do assento que deseja reservar: ");
            scanf("%d", &assentoEscolhido);

            // Verifica disponibilidade do assento
            if (aviao[i].assentos[0][assentoEscolhido] == 0) {
            printf("Informe seu nome: ");
            getchar(); // Consome quebra de linha pendente
            fgets(nomePassageiro, 100, stdin);
            nomePassageiro[strcspn(nomePassageiro, "\n")] = '\0';

            // Atualiza reserva
            aviao[i].assentos[0][assentoEscolhido] = 1;
            aviao[i].informacaoAssento[assentoEscolhido].numeroAssento = assentoEscolhido;
            strcpy(aviao[i].informacaoAssento[assentoEscolhido].nomePassageiro, nomePassageiro);

            printf("Reserva feita com sucesso! Assento %d reservado para %s.\n",
                    assentoEscolhido, nomePassageiro);
            return;
        } else {
            printf("Assento já ocupado. Tente outro!\n");
        }
    }
}

void cancelarReserva(Informacao *aviao, int numAvioes){
    int numAviao, linha, coluna;

    printf("Informe o numero do seu aviao: ");
    scanf("%d", &numAviao);

    if(numAviao < 0 || numAviao >= numAvioes){
        printf("Informe o numero do seu aviao corretamente!");
        return;
    }

    printf("Informe o numero da linha do seu assento: (0 ou 1)");
    scanf("%d", &linha);

    printf("Informe o numero da coluna do seu assento: (0 a %d)", numAvioes);
    scanf("%d", &coluna);

        if (linha < 0 || linha >= 2 || coluna < 0 || coluna >= MAX) {
        printf("Assento inválido.\n");
        return;
    }

    if(aviao[numAviao].assentos[linha][coluna] == 1){
        aviao[numAviao].assentos[linha][coluna] = 0;
        printf("Reserva cancelada com sucesso!!");
    }else{
        printf("O assento informado esta disponivel!");
    }
}

int principalReserva(){
    srand(time(NULL));

    Informacao *aviao;
    
    aviao = (Informacao*)malloc(MAX*sizeof(Informacao));

    if(aviao == NULL){
        printf("ERRO ao realizar a alocacao de memoria da STRUCT");
        return 1; //retorna 1 como ERRO
    }

    for(int i = 0; i < MAX; i++){
        aviao[i].origemdoaviao = (char*)malloc(MAX*sizeof(char));

        if(aviao[i].origemdoaviao == NULL){
            printf("ERRO ao realizar a alocacao de memoria da ORIGEM do aviao na STRUCT");
            return 1; //retorna 1 como ERRO
        }

        aviao[i].destinodoaviao = (char*)malloc(MAX*sizeof(char));
    
        if(aviao[i].destinodoaviao == NULL){
            printf("ERRO realizar a alocacao de memoria do DESTINO do aviao na STRUCT");
            return 1; //retorna 1 como ERRO
        }
    }

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

    int opcao;

    printf("Bem vindo a aba de RESERVA da sua viagem\n\n\n");

    do{
    printf("Oque você deseja fazer: \n\n");
    printf("[1] Realisar reserva de viagem; \n[2] Realisar o cancelamento da sua reserva; \n[3] Realisar busca; \n[0] Sair \n\n");
    scanf("%d", &opcao);

    switch (opcao) {
            case 1:
                reserva(aviao, MAX);
                break;
            case 2:
                cancelarReserva(aviao, MAX);
                break;
            case 3:
                realizarBusca(passageiro);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    for(int j = 0; j< MAX; j++){
        free(aviao[j].destinodoaviao);
        free(aviao[j].origemdoaviao);
    }

    for (int i = 0; i < MAX; i++) {
        free(passageiro[i].nome);
    }
    
    free(passageiro);
    free(aviao);

    return 0;
}