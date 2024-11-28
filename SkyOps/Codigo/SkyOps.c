#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


void TRIPULANTES(){

    int opcao;


    do
    {


        printf("\tTripulantes\n");
        printf("\n");
        printf("1 - Cadastrar tripulante\n");
        printf("2 - Lista de tripulantes\n");
        printf("3 - Buscar tripulantes\n");
        printf("4 - Deletar tripulante\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\n");
         printf("\nEscolha uma das opcoes (Entre 0 e 4): ");
        scanf("%d", &opcao);
        printf("\n");


        switch (opcao)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
    

}



void PASSAGEIROS(){
    
    int opcao;


    do
    {


        printf("\tPassageiros\n");
        printf("\n");
        printf("1 - Cadastrar passageiro\n");
        printf("2 - Lista de passageiros\n");
        printf("3 - Buscar passageiros\n");
        printf("4 - Deletar passageiro\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\n");
         printf("\nEscolha uma das opcoes (Entre 0 e 4: ");
        scanf("%d", &opcao);
        printf("\n");


        switch (opcao)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
    

}

void VOOS(){
    
    int opcao;


    do
    {


        printf("\tVoos\n");
        printf("\n");
        printf("1 - Cadastrar Voo\n");
        printf("2 - Lista de Voos\n");
        printf("3 - Buscar Voos\n");
        printf("4 - Cancelar Voos\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\n");
         printf("\nEscolha uma das opcoes (Entre 0 e 4): ");
        scanf("%d", &opcao);
        printf("\n");


        switch (opcao)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
    

}

void RESERVAS(){
    
    int opcao;


    do
    {


        printf("\tReservas\n");
        printf("\n");
        printf("1 - Criar reservas\n");
        printf("2 - Lista de reservas\n");
        printf("3 - Buscar reservas\n");
        printf("4 - Cancelar reservas\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\n");
         printf("\nEscolha uma das opcoes (Entre 0 e 4): ");
        scanf("%d", &opcao);
        printf("\n");


        switch (opcao)
        {
        case 0:
            break;
        case 1:

            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
    

}

void FIDELIDADE(){
    
    int opcao;


    do
    {


        printf("\tPlano de fidelidade\n");
        printf("\n");
        printf("1 - Adicionar plano de fidelidade\n");
        printf("2 - Consultar plano de fidelidade\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\n");
         printf("\nEscolha uma das opcoes (Entre 0 e 2): ");
        scanf("%d", &opcao);
        printf("\n");


        switch (opcao)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
         printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
    

}

int main() {

   
    int opcao;

    
    do
    {
    printf("\n____________________________________\n");
    printf("\t\tSKYOPS\n");

    printf("\n");
    printf("1 - Tripulantes\n");
    printf("2 - Passageiros\n");
    printf("3 - Voos\n");
    printf("4 - Reservas\n");
    printf("5 - Programa fidelidade\n");
    printf("0 - SAIR\n");

    printf("\nEscolha uma das opcoes (Entre 0 e 5): ");
    scanf("%d", &opcao);
    printf("\n");



        switch (opcao)
        {
        case 0:
            printf("\nSaindo...\n");;
            break;
        case 1:
            TRIPULANTES();
            break;
        case 2:
            PASSAGEIROS();
            break;
        case 3:
            VOOS();
            break;
        case 4:
            RESERVAS();
            break;
        case 5:
            FIDELIDADE();
            break;
        default:
            printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    
    } while (opcao != 0);
    

    return 0;
}