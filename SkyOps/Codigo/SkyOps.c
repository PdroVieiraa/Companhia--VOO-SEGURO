#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_VOOS 100
#define MAX_TRIPULACAO 5
#define ARQUIVO_VOOS "voos.bin"
#define MAX_CODIGOS 100
#define ARQUIVO_BINARIO "tripulantes.bin"

typedef struct {
    char nome[100];    // Nome do tripulante
    int codigo;       // Código único do tripulante
    int telefone;     // Número de telefone do tripulante
    char cargo[20];   // Cargo do tripulante (piloto, copiloto, comissário)
} Tripulantes;

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
} Voo;



Voo voos[MAX_VOOS];
int total_voos = 0;

Tripulantes tripulantes[MAX_CODIGOS];
char *codigos_gerados[MAX_CODIGOS];
int num_codigos = 0;
int total_tripulantes = 0;


/*---------------------------------------------------Ferramentas--------------------------------------------------------------*/

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

/*---------------------------------------------------TRIPULANTES---------------------------------------------------------------*/

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
            return 1;
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
                    return 1;
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
                return 0;

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
            return 1;
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
    return 0;
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

/*-----------------------------------------------------------------CADASTRO PASSAGEIROS------------------------------------------------------------------------*/


// Arquivo para persistência dos dados
#define ARQUIVO_PASSAGEIROS "passageiros.dat"

// Função para cadastrar um passageiro
int cadastrarPassageiro(Passageiro *novoPassageiro) {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de passageiros");
        return 0;
    }

    fwrite(novoPassageiro, sizeof(Passageiro), 1, arquivo);
    fclose(arquivo);
    return 1; // Sucesso
}

// Função para listar todos os passageiros
void listarPassageiros() {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de passageiros");
        return;
    }

    Passageiro passageiro;
    printf("Lista de Passageiros:\n");
    while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
        printf("Código: %d, Nome: %s, Endereço: %s, Telefone: %s, Fidelidade: %s, Pontos: %d\n",
               passageiro.codigo,
               passageiro.nome,
               passageiro.endereco,
               passageiro.telefone,
               passageiro.fidelidade ? "Sim" : "Não",
               passageiro.pontos);
    }
    fclose(arquivo);
}

// Função para buscar um passageiro pelo código
Passageiro* buscarPassageiro(int codigo) {
    FILE *arquivo = fopen(ARQUIVO_PASSAGEIROS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de passageiros");
        return NULL;
    }

    static Passageiro passageiro; // Necessário ser static para retornar o ponteiro
    while (fread(&passageiro, sizeof(Passageiro), 1, arquivo)) {
        if (passageiro.codigo == codigo) {
            fclose(arquivo);
            return &passageiro;
        }
    }
    fclose(arquivo);
    return NULL; // Não encontrado
}

// Função para salvar os dados dos passageiros (já implementada no cadastrar)
void salvarPassageirosEmArquivo() {
    printf("Os dados dos passageiros são salvos automaticamente ao cadastrar.\n");
}

// Função para carregar os dados dos passageiros (opcional se já salvarmos diretamente)
void carregarPassageirosDeArquivo() {
    printf("Os dados dos passageiros são carregados diretamente ao abrir o sistema.\n");
}


/*-----------------------------------------------------------------CADASTRO ASSENTOS------------------------------------------------------------------------*/


// Arquivo para persistência dos dados
#define ARQUIVO_ASSENTOS "assentos.dat"

// Função para cadastrar um assento
int cadastrarAssento(Assento *novoAssento) {
    FILE *arquivo = fopen(ARQUIVO_ASSENTOS, "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de assentos");
        return 0;
    }

    fwrite(novoAssento, sizeof(Assento), 1, arquivo);
    fclose(arquivo);
    return 1; // Sucesso
}

// Função para listar todos os assentos de um voo
void listarAssentosPorVoo(int codigoVoo) {
    FILE *arquivo = fopen(ARQUIVO_ASSENTOS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de assentos");
        return;
    }

    Assento assento;
    printf("Assentos do Voo %d:\n", codigoVoo);
    while (fread(&assento, sizeof(Assento), 1, arquivo)) {
        if (assento.codigoVoo == codigoVoo) {
            printf("Número: %d, Status: %s\n",
                   assento.numero,
                   assento.status ? "Ocupado" : "Livre");
        }
    }
    fclose(arquivo);
}

// Função para verificar a disponibilidade de um assento
int verificarDisponibilidade(int codigoVoo, int numeroAssento) {
    FILE *arquivo = fopen(ARQUIVO_ASSENTOS, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de assentos");
        return 0;
    }

    Assento assento;
    while (fread(&assento, sizeof(Assento), 1, arquivo)) {
        if (assento.codigoVoo == codigoVoo && assento.numero == numeroAssento) {
            fclose(arquivo);
            return assento.status == 0; // Disponível se status == 0
        }
    }
    fclose(arquivo);
    return 0; // Não encontrado ou ocupado
}

// Função para salvar os dados dos assentos em um arquivo binário
void salvarAssentosEmArquivo() {
    printf("Os dados dos assentos são salvos automaticamente ao cadastrar.\n");
}

// Função para carregar os dados dos assentos de um arquivo binário
void carregarAssentosDeArquivo() {
    printf("Os dados dos assentos são carregados diretamente ao abrir o sistema.\n");
}


/*-----------------------------------------------------------Funções unidads---------------------------------------------------------------------*/

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
        CadastroTripulantes(&tripulantes);
        SalvarTripulante(&tripulantes);
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

    return 0;
}

void PASSAGEIROS() {
    int opcao;
    int codigo = 0;
    Passageiro passageiros[MAX_CODIGOS];
    int total_passageiros = 0;

    do {
        printf("\tCadastro de Passageiros\n");
        printf("\n");
        printf("1 - Cadastrar passageiro\n");
        printf("2 - Lista de passageiros\n");
        printf("3 - Buscar passageiro\n");
        printf("4 - Deletar passageiro\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\nEscolha uma das opções (Entre 0 e 4): ");
        scanf("%d", &opcao);
        printf("\n");

        while (getchar() != '\n'); // Limpar buffer de entrada

        int continuar = 1;
        switch (opcao) {
        case 0:
            break;
        case 1:
            while (continuar && total_passageiros < MAX_CODIGOS) {
                CadastroPassageiro(&passageiros[total_passageiros]);
                SalvarPassageiro(&passageiros[total_passageiros]);
                total_passageiros++;

                printf("\nDeseja cadastrar outro passageiro? (1 - Sim / 0 - Não): ");
                scanf("%d", &continuar);
                printf("\n");
                getchar();
            }
            break;
        case 2:
            ListarPassageiros();
            break;
        case 3:
            printf("Digite o código do passageiro a ser buscado: ");
            scanf("%d", &codigo);
            BuscarPassageiro(codigo);
            break;
        case 4:
            printf("Digite o código do passageiro a ser excluído: ");
            scanf("%d", &codigo);
            ExcluirPassageiro(codigo);
            break;
        default:
            printf("\nNúmero inválido, selecione novamente!\n");
            break;
        }
    } while (opcao != 0);
}

void ASSENTOS() {
    int opcao;
    int codigoVoo = 0, numeroAssento = 0;
    Assento assentos[MAX_CODIGOS];
    int total_assentos = 0;

    do {
        printf("\tCadastro de Assentos\n");
        printf("\n");
        printf("1 - Cadastrar assento\n");
        printf("2 - Lista de assentos por voo\n");
        printf("3 - Verificar disponibilidade de assento\n");
        printf("4 - Deletar assento\n");
        printf("0 - Voltar ao menu principal\n");
        printf("\nEscolha uma das opções (Entre 0 e 4): ");
        scanf("%d", &opcao);
        printf("\n");

        while (getchar() != '\n'); // Limpar buffer de entrada

        int continuar = 1;
        switch (opcao) {
        case 0:
            break;
        case 1:
            while (continuar && total_assentos < MAX_CODIGOS) {
                CadastroAssento(&assentos[total_assentos]);
                SalvarAssento(&assentos[total_assentos]);
                total_assentos++;

                printf("\nDeseja cadastrar outro assento? (1 - Sim / 0 - Não): ");
                scanf("%d", &continuar);
                printf("\n");
                getchar();
            }
            break;
        case 2:
            printf("Digite o código do voo: ");
            scanf("%d", &codigoVoo);
            ListarAssentosPorVoo(codigoVoo);
            break;
        case 3:
            printf("Digite o código do voo: ");
            scanf("%d", &codigoVoo);
            printf("Digite o número do assento: ");
            scanf("%d", &numeroAssento);
            if (VerificarDisponibilidade(codigoVoo, numeroAssento)) {
                printf("Assento disponível.\n");
            } else {
                printf("Assento ocupado ou inexistente.\n");
            }
            break;
        case 4:
            printf("Digite o número do assento a ser excluído: ");
            scanf("%d", &numeroAssento);
            ExcluirAssento(numeroAssento);
            break;
        default:
            printf("\nNúmero inválido, selecione novamente!\n");
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
    printf("6 - Assentos\n");
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
            //RESERVAS();
            break;
        case 5:
            //FIDELIDADE();
            break;
            case 6:
            ASSENTOS();
            break;
        default:
            printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    
    } while (opcao != 0);
    

    return 0;
}
