#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>


#define MAX 200
#define MIN_TEMPO 1000
#define MAX_TEMPO 5000
#define MAX_VOOS 100
#define MAX_VOOS 100
#define MAX_TRIPULACAO 5
#define MAX_CODIGOS 100
#define MAX_PASSAGEIROS 100
#define MAX_PASSAGEIROS 100
#define TOTAL_ASSENTOS 100
#define ARQUIVO_VOOS "voos.bin"
#define ARQUIVO_BINARIO "tripulantes.bin"
#define ARQUIVO_PASSAGEIROS "passageiros.bin"
#define ARQUIVO_FIDELIDADE "fidelidade_manual.bin"

typedef struct {
    char nome[100];    // Nome do tripulante
    int codigo;       // Código único do tripulante
    int telefone;     // Número de telefone do tripulante
    char cargo[20];   // Cargo do tripulante (piloto, copiloto, comissário)
} Tripulantes;

typedef struct {
    int codigo;           
    char nome[100];       
    char endereco[150];
    char telefone[20];
    int fidelidade;       
    int pontos;           
} Passageiro;

typedef struct {
    int numero;
    int status; // 0 = desocupado, 1 = ocupado
    Passageiro nomePassageiro; // PUXAR O NOME PELA STRUCT "Passageiro" = nomePassageiro.nome
} Assento;

typedef struct {
    char codigo[10];
    char data[11];    // Formato: DD/MM/AAAA
    char hora[6];     // Formato: HH:MM
    char origem[50];
    char destino[50];
    float tarifa;
    Tripulantes tripulacao[MAX_TRIPULACAO];
    int qtd_tripulacao;
    char aviao[50];
    Assento assentos[MAX];
} Voo;

typedef struct {
    Passageiro *passageirros;
} Fidelidade;

typedef struct {
    Voo origem[MAX];  // Strings fixas origem.origem
    Voo destino[MAX];   //             destino.destino
    int numAviao;
    Assento informacaoAssento[MAX];
} Informacao;


Passageiro passageiros[MAX_PASSAGEIROS];
int total_passageiros = 0;

Voo voos[MAX_VOOS];
int total_voos = 0;

Tripulantes tripulantes[MAX_CODIGOS];
char *codigos_gerados[MAX_CODIGOS];
int num_codigos = 0;
int total_tripulantes = 0;

Assento assentosDetalhes[TOTAL_ASSENTOS];



/*---------------------------------------------------------------Ferramentas--------------------------------------------------------------*/

void ConverterPrimeiraMaiuscula(char *str) {
    int nova_palavra = 1;  // Variável para controlar quando é o início de uma nova palavra

    for (int i = 0; str[i] != '\0'; i++) {
        if (nova_palavra && isalpha((unsigned char)str[i])) {
            str[i] = toupper((unsigned char)str[i]);  // Primeira letra da palavra em maiúscula
            nova_palavra = 0;
        } else {
            str[i] = tolower((unsigned char)str[i]);  // O restante das letras em minúscula
        }

        if (isspace((unsigned char)str[i])) {
            nova_palavra = 1;  // Se encontrar um espaço, a próxima letra será a primeira da próxima palavra
        }
    }
}

/*----------------------------------------------------------------TRIPULANTES---------------------------------------------------------------*/

char *GerarNovoCodigo(void) {
    srand(time(NULL));  // Inicializa a semente do gerador de números aleatórios

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

void CadastroTripulantes(Tripulantes *tripulante) {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    char codigo[7];

    printf("Cadastro de Tripulante");
    printf("\n-------------------------------\n");

    printf("Nome: ");
    fgets(tripulante->nome, sizeof(tripulante->nome), stdin);
    tripulante->nome[strcspn(tripulante->nome, "\n")] = '\0';
    ConverterPrimeiraMaiuscula(tripulante->nome);

    // Gera o código aleatório
    char *novo_codigo = GerarNovoCodigo();
    strcpy(codigo, novo_codigo);
    free(novo_codigo);  // Libera a memória alocada

    tripulante->codigo = atoi(codigo);

    printf("Código (Gerado automaticamente): %d\n", tripulante->codigo);

    printf("Telefone (ddd9xxxxxxxx): ");
    scanf("%d", &tripulante->telefone);
    getchar();  // Consumir nova linha residual do buffer após scanf

    int cargo;
    do {
        printf("Cargo:\n 1 - Piloto(a)\n 2 - Copiloto(a)\n 3 - Comissário(a)\n");
        scanf("%d", &cargo);
        getchar();  // Consumir nova linha residual

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
    
    printf("Cadastro salvo com sucesso!\n");
}

// Função para salvar tripulante no arquivo binário
void SalvarTripulante(Tripulantes *tripulante) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "ab");  // Abrir para adicionar ao final
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Escreve o tripulante no arquivo binário
    fwrite(tripulante, sizeof(Tripulantes), 1, arquivo);

    fclose(arquivo);
}

// Função para ler todos os tripulantes do arquivo binário
void LerTripulantes() {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    Tripulantes tripulante;
    while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
        printf("\nNome: %s\n", tripulante.nome);
        printf("Código: %d\n", tripulante.codigo);
        printf("Telefone: %d\n", tripulante.telefone);
        printf("Cargo: %s\n", tripulante.cargo);
        printf("-----------------------------\n");
    }

    fclose(arquivo);
}

// Função para excluir um tripulante com base no código
void ExcluirTripulante(int codigo) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Lê os tripulantes existentes
    Tripulantes tripulantes[100];
    int num_tripulantes = 0;
    while (fread(&tripulantes[num_tripulantes], sizeof(Tripulantes), 1, arquivo) == 1) {
        num_tripulantes++;
    }
    fclose(arquivo);

    // Cria um novo arquivo sem o tripulante a ser excluído
    arquivo = fopen(ARQUIVO_BINARIO, "wb");  // Abre o arquivo para reescrever

    int encontrado = 0;
    for (int i = 0; i < num_tripulantes; i++) {
        if (tripulantes[i].codigo == codigo) {
            encontrado = 1;
        } else {
            fwrite(&tripulantes[i], sizeof(Tripulantes), 1, arquivo);
        }
    }

    if (encontrado) {
        printf("Tripulante excluído com sucesso!\n");
    } else {
        printf("Tripulante com o código %d não encontrado!\n", codigo);
    }

    fclose(arquivo);
}

void BuscarTripulante() {


    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int criterio;
    printf("Escolha o critério de busca:\n");
    printf("1 - Buscar por código\n");
    printf("2 - Buscar por nome\n");
    printf("3 - Buscar por cargo\n");
    printf("Escolha uma opção: ");
    scanf("%d", &criterio);
    getchar();  // Consumir a nova linha após o scanf

    Tripulantes tripulante;
    int encontrado = 0;

    switch (criterio) {
        case 1: {
            int codigo;
            printf("Digite o código do tripulante: ");
            scanf("%d", &codigo);
            getchar();

            while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
                if (tripulante.codigo == codigo) {
                    printf("\nNome: %s\n", tripulante.nome);
                    printf("Código: %d\n", tripulante.codigo);
                    printf("Telefone: %d\n", tripulante.telefone);
                    printf("Cargo: %s\n", tripulante.cargo);
                    printf("-----------------------------\n");
                    encontrado = 1;
                    break;
                }
            }
            break;
        }

        case 2: {
            char nome[100];
            printf("Digite o nome do tripulante: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';  // Remover o caractere de nova linha

            while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
                if (strstr(tripulante.nome, nome) != NULL) {  // Verifica se o nome contém o termo
                    printf("\nNome: %s\n", tripulante.nome);
                    printf("Código: %d\n", tripulante.codigo);
                    printf("Telefone: %d\n", tripulante.telefone);
                    printf("Cargo: %s\n", tripulante.cargo);
                    printf("-----------------------------\n");
                    encontrado = 1;
                }
            }
            break;
        }

        case 3: {
            char cargo[50];
            printf("Digite o cargo do tripulante: ");
            fgets(cargo, sizeof(cargo), stdin);
            cargo[strcspn(cargo, "\n")] = '\0';  // Remover o caractere de nova linha

            while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
                if (strstr(tripulante.cargo, cargo) != NULL) {  // Verifica se o cargo contém o termo
                    printf("\nNome: %s\n", tripulante.nome);
                    printf("Código: %d\n", tripulante.codigo);
                    printf("Telefone: %d\n", tripulante.telefone);
                    printf("Cargo: %s\n", tripulante.cargo);
                    printf("-----------------------------\n");
                    encontrado = 1;
                }
            }
            break;
        }

        default:
            printf("Opção inválida!\n");
            fclose(arquivo);
            return;
    }

    if (!encontrado) {
        printf("Nenhum tripulante encontrado com os critérios fornecidos.\n");
    }

    fclose(arquivo);
}

/*-------------------------------------------------------------CADASTRO ASSENTOS------------------------------------------------------------------------*/


/*------------------------------------------------------------------VOOS--------------------------------------------------------------------------*/

int validarData(const char *data) {
    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(data[i])) return 0;
    }
    int dia = atoi(data);
    int mes = atoi(data + 3);
    int ano = atoi(data + 6);
    if (mes < 1 || mes > 12 || dia < 1 || dia > 31 || ano < 1900) return 0;
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > 28 + bissexto) return 0;
    }
    return 1;
}

int validarHora(const char *hora) {
    if (strlen(hora) != 5 || hora[2] != ':') return 0;
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (!isdigit(hora[i])) return 0;
    }
    int horas = atoi(hora);
    int minutos = atoi(hora + 3);
    return horas >= 0 && horas < 24 && minutos >= 0 && minutos < 60;
}

int validarTarifa(const char *tarifa_str, float *tarifa) {
    char *endptr;
    *tarifa = strtof(tarifa_str, &endptr);
    if (*endptr != '\0' || *tarifa < 0) return 0; // Verifica se houve conversão válida e se o valor é positivo
    return 1;
}

// Função para verificar se o código já existe
int codigoJaExiste(const char *codigo) {
    for (int i = 0; i < total_voos; i++) {
        if (strcmp(voos[i].codigo, codigo) == 0) {
            return 1; // Código já existe
        }
    }
    return 0; // Código não encontrado
}

// Função para salvar os voos em arquivo binário
void salvarVoos() {
    FILE *arquivo = fopen(ARQUIVO_VOOS, "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }
    fwrite(&total_voos, sizeof(int), 1, arquivo);
    fwrite(voos, sizeof(Voo), total_voos, arquivo);
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

// Função para carregar os voos do arquivo binário
void carregarVoos() {
    FILE *arquivo = fopen(ARQUIVO_VOOS, "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de dados encontrado. Iniciando sistema vazio.\n");
        return;
    }
    fread(&total_voos, sizeof(int), 1, arquivo);
    fread(voos, sizeof(Voo), total_voos, arquivo);
    fclose(arquivo);
    printf("Dados carregados com sucesso!\n");
}

void adicionarVoo() {
    if (total_voos >= MAX_VOOS) {
        printf("Limite de voos atingido!\n");
        return;
    }

    Voo novoVoo;

    do {
        printf("-----------------------------\n");
        printf("-Código do voo: ");
        scanf("%s", novoVoo.codigo);
        if (codigoJaExiste(novoVoo.codigo)) {
            printf("Código já existe! Por favor, insira um código único.\n");
        }
    } while (codigoJaExiste(novoVoo.codigo));

    do {
        printf("-----------------------------\n");
        printf("-Data do voo (DD/MM/AAAA): ");
        scanf("%s", novoVoo.data);
        if (!validarData(novoVoo.data)) {
            printf("Data inválida! Use o formato DD/MM/AAAA e insira apenas números válidos.\n");
        }
    } while (!validarData(novoVoo.data));

    do {
        printf("-----------------------------\n");
        printf("-Hora do voo (HH:MM): ");
        scanf("%s", novoVoo.hora);
        if (!validarHora(novoVoo.hora)) {
            printf("Hora inválida! Use o formato HH:MM e insira apenas números válidos.\n");
        }
    } while (!validarHora(novoVoo.hora));

    printf("-----------------------------\n");
    printf("-Origem: ");
    scanf(" %[^\n]", novoVoo.origem);

    printf("-----------------------------\n");
    printf("-Destino: ");
    scanf(" %[^\n]", novoVoo.destino);

    char tarifa_str[20];
    do {
        printf("-----------------------------\n");
        printf("-Tarifa (apenas números): ");
        scanf("%s", tarifa_str);
        if (!validarTarifa(tarifa_str, &novoVoo.tarifa)) {
            printf("Tarifa inválida! Insira um número positivo válido.\n");
        }
    } while (!validarTarifa(tarifa_str, &novoVoo.tarifa));

    printf("-----------------------------\n");
    printf("-Informe o nome do avião: ");
    scanf(" %[^\n]", novoVoo.aviao);

    // Adicionar tripulação
    novoVoo.qtd_tripulacao = 0;
    int piloto_encontrado = 0, copiloto_encontrado = 0, opcao_comissario, codigo_tripulante, encontrado = 0;
    Tripulantes tripulante;
    int adicionar_tripulantes;

    while (piloto_encontrado == 0 || copiloto_encontrado == 0) {
        printf("-----------------------------\n");
        printf("-Tripulantes: \n");
        printf("1- Adicionar\n");
        printf("2- Buscar\n");
        printf("0- Cancelar\n");
        scanf("%d", &adicionar_tripulantes);
        while (getchar() != '\n');  // Limpar o buffer do teclado

        switch (adicionar_tripulantes) {
    case 1:
        // Adicionando piloto
        printf("\nDigite o código do piloto: ");
        scanf("%d", &codigo_tripulante);
        while (getchar() != '\n');  // Limpar o buffer do teclado

        FILE *arquivo = fopen("tripulantes.bin", "rb");
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            return;
        }

        while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
            // Adiciona a comparação com strncmp para garantir que o cargo "piloto" seja verificado corretamente
            if (tripulante.codigo == codigo_tripulante && strncmp(tripulante.cargo, "Piloto(a)", sizeof(tripulante.cargo)) == 0)  {
                printf("\nNome: %s\n", tripulante.nome);
                printf("Código: %d\n", tripulante.codigo);
                printf("Telefone: %d\n", tripulante.telefone);
                printf("Cargo: %s\n", tripulante.cargo);
                printf("-----------------------------\n");

                novoVoo.tripulacao[0] = tripulante;  // Adiciona o piloto
                piloto_encontrado = 1;
                novoVoo.qtd_tripulacao = 1;  // Incrementa a quantidade de tripulantes
                printf("Piloto adicionado à tripulação.\n");
                encontrado = 1;
                break;
            }
        }
        fclose(arquivo);
        if (!encontrado) {
            printf("Piloto não encontrado ou cargo incorreto.\n");
        }else if (copiloto_encontrado == 0) {
                // Adicionando copiloto
                printf("\nDigite o código do copiloto: ");
                scanf("%d", &codigo_tripulante);
                while (getchar() != '\n');  // Limpar o buffer do teclado

                FILE *arquivo = fopen("tripulantes.bin", "rb");
                if (arquivo == NULL) {
                    perror("Erro ao abrir o arquivo");
                    return;
                }

                encontrado = 0;
                while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
                    if (tripulante.codigo == codigo_tripulante && strncmp(tripulante.cargo, "Copiloto(a)", sizeof(tripulante.cargo)) == 0) {
                        printf("\nNome: %s\n", tripulante.nome);
                        printf("Código: %d\n", tripulante.codigo);
                        printf("Telefone: %d\n", tripulante.telefone);
                        printf("Cargo: %s\n", tripulante.cargo);
                        printf("-----------------------------\n");

                        novoVoo.tripulacao[1] = tripulante;  // Adiciona o copiloto
                        copiloto_encontrado = 1;
                        novoVoo.qtd_tripulacao++;  // Incrementa a quantidade de tripulantes
                        printf("Copiloto adicionado à tripulação.\n");
                        encontrado = 1;
                        break;
                    }
                }
                fclose(arquivo);
                if (!encontrado) {
                    printf("Copiloto não encontrado ou cargo incorreto.\n");
                }
            } else {
                printf("Ambos, piloto e copiloto, já foram adicionados.\n");
            }
            break;


        break;

            case 2:
                // Buscar tripulação
                printf("\nBuscando tripulação...\n");
                LerTripulantes();
                break;

            case 0:
                // Cancelar
                printf("Cancelando...\n");
                return;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }

    // Após piloto e copiloto encontrados, perguntar sobre o comissário
    printf("Deseja adicionar um comissário (1 para sim, 0 para não)? ");
    scanf("%d", &opcao_comissario);
    while (getchar() != '\n');  // Limpar o buffer do teclado

    if (opcao_comissario == 1 && novoVoo.qtd_tripulacao < MAX_TRIPULACAO) {
        printf("Digite o código do comissário: ");
        scanf("%d", &codigo_tripulante);
        while (getchar() != '\n');  // Limpar o buffer do teclado

        FILE* arquivo = fopen("tripulantes.bin", "rb");
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            return;
        }

        encontrado = 0;
        while (fread(&tripulante, sizeof(Tripulantes), 1, arquivo) == 1) {
            if (tripulante.codigo == codigo_tripulante && strcmp(tripulante.cargo, "comissário") == 0) {
                novoVoo.tripulacao[novoVoo.qtd_tripulacao] = tripulante;  // Adiciona o comissário
                novoVoo.qtd_tripulacao++;
                printf("Comissário adicionado à tripulação.\n");
                encontrado = 1;
                break;
            }
        }
        fclose(arquivo);

        if (!encontrado) {
            printf("Comissário não encontrado ou cargo incorreto.\n");
        }
    }

    printf("-----------------------------\n");
    voos[total_voos++] = novoVoo;
    printf("Voo cadastrado com sucesso!\n");

    printf("-----------------------------\n");
    salvarVoos(); // Salvar os dados atualizados
    return;
}

// Função para listar todos os voos
void listarVoos() {
    if (total_voos == 0) {
        printf("Nenhum voo cadastrado.\n");
        return;
    }

    for (int i = 0; i < total_voos; i++) {
        printf("\n-----------------------------\n");
        printf("\nVoo %d:\n", i + 1);
        printf("Código: %s\n", voos[i].codigo);
        printf("\n-----------------------------\n");
        printf("Data: %s\n", voos[i].data);
        printf("\n-----------------------------\n");
        printf("Hora: %s\n", voos[i].hora);
        printf("\n-----------------------------\n");
        printf("Origem: %s\n", voos[i].origem);
        printf("\n-----------------------------\n");
        printf("Destino: %s\n", voos[i].destino);
        printf("\n-----------------------------\n");
        printf("Tarifa: %.2f\n", voos[i].tarifa);
        printf("\n-----------------------------\n");
        printf("Avião: %s\n", voos[i].aviao);
        printf("\n-----------------------------\n");
        printf("Tripulação:\n");
        for (int j = 0; j < voos[i].qtd_tripulacao; j++) {
            printf("Nome: %s\nFunção: %s\n", voos[i].tripulacao[j].nome, voos[i].tripulacao[j].cargo);
        }
        printf("-----------------------------\n");
    }
}

void buscarVoo() {
    if (total_voos == 0) {
        printf("Nenhum voo cadastrado.\n");
        return;
    }

    int criterio;
    printf("Escolha o critério de busca:\n");
    printf("1 - Buscar por código do voo\n");
    printf("2 - Buscar por nome do piloto\n");
    printf("3 - Buscar por número do voo\n");
    printf("Escolha uma opção: ");
    scanf("%d", &criterio);
    getchar();  // Limpar o buffer

    int encontrado = 0;
    switch (criterio) {
        case 1: {  // Buscar por código do voo
            char codigoBusca[10];
            printf("Digite o código do voo: ");
            fgets(codigoBusca, sizeof(codigoBusca), stdin);
            codigoBusca[strcspn(codigoBusca, "\n")] = '\0';  // Remover newline

            for (int i = 0; i < total_voos; i++) {
                if (strcmp(voos[i].codigo, codigoBusca) == 0) {
                    printf("\nVoo encontrado:\n");
                    printf("\nVoo %d:\n", i + 1);
                    printf("Código: %s\n", voos[i].codigo);
                    printf("\n-----------------------------\n");
                    printf("Data: %s\n", voos[i].data);
                    printf("\n-----------------------------\n");
                    printf("Hora: %s\n", voos[i].hora);
                    printf("\n-----------------------------\n");
                    printf("Origem: %s\n", voos[i].origem);
                    printf("\n-----------------------------\n");
                    printf("Destino: %s\n", voos[i].destino);
                    printf("\n-----------------------------\n");
                    printf("Tarifa: %.2f\n", voos[i].tarifa);
                    printf("\n-----------------------------\n");
                    printf("Avião: %s\n", voos[i].aviao);
                    printf("\n-----------------------------\n");
                    printf("Tripulação:\n");
                    for (int j = 0; j < voos[i].qtd_tripulacao; j++) {
                        printf("Nome: %s\nFunção: %s\n", voos[i].tripulacao[j].nome, voos[i].tripulacao[j].cargo);
                    }
                    printf("-----------------------------\n");
                    }
                    encontrado = 1;
                    break;
                }
            }
            break;
        case 2: {  // Buscar por nome do piloto
            char nomePiloto[100];
            printf("Digite o nome do piloto: ");
            fgets(nomePiloto, sizeof(nomePiloto), stdin);
            nomePiloto[strcspn(nomePiloto, "\n")] = '\0';  // Remover newline

            for (int i = 0; i < total_voos; i++) {
                for (int j = 0; j < voos[i].qtd_tripulacao; j++) {
                    if (strcmp(voos[i].tripulacao[j].nome, nomePiloto) == 0 &&
                        strcmp(voos[i].tripulacao[j].cargo, "Piloto(a)") == 0) {
                        printf("\nVoo encontrado:\n");
                        printf("\nVoo %d:\n", i + 1);
                        printf("Código: %s\n", voos[i].codigo);
                        printf("\n-----------------------------\n");
                        printf("Data: %s\n", voos[i].data);
                        printf("\n-----------------------------\n");
                        printf("Hora: %s\n", voos[i].hora);
                        printf("\n-----------------------------\n");
                        printf("Origem: %s\n", voos[i].origem);
                        printf("\n-----------------------------\n");
                        printf("Destino: %s\n", voos[i].destino);
                        printf("\n-----------------------------\n");
                        printf("Tarifa: %.2f\n", voos[i].tarifa);
                        printf("\n-----------------------------\n");
                        printf("Avião: %s\n", voos[i].aviao);
                        printf("\n-----------------------------\n");
                        printf("Tripulação:\n");
                        for (int j = 0; j < voos[i].qtd_tripulacao; j++) {
                        printf("Nome: %s\nFunção: %s\n", voos[i].tripulacao[j].nome, voos[i].tripulacao[j].cargo);
                    }
                    printf("-----------------------------\n");
                        encontrado = 1;
                        break;
                    }
                }
            }
            break;
        }
        case 3: {  // Buscar por número do voo
    int numeroVoo;
    printf("Digite o número do voo (1 a %d): ", total_voos);
    scanf("%d", &numeroVoo);
    getchar();

    if (numeroVoo > 0 && numeroVoo <= total_voos) {
        Voo *voo = &voos[numeroVoo - 1];
        printf("\nVoo encontrado:\n");
        printf("\nVoo %d:\n", numeroVoo);
        printf("Código: %s\n", voo->codigo);
        printf("\n-----------------------------\n");
        printf("Data: %s\n", voo->data);
        printf("\n-----------------------------\n");
        printf("Hora: %s\n", voo->hora);
        printf("\n-----------------------------\n");
        printf("Origem: %s\n", voo->origem);
        printf("\n-----------------------------\n");
        printf("Destino: %s\n", voo->destino);
        printf("\n-----------------------------\n");
        printf("Tarifa: %.2f\n", voo->tarifa);
        printf("\n-----------------------------\n");
        printf("Avião: %s\n", voo->aviao);
        printf("\n-----------------------------\n");
        printf("Tripulação:\n");
        for (int j = 0; j < voo->qtd_tripulacao; j++) {
            printf("Nome: %s\nFunção: %s\n", voo->tripulacao[j].nome, voo->tripulacao[j].cargo);
        }
        printf("-----------------------------\n");
        encontrado = 1;
    } else {
        printf("Número do voo inválido.\n");
    }
    break;

}
        default:
            printf("Opção inválida.\n");
            return;
    }

    if (!encontrado) {
        printf("Voo não encontrado.\n");
    }
}

/*------------------------------------------------------------CADASTRO PASSAGEIROS------------------------------------------------------------------------*/

int verificaCodigoPassageiro(int codigo) {
    // Verificar no array em memória
    for (int i = 0; i < total_passageiros; i++) {
        if (passageiros[i].codigo == codigo) {
            return 1; // Código já existe no array
        }
    }

    // Verificar no arquivo binário
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (arquivo == NULL) {
        return 0; // Arquivo não existe, código não encontrado
    }

    Passageiro passageiro;
    while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
        if (passageiro.codigo == codigo) {
            fclose(arquivo);
            return 1; // Código já existe no arquivo
        }
    }

    fclose(arquivo);
    return 0; // Código não encontrado
}

void salvarPassageiroNoArquivo(Passageiro *passageiro) {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o passageiro.\n");
        return;
    }

    fwrite(passageiro, sizeof(Passageiro), 1, arquivo);
    fclose(arquivo);
}

void cadastrarPassageiro() {
    if (total_passageiros >= MAX_PASSAGEIROS) {
        printf("Limite de passageiros atingido!\n");
        return;
    }

    Passageiro novo_passageiro;

    printf("Cadastro de passageiro\n\n");
    // Leitura do código
    do {
        printf("Digite o código do passageiro: ");
        scanf("%d", &novo_passageiro.codigo);

        if (verificaCodigoPassageiro(novo_passageiro.codigo)) {
            printf("Código já existe! Por favor, insira um código único.\n");
        }
    } while (verificaCodigoPassageiro(novo_passageiro.codigo));

    // Limpar buffer
    getchar();

     printf("\n____________________________________\n");
    // Leitura dos outros campos
    printf("Digite o nome do passageiro: ");
    fgets(novo_passageiro.nome, sizeof(novo_passageiro.nome), stdin);
    novo_passageiro.nome[strcspn(novo_passageiro.nome, "\n")] = '\0';
    ConverterPrimeiraMaiuscula(novo_passageiro.nome);
    
     printf("\n____________________________________\n");

    printf("Digite o endereço do passageiro: ");
    fgets(novo_passageiro.endereco, sizeof(novo_passageiro.endereco), stdin);
    novo_passageiro.endereco[strcspn(novo_passageiro.endereco, "\n")] = '\0';

    printf("\n____________________________________\n");

    printf("Digite o telefone do passageiro: ");
    fgets(novo_passageiro.telefone, sizeof(novo_passageiro.telefone), stdin);
    novo_passageiro.telefone[strcspn(novo_passageiro.telefone, "\n")] = '\0';

    printf("\n____________________________________\n");

        novo_passageiro.pontos = 0;
    

    // Armazenar o passageiro no array
    passageiros[total_passageiros++] = novo_passageiro;

    // Salvar no arquivo binário
    salvarPassageiroNoArquivo(&novo_passageiro);
    printf("\n");
    printf("Passageiro cadastrado com sucesso!\n");
}

void listarPassageiros() {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (arquivo == NULL) {
        printf("Nenhum passageiro cadastrado ainda.\n");
        return;
    }

    Passageiro passageiro;
    printf("Passageiros Cadastrados:\n");
    printf("---------------------------------------------------------\n");
    while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
        printf("Código: %d\n", passageiro.codigo);
        printf("Nome: %s\n", passageiro.nome);
        printf("Endereço: %s\n", passageiro.endereco);
        printf("Telefone: %s\n", passageiro.telefone);
        printf("Fidelidade: %s\n", passageiro.fidelidade ? "Sim" : "Não");
        printf("Pontos: %d\n", passageiro.pontos);
        printf("---------------------------------------------------------\n");
    }

    fclose(arquivo);
}
// Função para buscar um passageiro por nome ou código
void buscarPassageiro() {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (arquivo == NULL) {
        printf("Nenhum passageiro cadastrado ainda.\n");
        return;
    }

    int opcao;
    printf("Buscar passageiro por:\n");
    printf("1 - Código\n");
    printf("2 - Nome\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    getchar(); // Limpar o buffer de entrada

    printf("\n");

    Passageiro passageiro;
    int encontrado = 0;

    if (opcao == 1) { // Busca por código
        int codigoBusca;
        printf("Digite o código do passageiro: ");
        scanf("%d", &codigoBusca);

        printf("\n");

        while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
            if (passageiro.codigo == codigoBusca) {
                printf("Passageiro Encontrado:\n");
                printf("---------------------------------------------------------\n");
                printf("Código: %d\n", passageiro.codigo);
                printf("Nome: %s\n", passageiro.nome);
                printf("Endereço: %s\n", passageiro.endereco);
                printf("Telefone: %s\n", passageiro.telefone);
                printf("Pontos: %d\n", passageiro.pontos);
                printf("---------------------------------------------------------\n");
                encontrado = 1;
                break;
            }
        }

    } else if (opcao == 2) { // Busca por nome
        char nomeBusca[100];
        printf("Digite o nome do passageiro: ");
        fgets(nomeBusca, sizeof(nomeBusca), stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = '\0'; // Remover o caractere de nova linha

        printf("\n");

        while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
            if (strstr(passageiro.nome, nomeBusca)) {
                printf("Passageiro Encontrado:\n");
                printf("---------------------------------------------------------\n");
                printf("Código: %d\n", passageiro.codigo);
                printf("Nome: %s\n", passageiro.nome);
                printf("Endereço: %s\n", passageiro.endereco);
                printf("Telefone: %s\n", passageiro.telefone);
                printf("Pontos: %d\n", passageiro.pontos);
                printf("---------------------------------------------------------\n");
                encontrado = 1;
            }
        }
    } else {
        printf("Opção inválida.\n");
    }

    if (!encontrado) {
        printf("Nenhum passageiro encontrado.\n");
    }

    fclose(arquivo);
}

/*----------------------------------------------------------------RESERVAS DE VOO-----------------------------------------------------------------------*/

// Função para salvar os dados de reservas em um arquivo binário
void salvarReservas() {
    FILE *arquivo = fopen("reservas.bin", "wb");  // Abrir arquivo para escrita binária

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar as reservas.\n");
        return;
    }

    // Salvar o total de voos
    fwrite(&total_voos, sizeof(int), 1, arquivo);

    // Salvar os voos
    fwrite(voos, sizeof(Voo), total_voos, arquivo);

    // Salvar o total de passageiros
    fwrite(&total_passageiros, sizeof(int), 1, arquivo);

    // Salvar os passageiros
    fwrite(passageiros, sizeof(Passageiro), total_passageiros, arquivo);

    // Salvar o total de tripulantes
    fwrite(&total_tripulantes, sizeof(int), 1, arquivo);

    // Salvar os tripulantes
    fwrite(tripulantes, sizeof(Tripulantes), total_tripulantes, arquivo);

    // Salvar os detalhes dos assentos
    fwrite(assentosDetalhes, sizeof(Assento), TOTAL_ASSENTOS, arquivo);

    // Fechar o arquivo
    fclose(arquivo);

    printf("Dados salvos com sucesso no arquivo reservas.bin!\n");
}

Passageiro* selecionarOuCadastrarPassageiro(Passageiro *passageiros, int *total_passageiros) {
    int opcao;

    printf("\nEscolha uma opcao:\n");
    printf("1. Selecionar passageiro existente\n");
    printf("2. Cadastrar novo passageiro\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    getchar(); // Limpar buffer do teclado

    if (opcao == 1) {
        // Listar e buscar passageiro existente
        listarPassageiros();
        int codigoBusca;
        printf("Digite o codigo do passageiro: ");
        scanf("%d", &codigoBusca);
        getchar(); // Limpar buffer do teclado

        for (int i = 0; i < *total_passageiros; i++) {
            if (passageiros[i].codigo == codigoBusca) {
                return &passageiros[i];
            }
        }
        printf("Passageiro nao encontrado.\n");
    } else if (opcao == 2) {
        // Cadastrar novo passageiro
        if (*total_passageiros >= MAX_PASSAGEIROS) {
            printf("Limite de passageiros atingido.\n");
            return NULL;
        }

        cadastrarPassageiro(); // Cadastra e adiciona ao array
        return &passageiros[*total_passageiros - 1]; // Retorna o último cadastrado
    } else {
        printf("Opcao invalida.\n");
    }
    return NULL;
} 

void inicializarAssentos(int **assentos, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            assentos[i][j] = 0; // Inicializa todos os assentos como livres
        }
    }
}

void exibirAssentos(int **assentos, int linhas, int colunas) {
    printf("Mapa de Assentos:\n\n");
    printf("   ");
    for (char c = 'A'; c < 'A' + colunas; c++) {
        printf("%c ", c);
        if ((c - 'A' + 1) % 3 == 0 && c != 'A' + colunas - 1) printf("  "); // Espaço extra entre grupos
    }
    printf("\n");

    for (int i = 0; i < linhas; i++) {
        printf("%2d|", i + 1);
        for (int j = 0; j < colunas; j++) {
            printf("%d ", assentos[i][j]);
            if ((j + 1) % 3 == 0 && j != colunas - 1) printf("  ");
        }
        printf("\n");
    }
}

int colunaParaIndice(char coluna, int colunas) {
    if (coluna >= 'A' && coluna < 'A' + colunas) {
        return coluna - 'A';
    } else if (coluna >= 'a' && coluna < 'a' + colunas) {
        return coluna - 'a';
    }
    return -1; // Letra inválida
}

int ocuparAssento(int **assentos, int fileira, char coluna, int linhas, int colunas) {
    int colIdx = colunaParaIndice(coluna, colunas);
    if (fileira < 1 || fileira > linhas || colIdx == -1 || colIdx >= colunas) {
        printf("Assento invalido!\n");
        return 0;
    }
    if (assentos[fileira - 1][colIdx] == 1) {
        printf("Assento %d%c ja esta ocupado!\n", fileira, coluna);
        return 0;
    }
    assentos[fileira - 1][colIdx] = 1;
    printf("Assento %d%c ocupado com sucesso!\n", fileira, coluna);
    return 1;
}

void selecionarAssento(Assento *assentos, int linhas, int colunas) {
    int opcao, fileira;
    char coluna;

    do {
        printf("\nEscolha uma opção:\n");
        printf("1. Escolher um assento\n");
        printf("2. Voltar para selecionar outro voo\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer do teclado

        switch (opcao) {
            case 1:
                exibirAssentos(assentos, linhas, colunas);
                printf("Digite a fileira (1-%d): ", linhas);
                scanf("%d", &fileira);
                printf("Digite a coluna (A-%c): ", 'A' + colunas - 1);
                scanf(" %c", &coluna);
                getchar(); // Limpar buffer do teclado

                if (ocuparAssento(assentos, fileira, coluna, linhas, colunas)) {
                    // Solicitar informações do passageiro
                    Passageiro *passageiroSelecionado = selecionarOuCadastrarPassageiro(passageiros, &total_passageiros);
                    if (passageiroSelecionado != NULL) {
                        printf("Assento reservado com sucesso para %s!\n", passageiroSelecionado->nome);

                        // Associar o passageiro ao assento
                        int colunaIndiceX = colunaParaIndice(coluna, colunas);
                        // Acessar o assento diretamente
                        assentos[(fileira - 1) * colunas + colunaIndiceX].status = 1; // Marcar como ocupado
                        assentos[(fileira - 1) * colunas + colunaIndiceX].nomePassageiro = *passageiroSelecionado; // Atribui os dados do passageiro
                    }
                    return; // Sai da função após reservar
                }
                break;

            case 2:
                printf("\nVoltando para selecionar outro voo...\n");
                return;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 2);
}

void cadastrarReserva(Informacao *aviao){
    char origem[50], destino[50];
    int vooEncontrado = 0;
    int indices[MAX_VOOS];

    printf("\tCadastro de Reserva:\n");
    printf("\n");

    printf("\nInforme a ORIGEM da sua viagem:\n");
    fgets(origem, sizeof(origem), stdin);
    origem[strcspn(origem, "\n")] = '\0';

    printf("\nInforme o DESTINO da sua viagem:\n");
    fgets(destino, sizeof(destino), stdin);
    destino[strcspn(destino, "\n")] = '\0';

    ConverterPrimeiraMaiuscula(origem);

    ConverterPrimeiraMaiuscula(destino);

    printf("\n\nProucurando voo de %s para %s...", origem, destino);

    printf("\n\n");

    for(int i = 0; i < MAX_VOOS; i++){
        if(strcmp(origem, aviao->origem[i].origem) == 0 && strcmp(destino, aviao->destino[i].destino) == 0){
            printf("Voo %d - Codigo: %s - Data %s - Hora %s - Tarifa: %.2f\n", vooEncontrado + 1, aviao->origem[i].codigo, aviao->origem[i].data, aviao->origem[i].hora, aviao->origem[i].tarifa);
            indices[vooEncontrado++] = i;
        }
    }

    if(vooEncontrado == 0){
        printf("Nenhum voo encontrado de %s para %s.", origem, destino);
        return;
    }

    int escolha;

    printf("\nEscolha um dos voos listados (1 a %d)", vooEncontrado);
    scanf("%d", &escolha);
    getchar();

    if(escolha < 1 || escolha > vooEncontrado){
        printf("\n\nESCOLA UMA OPCAO VALIDA.\n\n");
        return;
    }

    int vooSelecionado = indices[escolha -1];

    printf("\nVoce escolheu o Voo %s de %s para %s, Data: %s, Hora: %s, Tarifa: %.2f\n", aviao->origem[vooSelecionado].codigo, aviao->origem[vooSelecionado].origem, aviao->destino[vooSelecionado].destino, aviao->origem[vooSelecionado].data, aviao->origem[vooSelecionado].hora, aviao->origem[vooSelecionado].tarifa);

    // Inicializar mapa de assentos para o voo (exemplo: 10 linhas x 6 colunas)
    int linhas = 10, colunas = 6;
    int **assentos = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        assentos[i] = (int *)malloc(colunas * sizeof(int));
    }
    inicializarAssentos(assentos, linhas, colunas);

    // Seleção de assento
    selecionarAssento(assentos, linhas, colunas);

    // Liberar memória dos assentos
    for (int i = 0; i < linhas; i++) {
        free(assentos[i]);
    }
    free(assentos);

    salvarReservas();
}

void cancelarReserva(){
        int codigoPassageiro;
    int encontrouReservas = 0;
    int vooEscolhido, assentoEscolhido;

    // Passo 1: Solicitar o cadastro (código do passageiro)
    printf("Digite o codigo do passageiro: ");
    scanf("%d", &codigoPassageiro);
    getchar(); // Limpar buffer

    printf("\nReservas do passageiro %d:\n", codigoPassageiro);

    // Passo 2: Listar reservas associadas ao passageiro
    for (int i = 0; i < total_voos; i++) {
        for (int j = 0; j < MAX; j++) {  // Certifique-se de usar o valor correto para o número de assentos
            Assento *assento = &voos[i].assentos[j];  // Acessando o array de assentos corretamente
            if (assento->status == 1 && assento->nomePassageiro.codigo == codigoPassageiro) {
                encontrouReservas = 1;
                printf("Voo %s | Origem: %s | Destino: %s | Data: %s | Hora: %s\n",
                    voos[i].codigo, voos[i].origem, voos[i].destino, voos[i].data, voos[i].hora);
                printf("  -> Assento: %d | Tarifa: R$ %.2f\n", assento->numero, voos[i].tarifa);
                printf("\n");
            }
        }
    }

    if (!encontrouReservas) {
        printf("Nenhuma reserva encontrada para o passageiro %d.\n", codigoPassageiro);
        return;
    }

    // Passo 3: Escolher voo e assento
    printf("Digite o numero do voo com a reserva que deseja cancelar: ");
    scanf("%d", &vooEscolhido);
    getchar(); // Limpar buffer

    if (vooEscolhido < 1 || vooEscolhido > total_voos) {
        printf("Voo invalido!\n");
        return;
    }

    printf("Digite o numero do assento que deseja cancelar: ");
    scanf("%d", &assentoEscolhido);
    getchar(); // Limpar buffer

    if (assentoEscolhido < 1 || assentoEscolhido > MAX) {
        printf("Assento invalido!\n");
        return;
    }

    // Validar se o assento pertence ao passageiro e está ocupado
    Assento *assentoParaCancelar = NULL;
    for (int i = 0; i < MAX; i++) {
        if (voos[vooEscolhido - 1].assentos[i].numero == assentoEscolhido &&
            voos[vooEscolhido - 1].assentos[i].status == 1 &&
            voos[vooEscolhido - 1].assentos[i].nomePassageiro.codigo == codigoPassageiro) {
            assentoParaCancelar = &voos[vooEscolhido - 1].assentos[i];
            break;
        }
    }

    if (assentoParaCancelar == NULL) {
        printf("Assento nao encontrado ou nao esta associado ao passageiro.\n");
        return;
    }

    // Passo 4: Confirmar cancelamento
    printf("Confirma o cancelamento do assento %d no voo %s? (1-Sim / 0-Não): ",
           assentoEscolhido, voos[vooEscolhido - 1].codigo);
    int confirmar;
    scanf("%d", &confirmar);
    getchar(); // Limpar buffer

    if (confirmar != 1) {
        printf("Operacao cancelada.\n");
        return;
    }

    // Passo 5: Atualizar status do assento
    assentoParaCancelar->status = 0; // Tornar o assento desocupado
    memset(&assentoParaCancelar->nomePassageiro, 0, sizeof(Passageiro)); // Limpar dados do passageiro

    printf("Reserva do assento %d no voo %s cancelada com sucesso!\n",
           assentoEscolhido, voos[vooEscolhido - 1].codigo);

    salvarReservas();
}

void buscarReserva() {
    char nomePassageiro[100];
    int encontrouReservas = 0;
    
    // Passo 1: Solicitar o nome do passageiro
    printf("Digite o nome do passageiro: ");
    fgets(nomePassageiro, sizeof(nomePassageiro), stdin);
    nomePassageiro[strcspn(nomePassageiro, "\n")] = '\0'; // Remover o \n do final do nome

    printf("\nReservas encontradas para o passageiro: %s\n", nomePassageiro);

    // Passo 2: Listar reservas associadas ao nome do passageiro
    for (int i = 0; i < total_voos; i++) {  // total_voos deve ser o número de voos cadastrados
        for (int j = 0; j < MAX; j++) {  // MAX é o número máximo de assentos por voo
            Assento *assento = &voos[i].assentos[j];  // Assumindo que voos[i].assentos[j] é a estrutura de assento
            if (assento->status == 1 && strcmp(assento->nomePassageiro.nome, nomePassageiro) == 0) {
                encontrouReservas = 1;
                printf("Voo %s | Origem: %s | Destino: %s | Data: %s | Hora: %s\n",
                       voos[i].codigo, voos[i].origem, voos[i].destino, voos[i].data, voos[i].hora);
                printf("  -> Assento: %d | Tarifa: R$ %.2f\n", assento->numero, voos[i].tarifa);
                printf("\n");
            }
        }
    }

    if (!encontrouReservas) {
        printf("Nenhuma reserva encontrada para o passageiro %s.\n", nomePassageiro);
    }
}

/*------------------------------------------------------------------FIDELIDADE---------------------------------------------------------------------*/

void carregarPassageiros() {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (arquivo == NULL) {
        printf("Nenhum arquivo de passageiros encontrado. Criando novo arquivo.\n");
        return;
    }

    total_passageiros = fread(passageiros, sizeof(Passageiro), MAX_PASSAGEIROS, arquivo);
    fclose(arquivo);
}

int buscarFidelidade(const char *id) {
    for (int i = 0; i < total_passageiros; i++) {
        char codigo_str[10];
        sprintf(codigo_str, "%d", passageiros[i].codigo);
        if (strcmp(codigo_str, id) == 0) {
            return i; // Retorna o índice do passageiro
        }
    }
    return -1; // Não encontrado
}

void registrarPontos() {

    Passageiro passageiros_fidelidade;

    do {
        printf("Digite o código único do passageiro (somente números): ");
        scanf("%d", &passageiros_fidelidade.codigo);
        if (!verificaCodigoPassageiro(passageiros_fidelidade.codigo)) {
            printf("Código inválido! Use apenas números.\n");
        }
    } while (!verificaCodigoPassageiro(passageiros_fidelidade.codigo));

    char codigo_str[10];
    sprintf(codigo_str, "%d", passageiros_fidelidade.codigo);
    int indice = buscarFidelidade(codigo_str);

    if (indice != -1) {
        passageiros[indice].pontos += 10;
        printf("10 pontos adicionados para o passageiro %s (%d).\n", passageiros[indice].nome, passageiros[indice].codigo);

        salvarPassageiroNoArquivo(&passageiros_fidelidade);
    }
}

void consultarPontos() {
    
    Passageiro passageiros_fidelidade;

    printf("Digite o código único do passageiro: ");
    scanf("%d", &passageiros_fidelidade.codigo);

    char codigo_str[10];
    sprintf(codigo_str, "%d", passageiros_fidelidade.codigo);
    int indice = buscarFidelidade(codigo_str);

    if (indice != -1) {
        printf("Passageiro: %s (%d)\n", passageiros[indice].nome, passageiros[indice].codigo);
        printf("Pontos acumulados: %d\n", passageiros[indice].pontos);
    } else {
        printf("Passageiro não encontrado!\n");
    }
}

void removerPontos() {
    
    Passageiro passageiros_fidelidade;

    int pontosARemover;

    printf("Digite o código único do passageiro: ");
    scanf("%d", &passageiros_fidelidade.codigo);

    char codigo_str[10];
    sprintf(codigo_str, "%d", passageiros_fidelidade.codigo);
    int indice = buscarFidelidade(codigo_str);

    if (indice != -1) {
        printf("Passageiro encontrado: %s (%d)\n", passageiros[indice].nome, passageiros[indice].codigo);
        printf("Pontos atuais: %d\n", passageiros[indice].pontos);

        do {
            printf("Digite o número de pontos a remover: ");
            scanf("%d", &pontosARemover);

            if (pontosARemover < 0) {
                printf("Você não pode remover uma quantidade negativa de pontos.\n");
            } else if (pontosARemover > passageiros[indice].pontos) {
                printf("Quantidade de pontos a remover excede os pontos disponíveis.\n");
            }
        } while (pontosARemover < 0 || pontosARemover > passageiros[indice].pontos);

        passageiros[indice].pontos -= pontosARemover;
        printf("%d pontos removidos. Pontos restantes: %d\n", pontosARemover, passageiros[indice].pontos);

        salvarPassageiroNoArquivo(&passageiros_fidelidade); // Salvar alterações no arquivo
    } else {
        printf("Passageiro não encontrado!\n");
    }
}

/*---------------------------------------------------------------FUNÇÕES DE FLUXO---------------------------------------------------------------------*/

void TRIPULANTES(){



    int opcao;
    int codigo = 0;
    Tripulantes tripulantes[MAX_CODIGOS];
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
        CadastroTripulantes(tripulantes);
        SalvarTripulante(tripulantes);
        total_tripulantes++;

        printf("\nDeseja cadastrar outro tripulante? (1 - Sim / 0 - Não): ");
        scanf("%d", &continuar);
        printf("\n");
        getchar();
    } break;
        case 2:
        LerTripulantes();
            break;
        case 3:
        BuscarTripulante();
            break;
        case 4:
            printf("Digite o código do tripulante a ser excluído: ");
            scanf("%d", &codigo);
            ExcluirTripulante(codigo); 
            break;
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
}

void VOOS() {
    carregarVoos(); // Carregar os dados do arquivo ao iniciar o programa

    int opcao;

    do {
        printf("\n--- Sistema de Cadastro de Voos ---\n");
        printf("1. Adicionar voo\n");
        printf("2. Listar voos\n");
        printf("3. Buscar voos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarVoo();
                break;
            case 2:
                listarVoos();
                break;
            case 3:
                buscarVoo();
                break;
            case 0:
                salvarVoos();
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);

    return ;
}

void ASSENTOS(){

}

void PASSAGEIRO(){

    int opcao;

    do
    {    
        printf("\tPassageiros\n");
        printf("\n");
        printf("1 - Cadastrar passageiro\n");
        printf("2 - Listar passageiro\n");
        printf("3 - Buscar passageiro\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\n");
        printf("\nEscolha uma das opcoes (Entre 0 e 3): ");
        scanf("%d", &opcao);
        printf("\n");
        printf("\n");

        switch (opcao)
        {
        case 1:
            cadastrarPassageiro();
            break;
        case 2:
            listarPassageiros();
            break;
        case 3:
            buscarPassageiro();
            break;
        case 0:
        printf("Retornando ao menu principal");
            break;
        default:
        printf("\nNumero invalido, selecione novamente!\n");
            break;
            }

        while (getchar() != '\n');

}while (opcao != 0);
}

void FIDELIDADE(){}

void RESERVA(){
Informacao aviao[50]; 

int opcao;
int continua = 1;

do{
    printf("\tReservas:\n");
    printf("\n");
    printf("1 - Cadastro de Reserva\n");
    printf("2 - Cancelamento de Reserva\n");
    printf("3 - Buscar Reserva\n");
    printf("0 - Voltar ao menu principal\n");
    printf("\n");
    printf("\nEscolha uma das opcoes (Entre 0 e 4): ");
    scanf("%d", &opcao);

    switch(opcao){
        case 1:
            while(continua == 1){
                cadastrarReserva(aviao);

                printf("Deseja realizar outra reserva? (1 - Sim / 0 - Não):");
                scanf("%d", &continua);
            }
        break;
        case 2:
            cancelarReserva();
        break;
        case 3:
            buscarReserva();
        break;
        default:
            printf("\n\nOpcao errada, por gentileza escolha uma opcao valida.\n\n");
        break;
    }
}while(opcao != 0);
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
    printf("2 - Voos\n");
    printf("3 - Assentos\n");
    printf("4 - Passageiros\n");
    printf("5 - Reservas\n");
    printf("6 - Programa fidelidade\n");
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
            VOOS();
            break;
        case 3:
            ASSENTOS();
            break;
        case 4:
            PASSAGEIRO();
            break;
        case 5:
            RESERVA();
            break;
        case 6:
            registrarPontos();
            break;
        default:
            printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    
    } while (opcao != 0);
    

    return 0;
}