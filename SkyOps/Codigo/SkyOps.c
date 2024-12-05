#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>


#define MAX 200
#define MIN_TEMPO 1000
#define MAX_TEMPO 5000
#define MAX_VOOS 100
#define MAX_VOOS 100
#define MAX_TRIPULACAO 5
#define MAX_LINHAS 100   
#define MAX_COLUNAS 10   
#define MAX_CODIGOS 100
#define MAX_PASSAGEIROS 100
#define MAX_PASSAGEIROS 100
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
    int ocupado;          // 0 = livre, 1 = ocupado
    char passageiro[50];  // Nome do passageiro, se ocupado
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
    
    Assento assentosVoo[MAX_LINHAS][MAX_COLUNAS];
    int linhas;   
    int colunas;
} Voo;

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
    Voo codigoVoo;
    int status; // 0 = desocupado, 1 = ocupado
    Passageiro nomePassageiro[MAX]; // PUXAR O NOME PELA STRUCT "Passageiro" = nomePassageiro.nome
} reserva;

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

void inicializarAssentos(Voo *voo) {
    for (int i = 0; i < voo->linhas; i++) {
        for (int j = 0; j < voo->colunas; j++) {
            voo->assentosVoo[i][j].ocupado = 0;
            strcpy(voo->assentosVoo[i][j].passageiro, "");
        }
    }
}

void exibirAssentos(Voo *voo) {
    if (!voo || voo->linhas <= 0 || voo->colunas <= 0) {
        printf("Erro: Voo inválido.\n");
        return;
    }

    printf("Mapa de Assentos do Voo %s:\n\n", voo->codigo);
    printf("    ");
    for (char c = 'A'; c < 'A' + voo->colunas; c++) {
        printf("%c ", c);
    }
    printf("\n");

    for (int i = 0; i < voo->linhas; i++) {
        printf("%2d |", i + 1);
        for (int j = 0; j < voo->colunas; j++) {
            printf(" %d ", voo->assentosVoo[i][j].ocupado);
        }
        printf("\n");
    }
}

int colunaParaIndice(char coluna, int colunas) {
    if (coluna >= 'A' && coluna < 'A' + colunas) {
        return coluna - 'A';
    } else if (coluna >= 'a' && coluna < 'a' + colunas) {
        return coluna - 'a'; // Suporte para letras minúsculas
    }
    return -1; // Letra inválida
}

int ocuparAssento(Voo *voo, int fileira, char coluna, char *nomePassageiro) {
    int colIdx = colunaParaIndice(coluna, voo->colunas);
    if (fileira < 1 || fileira > voo->linhas || colIdx == -1) {
        printf("Assento inválido! Fileira deve estar entre 1 e %d, e coluna entre A e %c.\n", voo->linhas, 'A' + voo->colunas - 1);
        return 0;
    }

    Assento *assento = &voo->assentosVoo[fileira - 1][colIdx];
    if (assento->ocupado) {
        printf("Assento %d%c já está ocupado por %s!\n", fileira, coluna, assento->passageiro);
        return 0;
    }

    assento->ocupado = 1;
    strcpy(assento->passageiro, nomePassageiro);
    printf("Assento %d%c ocupado com sucesso por %s!\n", fileira, coluna, nomePassageiro);
    return 1;
}

void selecionarAssento(Voo *meuVoo) {
    int opcao, fileira;
    char coluna;
    char nomePassageiro[100];

    if (!meuVoo || meuVoo->linhas <= 0 || meuVoo->colunas <= 0) {
        printf("Erro: Configuração inválida do voo.\n");
        return;
    }

    do {
        printf("\n1. Exibir mapa de assentos\n");
        printf("2. Ocupar um assento\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirAssentos(meuVoo);
                break;
            case 2:
                printf("Digite a fileira (1-%d): ", meuVoo->linhas);
                scanf("%d", &fileira);
                getchar(); // Limpar buffer

                printf("Digite a coluna (1-%d): ", 'A' + meuVoo->colunas - 1);
                scanf(" %c", &coluna);
                getchar(); // Limpar buffer

                printf("Digite o nome do passageiro: ");
                fgets(nomePassageiro, sizeof(nomePassageiro), stdin);
                nomePassageiro[strcspn(nomePassageiro, "\n")] = '\0';

                if (!ocuparAssento(meuVoo, fileira, coluna, nomePassageiro)) {
                    printf("Falha ao ocupar o assento. Tente novamente.\n");
                }
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
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
    strupr(novoVoo.origem); 

    printf("-----------------------------\n");
    printf("-Destino: ");
    scanf(" %[^\n]", novoVoo.destino);
    strupr(novoVoo.destino); 

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

    printf("Digite a quantidade de fileiras no avião:");
    scanf("%d", &novoVoo.linhas);
    printf("\nDigite a quantidade de assentos por fileira (1-6):");
    scanf("%d", &novoVoo.colunas);

    inicializarAssentos(&novoVoo);

    // Adicionar tripulação
    novoVoo.qtd_tripulacao = 0;
    int piloto_encontrado = 0, copiloto_encontrado = 0, opcao_comissario, codigo_tripulante, encontrado = 0;
    Tripulantes tripulante;
    int adicionar_tripulantes;

    while (piloto_encontrado == 0 || copiloto_encontrado == 0 || adicionar_tripulantes == 0) {
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
        int capacidadeTotal = voos[i].linhas * voos[i].colunas;
        printf("%d linhas %d colunas", voos[i].linhas,voos[i].colunas);
        printf("\nCapacidade total do avião: %d assentos\n", capacidadeTotal);
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
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
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
        if (scanf("%d", &novo_passageiro.codigo) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            while (getchar() != '\n');  // Limpa buffer em caso de entrada inválida
            continue;
        }
        if (verificaCodigoPassageiro(novo_passageiro.codigo)) {
            printf("Código já existe! Por favor, insira um código único.\n");
        }
    } while (verificaCodigoPassageiro(novo_passageiro.codigo));

    // Limpar buffer após leitura do código
    getchar();

    printf("\n____________________________________\n");

    // Leitura dos outros campos
    printf("Digite o nome do passageiro: ");
    fgets(novo_passageiro.nome, sizeof(novo_passageiro.nome), stdin);
    novo_passageiro.nome[strcspn(novo_passageiro.nome, "\n")] = '\0';
    ConverterPrimeiraMaiuscula(novo_passageiro.nome);
    printf("%s\n", novo_passageiro.nome);
    printf("\n____________________________________\n");

    printf("Digite o endereço do passageiro: ");
    fgets(novo_passageiro.endereco, sizeof(novo_passageiro.endereco), stdin);
    novo_passageiro.endereco[strcspn(novo_passageiro.endereco, "\n")] = '\0';

    printf("\n____________________________________\n");

    printf("Digite o telefone do passageiro: ");
    fgets(novo_passageiro.telefone, sizeof(novo_passageiro.telefone), stdin);
    novo_passageiro.telefone[strcspn(novo_passageiro.telefone, "\n")] = '\0';

    printf("\n____________________________________\n");

    // Inicializa os pontos de fidelidade
    novo_passageiro.pontos = 0;

    // Armazenar o passageiro no array
    passageiros[total_passageiros++] = novo_passageiro;

    // Salvar no arquivo binário
    salvarPassageiroNoArquivo(&novo_passageiro);

    printf("\nPassageiro cadastrado com sucesso!\n");
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

    fclose(arquivo);
    printf("Dados salvos com sucesso no arquivo reservas.bin!\n");
}

void cadastrarReserva() {
    char origem[50], destino[50];
    int vooEncontrado = 0;
    int indices[MAX]; // Para armazenar os índices dos voos encontrados

    printf("\n-----------------------------\n");
    printf("\tCadastro de Reserva:\n");

    // Limpa o buffer antes de ler a origem
    printf("\nInforme a ORIGEM da sua viagem: ");
    fgets(origem, sizeof(origem), stdin);
    origem[strcspn(origem, "\n")] = '\0';  // Remove o caractere de nova linha
    for (int i = 0; origem[i] != '\0'; i++) {
        origem[i] = toupper((unsigned char)origem[i]);
    }

    // Limpa o buffer antes de ler o destino
    printf("Informe o DESTINO da sua viagem: ");
    fgets(destino, sizeof(destino), stdin);
    destino[strcspn(destino, "\n")] = '\0';  // Remove o caractere de nova linha
    for (int i = 0; destino[i] != '\0'; i++) {
        destino[i] = toupper((unsigned char)destino[i]);
    }

    printf("\n-----------------------------\n");

    // Busca por voos que correspondem à origem e destino
    for (int i = 0; i < total_voos; i++) {
        if (strcmp(origem, voos[i].origem) == 0 && strcmp(destino, voos[i].destino) == 0) {
            printf("Voo %d\nCódigo: %s\nData: %s - Hora: %s\nTarifa: %.2f\n",
                   vooEncontrado + 1, voos[i].codigo, voos[i].data, voos[i].hora, voos[i].tarifa);
            indices[vooEncontrado++] = i; // Armazena o índice do voo encontrado
            printf("-----------------------------\n");
        }
    }

    // Se nenhum voo foi encontrado
    if (vooEncontrado == 0) {
        printf("Nenhum voo encontrado de %s para %s.\n", origem, destino);
        return;
    }

    // Solicitar ao usuário para escolher um voo
    int escolha;
    printf("\nEscolha um dos voos listados (1 a %d): ", vooEncontrado);
    scanf("%d", &escolha);
    getchar(); // Limpa o buffer após o scanf

    // Verificar se a escolha é válida
    if (escolha < 1 || escolha > vooEncontrado) {
        printf("\nEscolha inválida. Por favor, tente novamente.\n");
        return;
    }

    // Obter o índice do voo selecionado
    int indiceVooSelecionado = indices[escolha - 1];

    // Selecionar assento no voo escolhido
    selecionarAssento(&voos[indiceVooSelecionado]);
}

void cancelarReserva() {
    int vooEscolhido, fileira;
    char coluna;
    char nomePassageiro[100];

    printf("Digite o nome do passageiro: ");
    fgets(nomePassageiro, sizeof(nomePassageiro), stdin);
    nomePassageiro[strcspn(nomePassageiro, "\n")] = '\0';

    printf("Digite o número do voo (1 a %d): ", total_voos);
    scanf("%d", &vooEscolhido);
    getchar(); // Limpar buffer

    if (vooEscolhido < 1 || vooEscolhido > total_voos) {
        printf("Voo inválido.\n");
        return;
    }

    printf("Digite a fileira: ");
    scanf("%d", &fileira);
    printf("Digite a coluna: ");
    scanf(" %c", &coluna);

    Voo *voo = &voos[vooEscolhido - 1];
    int colIdx = colunaParaIndice(coluna, voo->colunas);

    if (fileira < 1 || fileira > voo->linhas || colIdx == -1) {
        printf("Assento inválido.\n");
        return;
    }

    Assento *assento = &voo->assentosVoo[fileira - 1][colIdx];
    if (!assento->ocupado || strcmp(assento->passageiro, nomePassageiro) != 0) {
        printf("Assento não está reservado por %s.\n", nomePassageiro);
        return;
    }

    // Cancelar a reserva
    assento->ocupado = 0;
    strcpy(assento->passageiro, "");
    printf("Reserva cancelada com sucesso!\n");
}

void buscarReserva() {
    char nomePassageiro[100];
    printf("Digite o nome do passageiro: ");
    fgets(nomePassageiro, sizeof(nomePassageiro), stdin);
    nomePassageiro[strcspn(nomePassageiro, "\n")] = '\0'; // Remove o '\n' do final da string

    int encontrado = 0;

    for (int i = 0; i < total_voos; i++) {
        for (int j = 0; j < voos[i].linhas * voos[i].colunas; j++) {
            // Verifica se o assento está ocupado e se o nome corresponde ao passageiro
            if (voos[i].assentosVoo[j / voos[i].colunas][j % voos[i].colunas].ocupado == 1 &&
                strcmp(voos[i].assentosVoo[j / voos[i].colunas][j % voos[i].colunas].passageiro, nomePassageiro) == 0) {
                encontrado = 1;
                
                // Converte o índice linear para linha e coluna
                int fileira = j / voos[i].colunas + 1;
                char coluna = 'A' + (j % voos[i].colunas);

                // Exibe os detalhes da reserva
                printf("Reserva encontrada: Voo %s | Assento %d%c | Origem: %s | Destino: %s\n",
                       voos[i].codigo, fileira, coluna, voos[i].origem, voos[i].destino);
            }
        }
    }

    if (!encontrado) {
        printf("Nenhuma reserva encontrada para o passageiro %s.\n", nomePassageiro);
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
        CadastroTripulantes(&tripulantes[total_tripulantes]);
        SalvarTripulante(&tripulantes[total_tripulantes]);
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
    } while (opcao != 0);

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

void gerenciarReservas() {
    int opcao;

    do {
        printf("\n\n=== Gerenciamento de Reservas ===\n");
        printf("1 - Cadastro de Reserva\n");
        printf("2 - Cancelamento de Reserva\n");
        printf("3 - Buscar Reserva\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Por favor, insira um numero.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }
        getchar(); // Limpar buffer para próximas leituras

        switch (opcao) {
            case 1:
                cadastrarReserva();
                break;

            case 2:
                cancelarReserva();
                break;

            case 3:
                buscarReserva();
                break;

            case 0:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("Opcao invalida! Por favor, escolha entre 0 e 3.\n");
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
    printf("2 - Voos\n");
    printf("3 - Passageiros\n");
    printf("4 - Reservas\n");
    printf("0 - SAIR\n");

    printf("\nEscolha uma das opcoes (Entre 0 e 4): ");
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
            PASSAGEIRO();
            break;
        case 4:
            gerenciarReservas();
            break;
        default:
            printf("\nNumero invalido, selecione novamente!\n");
            break;
        }
    
    } while (opcao != 0);
    

    return 0;
}
