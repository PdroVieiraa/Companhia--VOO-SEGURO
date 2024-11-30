#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_CODIGOS 100
#define ARQUIVO_BINARIO "tripulantes.bin"

/*---------------------------------------------------Ferramentas--------------------------------------------------------------*/

int obterOpcao() {
    int opcao;
    
    while (1) {
        printf("\nEscolha uma das opcoes (Entre 0 e 4): ");
        
        // Verificar se a entrada é um número válido
        if (scanf("%d", &opcao) == 1 && opcao >= 0 && opcao <= 4) {
            while (getchar() != '\n');  // Limpa o buffer após a leitura
            return opcao;  // Se for válido, retorna a opção
        } else {
            // Se não for válido, limpa o buffer e pede novamente
            while (getchar() != '\n');
            printf("Entrada inválida! Por favor, digite um número entre 0 e 4.\n");
        }
    }
}

/*---------------------------------------------------Funções---------------------------------------------------------------*/

typedef struct {
    char nome[60];    // Nome do tripulante
    int codigo;       // Código único do tripulante
    int telefone;     // Número de telefone do tripulante
    char cargo[20];   // Cargo do tripulante (piloto, copiloto, comissário)
} Tripulantes;

    Tripulantes tripulantes[MAX_CODIGOS];
    char *codigos_gerados[MAX_CODIGOS];
    int num_codigos = 0;
    int total_tripulantes = 0;

char *GerarNovoCodigo(void) {
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    int codigo = 0;

    // Gera 6 dígitos aleatórios
    for (int i = 0; i < 6; i++) {
        int digito = rand() % 10;  // Gera um dígito aleatório de 0 a 9
        codigo = codigo * 10 + digito;  // Concatena o dígito no código
    }

    // Aloca memória para o código gerado (6 dígitos + '\0' terminador)
    char *codigo_str = (char *)malloc(7 * sizeof(char));
    if (codigo_str == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    // Formata o código com 6 dígitos, garantindo que tenha zeros à esquerda se necessário
    sprintf(codigo_str, "%06d", codigo);
    return codigo_str;
}

int VerificarCodigoArquivo(int codigo) {
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
    tripulante->nome[strcspn(tripulante->nome, "\n")] = '\0';

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

    int cargo;
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
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "wb");  // Abre em modo escrita binária
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        exit(1);
    }

    fwrite(&num_tripulantes, sizeof(int), 1, arquivo);  // Salva o número total de tripulantes
    fwrite(tripulantes, sizeof(Tripulantes), num_tripulantes, arquivo);  // Salva a lista completa de tripulantes

    fclose(arquivo);
    printf("\nDados salvos com sucesso no arquivo binário!\n");
}

void AdicionarTripulante(Tripulantes *novo_tripulante) {
    Tripulantes tripulantes[MAX_CODIGOS];
    int num_tripulantes = 0;

    // Carregar tripulantes existentes
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo != NULL) {
        fread(&num_tripulantes, sizeof(int), 1, arquivo);  // Lê o número de tripulantes
        fread(tripulantes, sizeof(Tripulantes), num_tripulantes, arquivo);  // Lê a lista de tripulantes
        fclose(arquivo);
    }

    // Adicionar novo tripulante
    if (num_tripulantes < MAX_CODIGOS) {
        tripulantes[num_tripulantes] = *novo_tripulante;
        num_tripulantes++;

        // Salvar todos os tripulantes novamente
        SalvarTripulantes(tripulantes, num_tripulantes);
    } else {
        printf("Limite de tripulantes atingido!\n");
    }
}

void ExcluirTripulante(void) {
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

void ListarTripulantesRegistrados() {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo == NULL) {
        printf("Nenhum tripulante registrado ainda ou erro ao abrir o arquivo.\n");
        return;
    }

    int num_tripulantes;
    fread(&num_tripulantes, sizeof(int), 1, arquivo);  // Lê o número de tripulantes no início do arquivo

    if (num_tripulantes == 0) {
        printf("Nenhum tripulante registrado.\n");
    } else {
        printf("\n--- Lista de Tripulantes Registrados ---\n");
        Tripulantes tripulante;

        for (int i = 0; i < num_tripulantes; i++) {
            fread(&tripulante, sizeof(Tripulantes), 1, arquivo);  // Lê cada tripulante do arquivo
            printf("Código: %d\n", tripulante.codigo);
            printf("Nome: %s\n", tripulante.nome);
            printf("Telefone: %d\n", tripulante.telefone);
            printf("Cargo: %s\n", tripulante.cargo);
            printf("----------------------------------------\n");
        }
    }

    fclose(arquivo);

    printf("\nPressione Enter para voltar ao menu...");
    getchar();  // Aguarda o Enter
    printf("\n----------------------------------------\n");
}

void BuscarTripulante(Tripulantes *tripulantes, int total_tripulantes) {
    int opcao_busca;
    char termo[50];
    int encontrado = 0;

    printf("\nEscolha uma opção de busca:\n");
    printf("1 - Buscar por Nome\n");
    printf("2 - Buscar por Cargo\n");
    printf("3 - Buscar por Código\n");
    printf("Escolha uma opção (1-3): ");
    scanf("%d", &opcao_busca);
    getchar();  // Limpa o buffer

    switch(opcao_busca) {
        case 1:  // Buscar por Nome
            printf("Digite o nome do tripulante: ");
            fgets(termo, sizeof(termo), stdin);
            termo[strcspn(termo, "\n")] = '\0';  // Remove o \n deixado pelo fgets

            for (int i = 0; i < total_tripulantes; i++) {
                if (strstr(tripulantes[i].nome, termo) != NULL) {  // Busca se o nome contém o termo
                    printf("\nTripulante encontrado:\n");
                    ExibirTripulantes(tripulantes[i]);
                    encontrado = 1;
                    break;
                }
            }
            break;

        case 2:  // Buscar por Cargo
            printf("Digite o cargo do tripulante: ");
            fgets(termo, sizeof(termo), stdin);
            termo[strcspn(termo, "\n")] = '\0';  // Remove o \n deixado pelo fgets

            for (int i = 0; i < total_tripulantes; i++) {
                if (strstr(tripulantes[i].cargo, termo) != NULL) {  // Busca se o cargo contém o termo
                    printf("\nTripulante encontrado:\n");
                    ExibirTripulantes(tripulantes[i]);
                    encontrado = 1;
                    break;
                }
            }
            break;

        case 3:  // Buscar por Código
            printf("Digite o código do tripulante: ");
            int codigo;
            scanf("%d", &codigo);

            for (int i = 0; i < total_tripulantes; i++) {
                if (tripulantes[i].codigo == codigo) {  // Compara o código diretamente
                    printf("\nTripulante encontrado:\n");
                    ExibirTripulantes(tripulantes[i]);
                    encontrado = 1;
                    break;
                }
            }
            break;

        default:
            printf("Opção inválida! Escolha entre 1 e 3.\n");
            break;
    }

    if (!encontrado) {
        printf("\nNenhum tripulante encontrado com o critério informado.\n");
    }
}

/*-----------------------------------------------------------Funções unidads---------------------------------------------------------------------*/

void TRIPULANTES(){

    int opcao;
    Tripulantes tripulantes[MAX_CODIGOS];
    char *codigos_gerados[MAX_CODIGOS];
    int num_codigos = 0;
    int total_tripulantes = 0;

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

        while (getchar() != '\n');

        int continuar = 1;
        switch (opcao)
        {
        case 0:
            break;
        case 1:
           while (continuar && total_tripulantes < MAX_CODIGOS) {
        CadastroTripulantes(&tripulantes[total_tripulantes], codigos_gerados, &num_codigos);
        ExibirTripulantes(tripulantes[total_tripulantes]);
        total_tripulantes++;
        SalvarTripulantes(tripulantes, total_tripulantes);

        printf("\nDeseja cadastrar outro tripulante? (1 - Sim / 0 - Não): ");
        scanf("%d", &continuar);
        getchar();
    } break;
        case 2:
            ListarTripulantesRegistrados("tripulantes.bin");
            break;
        case 3:
            BuscarTripulante(tripulantes, total_tripulantes);
            break;
        case 4:
            ExcluirTripulante();
            break;
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
}

/*-----------------------------------------------------------------MAIN--------------------------------------------------------------------------*/

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
            //PASSAGEIROS();
            break;
        case 3:
            //VOOS();
            break;
        case 4:
            //RESERVAS();
            break;
        case 5:
            //FIDELIDADE();
            break;
        default:
            printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    
    } while (opcao != 0);
    

    return 0;
}
