#ifndef RESERVAS_H
#define RESERVAS_H

#define MAX 200
#define MIN_TEMPO 1000
#define MAX_TEMPO 5000

typedef struct{
    char *origemdoaviao; //deixar como padrao tudo minusculo
    char *destinodoaviao; //deixar como padrao tudo minusculo
    int assentos[2][MAX]; //1 ocupado e 0 desocupado
    int numAviao;
}Informacao;

void reserva(Informacao *aviao, int numAviao);
void cancelarReserva(Informacao *aviao, int numAvioes);
void principalRESERVA();


#endif