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

int main(){
    srand(time(NULL));

    Informacao *i;
    
    i = (Informacao*)malloc(MAX*sizeof(Informacao));

    if(i == NULL){
        printf("ERRO ao realizar a alocacao de memoria da STRUCT");
        return 1; //retorna 1 como ERRO
    }

    for(int j = 0; j < MAX; j++){
        i[j].origemdoaviao = (char*)malloc(MAX*sizeof(char));

        if(i[j].origemdoaviao == NULL){
            printf("ERRO ao realizar a alocacao de memoria da ORIGEM do aviao na STRUCT");
            return 1; //retorna 1 como ERRO
        }

        i[j].destinodoaviao = (char*)malloc(MAX*sizeof(char));
    
        if(i[j].destinodoaviao == NULL){
            printf("ERRO realizar a alocacao de memoria do DESTINO do aviao na STRUCT");
            return 1; //retorna 1 como ERRO
        }
    }

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

    printf("Bem vindo a aba de RESERVA da sua viagem\n\n\n");

    //apartir daqui o usuario deve informar seu destino para encontrar um avião disponivel

    printf("Informe sua ORIGEM: ");
    fgets(origem, MAX, stdin);

    origem[strcspn(origem, "\n")] = '\0';

    printf("Informe seu DESTINO: ");
    fgets(destino, MAX, stdin);

    destino[strcspn(destino, "\n")] = '\0';
    origem[strcspn(origem, "\n")] = '\0';

    for(int j = 0; j < MAX; j++){
        destino[j] = tolower(destino[j]);
        origem[j] = tolower(destino[j]);
    }

    for(int j = 0; j < MAX; j++){
        if(strcmp(origem, i[j].origemdoaviao) == 0){
            if(strcmp(destino, i[j].destinodoaviao) == 0){
                printf("ASSENTOS LIBERADOS:\n\n");
                for(int l = 0; l < 2; l++){
                    for(int c = 0; c < MAX; c++){
                        if(i->assentos[l][c] == 0){
                            printf("Assento %d\n", i->assentos);
                        }
                    }
                }
            }
        }
    }

    for(int j = 0; j< MAX; j++){
        free(i[j].destinodoaviao);
        free(i[j].origemdoaviao);
    }
    free(origem);
    free(destino);
}