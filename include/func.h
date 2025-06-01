/**
 * @file func.h
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-05-18
 * @copyright Copyright (c) 2025
 */

#ifndef FUNC_H
#define FUNC_H

#include "estruturas.h"

Grafo *criarGrafo();
Antena *criarVertice(Grafo *grafo, int x, int y, char freq);
int criarAresta(Antena *origem, Antena *destino);

Grafo *carregarAntenas(const char *ficheiro);
int inserirArestas(Grafo *grafo);

Antena *procurarAntenaId(Grafo *grafo, int id);

dfslista *dfs(Antena *atual, dfslista *lista);
dfslista *execDfs(Grafo *grafo, int id_inicio);

int inicializarFila(Fila *f);
int enfileirar(Fila *f, Antena *a);
Antena *desenfileirar(Fila *f);
int filaVazia(Fila *f);
bfslista *execBfs(Grafo *grafo, int id_inicio);

int adicionarAoCamino(Caminho *c, int id_antena);
int removerDoCaminho(Caminho *c);
int dfsBacktracking(Antena *atual, Antena *destino, Caminho *caminho);
int encontrarCaminhoDFS(Grafo *grafo, int id_origem, int id_destino, Caminho *caminho);

int segmentosSeCruzam(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
interlista *detetarIntersecoes(Grafo *grafo);

int escreverBloco(char *tag, FILE *f);
int escreverAntenas(Grafo *grafo, FILE *f);
int escreverDFS(dfslista *dfs, FILE *f);
int escreverBFS(bfslista *bfs, FILE *f);
int escreverCaminho(Caminho *caminho, FILE *f);
int escreverIntersecoes(interlista *lista, FILE *f);
int lerFicheiroEscreverOutput(const char *nome_ficheiro);

int freeMem(dfslista *dfs, bfslista *bfs, interlista *ints);

#endif