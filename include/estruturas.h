/**
 * @file antena.h
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-05-18
 * @copyright Copyright (c) 2025
 */

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

/**
 * @brief Estrutura de uma antena (vértice)
 */

typedef struct Antena
{
    int x, y;            // coordenadas da antena
    char freq;           // frequência
    int visitado;        // inteiro para verificar se foi visitado
    int id;              // id
    struct Aresta *adj;  // lista de adjacência (arestas)
    struct Antena *next; // pointer prox antena
} Antena;

/**
 * @brief Estrutura de uma aresta
 */

typedef struct Aresta
{
    struct Antena *destino; // proxima antena
    struct Aresta *next;    // pointer prox aresta
} Aresta;

/**
 * @brief Estrutura do Gráfo
 */

typedef struct Grafo
{
    int V;                   //    número de vértices
    struct Antena *vertices; //    pointer para a lista ligada de antenas (head)
} Grafo;

/**
 * @brief Estrutura de uma lista de resultados da pesquisa em profundidade (dfs)
 */

typedef struct dfslista
{
    Antena *antena;        // antena
    struct dfslista *prox; //   pointer prox antena
} dfslista;

/**
 * @brief Estrutura de uma lista de resultados da pesquisa em largura (bfs)
 */

typedef struct bfslista
{
    Antena *antena;        //    antena
    struct bfslista *prox; //   pointer prox antena
} bfslista;

/**
 * @brief Estrutura de uma Fila (lista dinámica para bfs)
 */

typedef struct Fila
{
    bfslista *inicio; // inicio da fila
    bfslista *fim;    // fim da fila
} Fila;

/**
 * @brief Estrutura de um caminho (lista ligada)
 */

typedef struct Caminho
{
    int tamanho;            // número de saltos em um caminho
    struct NoCaminho *head; // inicio da lista
} Caminho;

/**
 * @brief Estrutura de um Nó de um caminho
 */

typedef struct NoCaminho
{
    int id_antena;          // id da antena no nó do caminho
    struct NoCaminho *prox; // pointer para o prox
} NoCaminho;

/**
 * @brief Estrutura de uma lista de resultados de interseção
 */

typedef struct interlista
{
    int x1, y1, x2, y2;      //   coordenadas do primeiro segmento
    int x3, y3, x4, y4;      //   coordenadas do segundo segmento
    char freq;               // frequencia das antenas
    struct interlista *prox; // pointer para o prox
} interlista;

#endif