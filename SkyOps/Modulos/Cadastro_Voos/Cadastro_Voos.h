#ifndef VOOS_H
#define VOOS_H

#define MAX_VOOS 100
#define MAX_TRIPULACAO 5

typedef struct {
    char nome[50];
    char funcao[20]; // "Piloto", "Copiloto", "Comissário"
} Tripulacao;

typedef struct {
    char codigo[10];
    char data[11];    // Formato: DD/MM/AAAA
    char hora[6];     // Formato: HH:MM
    char origem[50];
    char destino[50];
    float tarifa;
    Tripulacao tripulacao[MAX_TRIPULACAO];
    int qtd_tripulacao;
    char aviao[50];
} Voo;


int validarData(const char *data);
int validarHora(const char *hora);
int validarTarifa(float tarifa);
int codigoJaExiste(Voo voos[], int qtd_voos, const char *codigo);
void salvarVoos(Voo voos[], int qtd_voos);
void carregarVoos(Voo voos[], int *qtd_voos);
void adicionarVoo(Voo voos[], int *qtd_voos);
void listarVoos(Voo voos[], int qtd_voos);


#endif

