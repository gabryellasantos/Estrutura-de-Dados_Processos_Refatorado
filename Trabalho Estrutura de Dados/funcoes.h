#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

#define MAX_LINE_LENGTH 20000
#define MAX_ID 18400

typedef struct{
    int id;
    char numero[25];
    char dataAjuizamento[30];
    char idClasse[30];
    char idAssunto[30];
    char anoEleicao[30];
} Processo;

typedef struct {
    char id_classe[50];
    int count;
} ClasseContagem;

typedef struct {
    char id_assunto[50];
} Assunto;

Processo* alocar_processos(int quantidade);
void liberar_processos(Processo* processos);
void leituraArquivo(Processo processos[], int *num_processos, const char *nome_arquivo);
void arquivoCSV(const char *nome_arquivo, Processo processos[], int num_processos);
void selecao_por_id(Processo processos[], int num_processos);
void selecao_por_data(Processo processos[], int num_processos);
void tempoTramitacao();
int encontrar_classe_aux(ClasseContagem classes[], int total_classes, char *id_classe);
void encontrar_classe();
int acharAssunto(char ids_assuntos[][50], int total_assuntos, char *id_assunto);
void indentifica_assuntos();
void contarAssuntos(FILE *file);
void indentifica_assuntos();
void menu_principal();

#endif // FUNCOES_H_INCLUDED
