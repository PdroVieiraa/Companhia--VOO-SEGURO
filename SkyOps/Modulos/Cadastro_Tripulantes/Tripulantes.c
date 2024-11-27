/*Adicionar membros com cargos específicos. Garantir códigos únicos e permitir geração automática. Salvar dados em arquivos binários
TRIPULAÇÃO: código, nome, telefone, cargo (piloto, copiloto, comissário)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_CODIGOS 100
#define ARQUIVO_BINARIO "tripulantes.bin"

typedef struct {
    char nome[60];
    int codigo;
    int telefone;
    char cargo[20];
} Tripulantes;

char *GerarNovoCodigo(void) {

    int codigo = 0;

    // Gera 6 dígitos
    for (int i = 0; i < 6; i++) {
        int digito = rand() % 10;
        codigo = codigo * 10 + digito;
    }

    char *codigo_str = (char *)malloc(7 * sizeof(char));
    if (codigo_str == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    sprintf(codigo_str, "%06d", codigo);
    return codigo_str;
}

int VerificarCodigoArquivo(int codigo) { // Usando int diretamente para consistência
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo == NULL) return 0;

    int num_tripulantes;
    fread(&num_tripulantes, sizeof(int), 1, arquivo);

    Tripulantes tripulante;
    for (int i = 0; i < num_tripulantes; i++) {
        fread(&tripulante, sizeof(Tripulantes), 1, arquivo);
        if (tripulante.codigo == codigo) {
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

int VerificarCodigo(char *novo_codigo, char *codigos_gerados[], int num_codigos) {
    for (int i = 0; i < num_codigos; i++) {
        if (strcmp(novo_codigo, codigos_gerados[i]) == 0) {
            return 1;
        }
    }
    return VerificarCodigoArquivo(atoi(novo_codigo)); // Conversão única aqui
}

void CadastroTripulantes(Tripulantes *tripulante, char *codigos_gerados[], int *num_codigos) {
   setlocale(LC_ALL, "Portuguese_Brazil.1252");


    char codigo[7];
    int codigo_unico = 0;

    printf("Cadastro de Tripulante");
    printf("\n-------------------------------\n");

    printf("Nome: ");
    fgets(tripulante->nome, sizeof(tripulante->nome), stdin);
    tripulante->nome[strcspn(tripulante->nome, "\n")] = '\0'; // Remove o '\n'

    while (!codigo_unico) {
        char *novo_codigo = GerarNovoCodigo();
        codigo_unico = !VerificarCodigo(novo_codigo, codigos_gerados, *num_codigos);

        if (codigo_unico) {
            strcpy(codigo, novo_codigo);
        }
        free(novo_codigo);
    }

    tripulante->codigo = atoi(codigo);
    codigos_gerados[*num_codigos] = strdup(codigo);
    (*num_codigos)++;

    printf("Código (Gerado automaticamente): %d\n", tripulante->codigo);

    printf("Telefone (ddd9xxxxxxxx): ");
    scanf("%d", &tripulante->telefone);

    int cargo = 0;
    do {
        printf("Cargo:\n 1 - Piloto(a)\n 2 - Copiloto(a)\n 3 - Comissário(a)\n");
        scanf("%d", &cargo);
        getchar();

        switch (cargo) {
            case 1:
                strcpy(tripulante->cargo, "Piloto(a)");
                break;
            case 2:
                strcpy(tripulante->cargo, "Copiloto(a)");
                break;
            case 3:
                strcpy(tripulante->cargo, "Comissário(a)");
                break;
            default:
                printf("Opção inválida! Escolha entre 1 e 3.\n");
                cargo = 0;
                break;
        }
    } while (cargo == 0);
}

void ExibirTripulantes(Tripulantes tripulante) {

   setlocale(LC_ALL, "Portuguese_Brazil.1252");


    printf("\n--- Dados do Tripulante ---\n");
    printf("Nome: %s\n", tripulante.nome);
    printf("Código: %06d\n", tripulante.codigo);
    printf("Telefone: %d\n", tripulante.telefone);
    printf("Cargo: %s\n", tripulante.cargo);
}

void SalvarTripulantes(Tripulantes *tripulantes, int num_tripulantes) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        exit(1);
    }

    fwrite(&num_tripulantes, sizeof(int), 1, arquivo);  // Salva o número de tripulantes
    fwrite(tripulantes, sizeof(Tripulantes), num_tripulantes, arquivo);  // Salva os tripulantes

    fclose(arquivo);
    printf("\nDados salvos com sucesso no arquivo binário!\n");
}

void ExcluirTripulantes(void) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura! Nenhum tripulante encontrado.\n");
        return;
    }

    int num_tripulantes;
    fread(&num_tripulantes, sizeof(int), 1, arquivo);

    if (num_tripulantes == 0) {
        printf("Nenhum tripulante cadastrado.\n");
        fclose(arquivo);
        return;
    }

    Tripulantes *tripulantes = (Tripulantes *)malloc(num_tripulantes * sizeof(Tripulantes));
    if (tripulantes == NULL) {
        printf("Erro de alocação de memória!\n");
        fclose(arquivo);
        return;
    }

    fread(tripulantes, sizeof(Tripulantes), num_tripulantes, arquivo);
    fclose(arquivo);

    int codigo;
    printf("\nDigite o código do tripulante a ser excluído: ");
    scanf("%d", &codigo);

    int encontrado = 0;
    for (int i = 0; i < num_tripulantes; i++) {
        if (tripulantes[i].codigo == codigo) {
            encontrado = 1;

            // Remove o tripulante deslocando os próximos para preencher o espaço
            for (int j = i; j < num_tripulantes - 1; j++) {
                tripulantes[j] = tripulantes[j + 1];
            }

            num_tripulantes--; // Diminui a contagem de tripulantes
            break;
        }
    }

    if (!encontrado) {
        printf("Tripulante com código %06d não encontrado.\n", codigo);
    } else {
        // Reabre o arquivo para escrita e salva os novos dados
        arquivo = fopen(ARQUIVO_BINARIO, "wb");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo para escrita!\n");
            free(tripulantes);
            return;
        }

        fwrite(&num_tripulantes, sizeof(int), 1, arquivo); // Atualiza o número de tripulantes
        fwrite(tripulantes, sizeof(Tripulantes), num_tripulantes, arquivo);
        fclose(arquivo);

        printf("Tripulante com código %06d excluído com sucesso.\n", codigo);
    }

    free(tripulantes); // Libera a memória alocada
}




int main() {
   setlocale(LC_ALL, "Portuguese_Brazil.1252");


    Tripulantes tripulantes[MAX_CODIGOS];
    char *codigos_gerados[MAX_CODIGOS];
    int num_codigos = 0;
    int total_tripulantes = 0;

    int continuar = 1;
    while (continuar && total_tripulantes < MAX_CODIGOS) {
        CadastroTripulantes(&tripulantes[total_tripulantes],codigos_gerados, &num_codigos);
        ExibirTripulantes(tripulantes[total_tripulantes]);
        total_tripulantes++;
        SalvarTripulantes(tripulantes, total_tripulantes);

        printf("\nDeseja cadastrar outro tripulante? (1 - Sim / 0 - Não): ");
        scanf("%d", &continuar);
        getchar();
    }
    

    for (int i = 0; i < num_codigos; i++) {
        free(codigos_gerados[i]);
    }

    return 0;
}