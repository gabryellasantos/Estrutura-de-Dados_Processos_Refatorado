#include "funcoes.h"

Processo* alocarMemoria(int quantidade) {
    Processo *processos = (Processo*) malloc(quantidade * sizeof(Processo));
    if (processos == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    return processos;
}

void liberarMemoria(Processo* processos) {
    free(processos);
}

void leituraArquivo(Processo processos[], int *num_processos, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[MAX_LINE_LENGTH];
    int i = 0;

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_ID) {

        char *token = strtok(linha, ",");

        processos[i].id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(processos[i].numero, token, sizeof(processos[i].numero) - 1);

        token = strtok(NULL, ",");
        strncpy(processos[i].dataAjuizamento, token, sizeof(processos[i].dataAjuizamento) - 1);

        token = strtok(NULL, ",");
        strncpy(processos[i].idClasse, token, sizeof(processos[i].idClasse) - 1);

        token = strtok(NULL, ",");
        strncpy(processos[i].idAssunto, token, sizeof(processos[i].idAssunto) - 1);

        token = strtok(NULL, ",\n");
        strncpy(processos[i].anoEleicao, token, sizeof(processos[i].anoEleicao) - 1);

        i++;
    }

    *num_processos = i;
    fclose(arquivo);
}

void arquivoCSV(const char *nome_arquivo, Processo processos[], int num_processos) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "ID,Numero,DataAjuizamento,IdClasse,IdAssunto,AnoEleicao\n");

    for (int i = 0; i < num_processos; i++) {
        fprintf(arquivo, "%d,%s,%s,%s,%s,%s\n",
            processos[i].id,
            processos[i].numero,
            processos[i].dataAjuizamento,
            processos[i].idClasse,
            processos[i].idAssunto,
            processos[i].anoEleicao);
    }

    fclose(arquivo);
    printf("\nDados salvos no arquivo: %s.\n", nome_arquivo);
}

void selecao_por_id(Processo processos[], int num_processos) {
    for (int i = 0; i < num_processos - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < num_processos; j++) {
            if (processos[j].id < processos[minIndex].id) {
                minIndex = j;
            }
        }
        Processo temp = processos[minIndex];
        processos[minIndex] = processos[i];
        processos[i] = temp;
    }
}

void selecao_por_data(Processo processos[], int num_processos) {
    for (int i = 0; i < num_processos - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < num_processos; j++) {
            if (strcmp(processos[j].dataAjuizamento, processos[maxIndex].dataAjuizamento) > 0) {
                maxIndex = j;
            }
        }
        Processo temp = processos[maxIndex];
        processos[maxIndex] = processos[i];
        processos[i] = temp;
    }

}

void tempoTramitacao() {
    FILE *file = fopen("processo_043_202409032338.csv", "r");
    if (file==NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[MAX_LINE_LENGTH], ajuizamento[20], numero_processo[30];
    struct tm data_aj = {0};
    time_t t_atual = time(NULL);

    while (fgets(linha, MAX_LINE_LENGTH, file)) {

        sscanf(linha, "%*[^,],%29[^,],%19[^,]", numero_processo, ajuizamento);

        sscanf(ajuizamento, "%d-%d-%d %d:%d:%d", &data_aj.tm_year, &data_aj.tm_mon, &data_aj.tm_mday, &data_aj.tm_hour, &data_aj.tm_min, &data_aj.tm_sec);

       data_aj.tm_year -= 1900;
       data_aj.tm_mon -= 1;

        time_t inicio_proc = mktime(&data_aj);
        int dias_tramitacao = (difftime(t_atual, inicio_proc) / (86400));

        printf("Processo: %s   Tempo em tramitacao: %d dias.\n", numero_processo, dias_tramitacao);
    }
    fclose(file);
}

int encontrar_classe_aux(ClasseContagem classes[], int total_classes, char *id_classe) {
	int i;
    for ( i = 0; i < total_classes; i++) {
        if (strcmp(classes[i].id_classe, id_classe) == 0) {
            return i;
        }
    }
    return -1;
}

void encontrar_classe() {
    FILE *file = fopen("processo_043_202409032338.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[MAX_LINE_LENGTH];
    ClasseContagem classes[MAX_ID];
    int total_classes = 0;
    int coluna_classe = 3;

    fgets(linha, MAX_LINE_LENGTH, file);

    while (fgets(linha, MAX_LINE_LENGTH, file)) {
        char *token;
        int coluna_atual = 0;
        char id_classe_atual[50];

        token = strtok(linha, ",");

        while (token != NULL) {
            if (coluna_atual == coluna_classe) {

                strcpy(id_classe_atual, token);
                break;
            }
            token = strtok(NULL, ",");
            coluna_atual++;
        }

        id_classe_atual[strcspn(id_classe_atual, "\n")] = 0;

        int indice = encontrar_classe_aux(classes, total_classes, id_classe_atual);

        if (indice == -1) {
            strcpy(classes[total_classes].id_classe, id_classe_atual);
            classes[total_classes].count = 1;
            total_classes++;
        } else {
            classes[indice].count++;
        }
    }
    fclose(file);

    printf("Contagem de processos por id_classe:\n");
    int i;
    for ( i = 0; i < total_classes; i++) {
        printf("%s: %d\n", classes[i].id_classe, classes[i].count);
    }
}

int acharAssunto(char ids_assuntos[][50], int total_assuntos, char *id_assunto) {
    for (int i = 0; i < total_assuntos; i++) {
        if (strcmp(ids_assuntos[i], id_assunto) == 0) return 1;
    }
    return 0;
}

void indentifica_assuntos_aux(FILE *file) {
    char linha[MAX_LINE_LENGTH], ids_assuntos[MAX_ID][50];
    int total_assuntos = 0, coluna = 4;

    fgets(linha, MAX_LINE_LENGTH, file);

    while (fgets(linha, MAX_LINE_LENGTH, file)) {
        char *token = strtok(linha, ",");
        int current_column = 0;

        while (token && current_column++ != coluna) {
            token = strtok(NULL, ",");
        }

        if (token) {
            token[strcspn(token, "\n")] = 0;
            if (!acharAssunto(ids_assuntos, total_assuntos, token)) {
                strcpy(ids_assuntos[total_assuntos++], token);
            }
        }
    }
    printf("Constam %d assuntos diferentes no arquivo\n", total_assuntos);
}

void contarAssuntos(FILE *file) {
    char linha[MAX_LINE_LENGTH];
    int total = 0, coluna = 4;

    fgets(linha, MAX_LINE_LENGTH, file);

    while (fgets(linha, MAX_LINE_LENGTH, file)) {
        char *token = strtok(linha, ",");
        int coluna_atual = 0;

        while (token && coluna_atual < coluna) {
            token = strtok(NULL, ",");
            coluna_atual++;
        }

        if (token != NULL) {
            total++;
        }
    }
    printf("Total de 'idAssunto' no arquivo: %d\n", total);
}

void indentifica_assuntos() {
    const char *nome_arquivo = "processo_043_202409032338.csv";
    FILE *file = fopen(nome_arquivo, "r");

    if (!file) {
        printf("Erro na abertura do arquivo!\n");
        exit(1);
    }

    indentifica_assuntos_aux(file);
    rewind(file);
    contarAssuntos(file);

    fclose(file);
}

void menu_principal() {
    int opcao;
    int num_processos;
    Processo* processos = alocarMemoria(18400);

    leituraArquivo(processos, &num_processos, "processo_043_202409032338.csv");

    do {
    	system("cls");
        printf("Menu Principal:\n");
        printf("1. Ordenar processos por ID (em ordem crescente)\n");
        printf("2. Ordenar processos por Data de Ajuizamento (em ordem decrescente)\n");
        printf("3. Verificar a quantos dias um processo esta em tramitacao.\n");
        printf("4. Contar quantos processos estao vinculados a uma determinada classe.\n");
        printf("5. Identificar quantos assuntos constam nos processos presentes na base de dados.\n");
        printf("6. Sair\n");
        printf("Digite o numero da opcao desejada: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                system("cls");
                selecao_por_id(processos, num_processos);
                printf("\nProcessos ordenados.");
                arquivoCSV("Processos ordenados por ID.csv", processos, num_processos);
                getch();
                break;
            case 2:
                system("cls");
                selecao_por_data(processos, num_processos);
                printf("\nProcessos ordenados.");
                arquivoCSV("Processos ordenados por Data de Ajuizamento.csv", processos, num_processos);
                getch();
                break;
            case 3:
            	system("cls");
            	tempoTramitacao();
            	getch();
            	break;
            case 4:
            	system("cls");
            	encontrar_classe();
            	getch();
            	break;
            case 5:
            	system("cls");
            	indentifica_assuntos();
            	getch();
            	break;
            case 6:
                system("cls");
                printf("\nSaindo do programa.\n");
                liberarMemoria(processos);
                Sleep(500);
                return;
            default:
                printf("\nOpcao invalida. Digite o numero de alguma opcao presente no menu:\n");
                break;
        }
    } while (opcao != 3);
}
