//Demitir piloto
//Arquivos

#ifndef FIDELIDADE_H
#define FIDELIDADE_H

#define MAX_USUARIOS 100
#define ARQUIVO_FIDELIDADE "fidelidade.bin"

    typedef struct {
    char id[20];
    char nome[50];
    int pontos; // Pontos de fidelidade
} Usuario;

Usuario usuarios[MAX_USUARIOS];

void carregarUsuarios(void);
void salvarUsuarios(void);
int idJaExiste(const char *id);
int validarNome(const char *nome);
int validarId(const char *id);
int cadastrarUsuario(void);
void consultarPontos(void);
void adicionarPontos(void);


#endif