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
    char *origemdoaviao; //deixar como padrao tudo minusculo
    char *destinodoaviao; //deixar como padrao tudo minusculo
    int assentos[2][MAX]; //1 ocupado e 0 desocupado
    int numAviao;
}Informacao;


void reserva(Informacao *aviao, int numAviao){
    char *destino, *origem;

    destino = (char*)malloc(MAX * sizeof(char));

    if(destino == NULL){
        printf("Erro ao realizar a alocacao de memoria do DESTINO");
        return 1; //retorna 1 como ERRO
    }

    origem = (char*)malloc(MAX * sizeof(char));

    if(origem == NULL){
        printf("ERRO ao realizar a alocacao de memoria da ORIGEM");
        return 1; //retorna 1 como ERRO
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

    for(int i = 0; i < MAX; i++){
        if(strcmp(origem, aviao[i].origemdoaviao) == 0){
            if(strcmp(destino, aviao[i].destinodoaviao) == 0){
                printf("ASSENTOS LIBERADOS:\n\n");
                for(int j = 0; j < 2; j++){
                    for(int k = 0; k < MAX; k++){
                        if(aviao->assentos[j][k] == 0){
                            printf("Assento %d\n", aviao->assentos[j][k]);
                        }
                    }
                }
            }
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
    scanf("%d", linha);

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

int principalRESERVA(){
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

    int opcao;

    printf("Bem vindo a aba de RESERVA da sua viagem\n\n\n");

    do{
    printf("Oque você deseja fazer: \n\n");
    printf("[1] Realisar reserva de viagem; \n[2] Realisar o cancelamento da sua reserva; \n[0] Sair \n\n");
    scanf("%d", &opcao);

    switch (opcao) {
            case 1:
                reserva(aviao, MAX);
                break;
            case 2:
                cancelarReserva(aviao, MAX);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    //apartir daqui o usuario deve informar seu destino para encontrar um avião disponivel


    for(int j = 0; j< MAX; j++){
        free(aviao[j].destinodoaviao);
        free(aviao[j].origemdoaviao);
    }


    return 0;
}
