#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USUARIOS 100
#define ARQUIVO_FIDELIDADE "fidelidade.bin"

// Estrutura para armazenar dados do usuário
typedef struct {
    char id[20];
    char nome[50];
    int pontos; // Pontos de fidelidade
} Usuario;

Usuario usuarios[MAX_USUARIOS];
int total_usuarios = 0;

// Função para carregar os dados do arquivo
void carregarUsuarios() {
    FILE *arquivo = fopen(ARQUIVO_FIDELIDADE, "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de fidelidade encontrado. Iniciando sistema vazio.\n");
        return;
    }
    fread(&total_usuarios, sizeof(int), 1, arquivo);
    fread(usuarios, sizeof(Usuario), total_usuarios, arquivo);
    fclose(arquivo);
    printf("Dados de fidelidade carregados com sucesso!\n");
}

// Função para salvar os dados no arquivo
void salvarUsuarios() {
    FILE *arquivo = fopen(ARQUIVO_FIDELIDADE, "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar os dados");
        return;
    }
    fwrite(&total_usuarios, sizeof(int), 1, arquivo);
    fwrite(usuarios, sizeof(Usuario), total_usuarios, arquivo);
    fclose(arquivo);
    printf("Dados de fidelidade salvos com sucesso!\n");
}

// Função para verificar se um ID já existe
int idJaExiste(const char *id) {
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].id, id) == 0) {
            return i; // Retorna o índice do usuário encontrado
        }
    }
    return -1; // Não encontrado
}

// Função para validar o nome (somente letras e espaços)
int validarNome(const char *nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ') {
            return 0; // Caracter inválido encontrado
        }
    }
    return 1; // Nome válido
}

// Função para validar o ID (somente números)
int validarId(const char *id) {
    for (int i = 0; id[i] != '\0'; i++) {
        if (!isdigit(id[i])) {
            return 0; // Caracter inválido encontrado
        }
    }
    return 1; // ID válido
}

// Função para cadastrar um novo usuário
void cadastrarUsuario() {
    if (total_usuarios >= MAX_USUARIOS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    Usuario novoUsuario;

    do {
        printf("Digite o ID único do usuário (somente números): ");
        scanf("%s", novoUsuario.id);
        if (!validarId(novoUsuario.id)) {
            printf("ID inválido! Use apenas números.\n");
            continue;
        }
        if (idJaExiste(novoUsuario.id) != -1) {
            printf("ID já existe! Por favor, insira um ID único.\n");
        }
    } while (!validarId(novoUsuario.id) || idJaExiste(novoUsuario.id) != -1);

    do {
        printf("Digite o nome do usuário (somente letras): ");
        scanf(" %[^\n]", novoUsuario.nome);
        if (!validarNome(novoUsuario.nome)) {
            printf("Nome inválido! Use apenas letras e espaços.\n");
        }
    } while (!validarNome(novoUsuario.nome));

    novoUsuario.pontos = 0; // Inicializa com 0 pontos
    usuarios[total_usuarios++] = novoUsuario;

    printf("Usuário cadastrado com sucesso!\n");
    salvarUsuarios(); // Salvar dados atualizados
}

// Função para adicionar pontos fidelidade
void adicionarPontos() {
    char id[20];
    printf("Digite o ID do usuário: ");
    scanf("%s", id);

    int indice = idJaExiste(id);
    if (indice == -1) {
        printf("Usuário não encontrado!\n");
        return;
    }

    usuarios[indice].pontos += 10; // Adiciona 10 pontos
    printf("10 pontos adicionados ao usuário %s (%s).\n", usuarios[indice].nome, usuarios[indice].id);
    salvarUsuarios(); // Salvar dados atualizados
}

// Função para consultar pontos de fidelidade
void consultarPontos() {
    char id[20];
    printf("Digite o ID do usuário: ");
    scanf("%s", id);

    int indice = idJaExiste(id);
    if (indice == -1) {
        printf("Usuário não encontrado!\n");
        return;
    }

    printf("Usuário: %s (%s)\n", usuarios[indice].nome, usuarios[indice].id);
    printf("Pontos acumulados: %d\n", usuarios[indice].pontos);
}

// Menu principal
int main() {
    carregarUsuarios(); // Carregar os dados ao iniciar o programa

    int opcao;

    do {
        printf("\n--- Programa de Fidelidade ---\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Adicionar pontos\n");
        printf("3. Consultar pontos\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarUsuario();
                break;
            case 2:
                adicionarPontos();
                break;
            case 3:
                consultarPontos();
                break;
            case 4:
                salvarUsuarios();
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 4);

    return 0;
}
