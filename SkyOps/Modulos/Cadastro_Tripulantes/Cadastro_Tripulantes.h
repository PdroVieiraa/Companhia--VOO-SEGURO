#ifndef TRIPULANTES_H
#define TRIPULANTES_H

typedef struct {
    char nome[60];
    int codigo;
    int telefone;
    char cargo[20];
} Tripulantes;

char *GerarNovoCodigo(void);
int VerificarCodigoArquivo(int codigo);
int VerificarCodigo(char *novo_codigo, char *codigos_gerados[], int num_codigos);
void CadastroTripulantes(Tripulantes *tripulante, char *codigos_gerados[], int *num_codigos);
void ExibirTripulantes(Tripulantes tripulante);
void SalvarTripulantes(Tripulantes *tripulantes, int num_tripulantes);
void ExcluirTripulante(void);

#endif