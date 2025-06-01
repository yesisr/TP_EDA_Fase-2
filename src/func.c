/**
 * @file func.c
 * @author Bruno (a31496@alunos.ipca.pt)
 * @brief
 * @version 1.2
 * @date 2025-05-18
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/estruturas.h"
#include "../include/func.h"

#pragma region Inicialização

/**
 * @brief   Função para criar e inicializar um grafo
 * @return  grafo criado
 */

Grafo *criarGrafo()
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));

    grafo->vertices = NULL;
    grafo->V = 0;

    return grafo;
}

/**
 * @brief Função para criar uma antena/vértice no grafo
 * @param grafo grafo para inserir as antena
 * @param x  coordenada x da antena
 * @param y  coordenada y da antena
 * @param freq  frequência da antena
 * @return pointer para a antena criada
 */

Antena *criarVertice(Grafo *grafo, int x, int y, char freq)
{
    Antena *temp = malloc(sizeof(Antena));

    temp->x = x;
    temp->y = y;
    temp->freq = freq;
    temp->adj = NULL;
    temp->visitado = 0;
    temp->next = grafo->vertices;
    temp->id = grafo->V++;

    grafo->vertices = temp;

    return temp;
}

/**
 * @brief   Função para criar uma aresta para ligar duas antenas
 * @param origem    antena de origem
 * @param destino   antena de destino
 * @return 1 = sucesso
 */

int criarAresta(Antena *origem, Antena *destino)
{
    // aresta origem -> destino
    Aresta *ar1 = malloc(sizeof(Aresta));
    ar1->destino = destino;
    ar1->next = origem->adj;
    origem->adj = ar1;

    // aresta destino -> origem
    Aresta *ar2 = malloc(sizeof(Aresta));
    ar2->destino = origem;
    ar2->next = destino->adj;
    destino->adj = ar2;

    return 1;
}

#pragma endregion

#pragma region Inserir e Carregamento

/**
 * @brief   Função principal de carregamento de antenas
 * @param ficheiro  nome do ficheiro txt com antenas
 * @return grafo com antenas inseridas
 */

Grafo *carregarAntenas(const char *ficheiro)
{
    FILE *file = fopen(ficheiro, "r");
    if (file == NULL)
        return NULL;

    Grafo *grafo = criarGrafo();
    int x = 0, y = 0;
    int c;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            y++;
            x = 0; // nova linha, reinicia x
        }
        else
        {
            if (c != '.')
            {
                criarVertice(grafo, x, y, c);
            }
            x++; // próximo caractere na linha
        }
    }

    fclose(file);
    return grafo;
}

/**
 * @brief   Função para criar uma aresta de 2 antenas com a mesma função
 * @param grafo grafo com antenas inseridas
 * @return 1 = sucesso
 */

int inserirArestas(Grafo *grafo)
{
    Antena *a = grafo->vertices;

    while (a != NULL)
    {
        Antena *b = a->next;

        while (b != NULL)
        {
            if (a->freq == b->freq)
            {
                criarAresta(a, b);
            }

            b = b->next;
        }
        a = a->next;
    }
    return 1;
}

#pragma endregion

/**
 * @brief   Função para procurar uma antena no grafo pelo id
 * @param grafo grafo com as antenas
 * @param id    id da antena
 * @return antena pedida
 */

Antena *procurarAntenaId(Grafo *grafo, int id)
{
    Antena *atual = grafo->vertices;

    while (atual != NULL)
    {
        if (atual->id == id)
            return atual;
        atual = atual->next;
    }

    return NULL;
}

#pragma region dfs

/**
 * @brief Função para fazer pesquisa em profundidade (DFS)
 * @param atual antena atual
 * @param lista lista de antenas já visitadas
 * @return lista com todas as antenas alcançadas pelo dfs
 */

dfslista *dfs(Antena *atual, dfslista *lista)
{
    //  se já foi visitado ou não existe ignora
    if (!atual || atual->visitado)
        return lista;

    atual->visitado = 1;

    //  adiciona a antena à lista
    dfslista *novo = malloc(sizeof(dfslista));
    novo->antena = atual;
    novo->prox = lista; //  faz a inserção no inicio da lista
    lista = novo;

    // faz a pesquisa pelos visinhos recursivamente
    for (Aresta *ar = atual->adj; ar != NULL; ar = ar->next)
    {
        lista = dfs(ar->destino, lista); //  atualiza a lista com visinhos
    }

    return lista;
}

/**
 * @brief   Função para executar o dfs através de um id de antena
 * @param grafo grafo com antenas
 * @param id_inicio id da antena que começa a pesquisa
 * @return lista com todos os pontos alcançados pelo dfs
 */

dfslista *execDfs(Grafo *grafo, int id_inicio)
{
    // reseta todas as antenas e marca como não visitado
    for (Antena *a = grafo->vertices; a != NULL; a = a->next)
    {
        a->visitado = 0;
    }

    Antena *inicio = procurarAntenaId(grafo, id_inicio);

    return dfs(inicio, NULL); //  null pois vai criar a lista
}

#pragma endregion

#pragma region bfs

/**
 * @brief Função para inicializar a estrutura de uma fila vazia
 * @param f fila que será inicializada
 * @return 1 = sucesso
 */

int inicializarFila(Fila *f)
{
    f->inicio = f->fim = NULL;
    return 1;
}

/**
 * @brief Função que insere uma antena no final da fila
 * @param f fila
 * @param a antena que será inserida
 * @return 1 = sucesso
 */

int enfileirar(Fila *f, Antena *a)
{
    bfslista *novo = malloc(sizeof(bfslista));
    novo->antena = a;
    novo->prox = NULL;

    if (f->fim == NULL) //  fila vazia
    {
        f->inicio = f->fim = novo;
    }
    else
    {
        f->fim->prox = novo;
        f->fim = novo;
    }
    return 1;
}

/**
 * @brief   Função para remover a antena do inicio da lista
 * @param f fila
 * @return antena do inicio da lista
 */

Antena *desenfileirar(Fila *f)
{
    if (f->inicio == NULL)
        return NULL;

    bfslista *temp = f->inicio;
    Antena *a = temp->antena;
    f->inicio = f->inicio->prox;

    if (f->inicio == NULL)
    {
        f->fim = NULL;
    }

    free(temp);
    return a;
}

/**
 * @brief   verifica se a fila está vazia
 * @param f fila
 * @return 1 se está vazia, caso contrario 0
 */

int filaVazia(Fila *f)
{
    return f->inicio == NULL;
}

/**
 * @brief   Função para fazer pesquisa em largura (bfs) com id de uma antena
 * @param grafo grafo com as antenas
 * @param id_inicio id da antena inicial
 * @return lista com as antenas visitadas pelo bfs
 */

bfslista *execBfs(Grafo *grafo, int id_inicio)
{
    // reseta todas as antenas e marca como não visitado
    for (Antena *a = grafo->vertices; a != NULL; a = a->next)
    {
        a->visitado = 0;
    }

    // encontra antena inicial
    Antena *inicio = procurarAntenaId(grafo, id_inicio);
    if (!inicio)
        return NULL;

    // inicializa fila e lista de resultados
    Fila f;
    inicializarFila(&f);
    bfslista *resultado = NULL;

    // marca o inicio como visitado e enfileira
    inicio->visitado = 1;
    enfileirar(&f, inicio);

    while (!filaVazia(&f))
    {
        Antena *atual = desenfileirar(&f); // remove a antena atual da fila

        //  guarda a antena atual na lista bfs
        bfslista *novo = malloc(sizeof(bfslista));
        novo->antena = atual;
        novo->prox = resultado;
        resultado = novo;

        // enfileira (insere no inicio da lista) vizinhos não visitados
        for (Aresta *ar = atual->adj; ar != NULL; ar = ar->next)
        {
            Antena *vizinho = ar->destino;
            if (!vizinho->visitado)
            {
                vizinho->visitado = 1;
                enfileirar(&f, vizinho);
            }
        }
    }

    return resultado;
}

#pragma endregion

#pragma region caminho

/**
 * @brief Função para adicionar uma nova antena (id) a um caminho
 * @param c caminho
 * @param id_antena id da antena
 * @return 1 = sucesso
 */

int adicionarAoCamino(Caminho *c, int id_antena)
{
    NoCaminho *novo = malloc(sizeof(NoCaminho));
    novo->id_antena = id_antena;
    novo->prox = c->head;
    c->head = novo;
    c->tamanho++;

    return 1;
}

/**
 * @brief   Função para remover uma antena (id) de um caminho
 * @param c caminho
 * @return 1 = sucesso
 */

int removerDoCaminho(Caminho *c)
{
    if (c->head == NULL)
        return 0;

    NoCaminho *temp = c->head;
    c->head = c->head->prox;
    free(temp);
    c->tamanho--;

    return 1;
}

/**
 * @brief   Função dfs para procurar caminhos
 * @param atual antena atual
 * @param destino antena destino
 * @param caminho   lista do caminho
 * @return 1 = caminho encontrado, 0 caso contrario
 */

int dfsBacktracking(Antena *atual, Antena *destino, Caminho *caminho)
{
    if (!atual || atual->visitado)
        return 0;

    // marca como visitado
    atual->visitado = 1;
    adicionarAoCamino(caminho, atual->id);

    // verifica se chegou ao destino
    if (atual == destino)
        return 1;

    // visita os vizinhos
    for (Aresta *ar = atual->adj; ar != NULL; ar = ar->next)
    {
        if (dfsBacktracking(ar->destino, destino, caminho))
        {
            return 1; // caminho encontrado
        }
    }

    // se nenhum vizinho leva ao destino, desfaz o passo atual (backtracking)
    removerDoCaminho(caminho);
    return 0;
}

/**
 * @brief Função principal para procurar caminhos entre 2 antenas no grafo
 * @param grafo grafo com antenas
 * @param id_origem id da antena de origem
 * @param id_destino id da antena de origem
 * @param caminho lista do caminho
 * @return 1 = caminho encontrado, 0 caso contrario
 */

int encontrarCaminhoDFS(Grafo *grafo, int id_origem, int id_destino, Caminho *caminho)
{
    // resetar visitados
    for (Antena *a = grafo->vertices; a != NULL; a = a->next)
    {
        a->visitado = 0;
    }

    // encontrar antenas de origem e destino
    Antena *origem = procurarAntenaId(grafo, id_origem);
    Antena *destino = procurarAntenaId(grafo, id_destino);

    if (!origem || !destino)
        return 0;

    // iniciar caminho
    caminho->head = NULL;
    caminho->tamanho = 0;

    // procurar o caminho
    return dfsBacktracking(origem, destino, caminho);
}

#pragma endregion

#pragma region interseção

/**
 * @brief   Função para verificar se 2 segmentos se cruzam
 * @param x1    coordenada x do primeiro ponto
 * @param y1    coordenada y do primeiro ponto
 * @param x2    coordenada x do segundo ponto
 * @param y2    coordenada y do segundo ponto
 * @param x3    coordenada x do terceiro ponto
 * @param y3    coordenada y do terceiro ponto
 * @param x4    coordenada x do quarto ponto
 * @param y4    coordenada y do quarto ponto
 * @return      1 se os segmentos (x1,y1)-(x2,y2) e (x3,y3)-(x4,y4) se cruzam, 0 caso contrário
 */

int segmentosSeCruzam(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    // calcula os 4 determinantes (orientações relativas)
    int d1 = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    int d2 = (x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1);
    int d3 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    int d4 = (x4 - x3) * (y2 - y3) - (y4 - y3) * (x2 - x3);

    // orientações opostas (interseção não colinear)
    if ((d1 * d2 < 0) && (d3 * d4 < 0))
        return 1;

    // verifica se algum ponto está dentro do segmento
    if (d1 == 0 && (x3 >= fmin(x1, x2)) && (x3 <= fmax(x1, x2)) && (y3 >= fmin(y1, y2)) && (y3 <= fmax(y1, y2)))
        return 1;
    if (d2 == 0 && (x4 >= fmin(x1, x2)) && (x4 <= fmax(x1, x2)) && (y4 >= fmin(y1, y2)) && (y4 <= fmax(y1, y2)))
        return 1;
    if (d3 == 0 && (x1 >= fmin(x3, x4)) && (x1 <= fmax(x3, x4)) && (y1 >= fmin(y3, y4)) && (y1 <= fmax(y3, y4)))
        return 1;
    if (d4 == 0 && (x2 >= fmin(x3, x4)) && (x2 <= fmax(x3, x4)) && (y2 >= fmin(y3, y4)) && (y2 <= fmax(y3, y4)))
        return 1;

    return 0; // sem interseção
}

/**
 * @brief   Função para detetar interseções em um grafo
 * @param grafo grafo com as antenas
 * @return Lista com os pontos que se intersetam
 */

interlista *detetarIntersecoes(Grafo *grafo)
{
    interlista *head = NULL;

    //  precorre cada antena do grafo
    for (Antena *a1 = grafo->vertices; a1 != NULL; a1 = a1->next)
    {
        //  para cada antena percorre as arestas
        for (Aresta *ar1 = a1->adj; ar1 != NULL; ar1 = ar1->next)
        {
            Antena *b1 = ar1->destino;

            // evita o mesmo par de antenas
            if (a1->id >= b1->id)
                continue;

            //  precorre todas as antenas novamente para procurar um segundo segmento
            for (Antena *a2 = grafo->vertices; a2 != NULL; a2 = a2->next)
            {
                //  evita frequências iguais
                if (a1->freq == a2->freq)
                    continue;

                //  precorre as arestas do segundo segmento
                for (Aresta *ar2 = a2->adj; ar2 != NULL; ar2 = ar2->next)
                {
                    Antena *b2 = ar2->destino;

                    // evita o mesmo par de antenas
                    if (a2->id >= b2->id)
                        continue;

                    //  ignora segmentos com as mesmas antenas
                    if (a1 == a2 || a1 == b2 || b1 == a2 || b1 == b2)
                        continue;

                    //  verifica se os segmentos se cruzam
                    if (segmentosSeCruzam(a1->x, a1->y, b1->x, b1->y, a2->x, a2->y, b2->x, b2->y))
                    {
                        //  cria um novo nó na lista de inter
                        interlista *nova = malloc(sizeof(interlista));
                        nova->x1 = a1->x;
                        nova->y1 = a1->y;
                        nova->x2 = b1->x;
                        nova->y2 = b1->y;
                        nova->x3 = a2->x;
                        nova->y3 = a2->y;
                        nova->x4 = b2->x;
                        nova->y4 = b2->y;
                        nova->freq = a1->freq; // frequência é igual para as duas
                        nova->prox = head;
                        head = nova; // adiciona à lista
                    }
                }
            }
        }
    }
    return head;
}

#pragma endregion

#pragma region binário

/**
 * @brief Função para escrever um bloco em um ficheiro bin
 * @param tag tag de 3 caracteres para identificar os dados exmpl "ANT" - antenas
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverBloco(char *tag, FILE *f)
{
    fwrite(tag, sizeof(char), 3, f);
    return 1;
}

/**
 * @brief   Função para escrever antenas e os seus dados em um ficheiro bin
 * @param grafo grafo com antenas
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverAntenas(Grafo *grafo, FILE *f)
{
    escreverBloco("ANT", f);
    fwrite(&grafo->V, sizeof(int), 1, f);

    Antena *atual = grafo->vertices;
    while (atual != NULL)
    {
        fwrite(&atual->id, sizeof(int), 1, f);
        fwrite(&atual->x, sizeof(int), 1, f);
        fwrite(&atual->y, sizeof(int), 1, f);
        fwrite(&atual->freq, sizeof(char), 1, f);
        atual = atual->next;
    }
    return 1;
}

/**
 * @brief   Função para escrever pontos encontrados pelo bfs em um ficheiro bin
 * @param dfs lista de pontos encontrados pelo dfs
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverDFS(dfslista *dfs, FILE *f)
{
    escreverBloco("DFS", f);
    for (dfslista *node = dfs; node != NULL; node = node->prox)
    {
        Antena *a = node->antena;
        fwrite(&a->x, sizeof(int), 1, f);
        fwrite(&a->y, sizeof(int), 1, f);
        fwrite(&a->freq, sizeof(char), 1, f);
    }
    return 1;
}

/**
 * @brief   Função para escrever pontos encontrados pelo bfs em um ficheiro bin
 * @param bfs lista de pontos encontrados pelo bfs
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverBFS(bfslista *bfs, FILE *f)
{
    escreverBloco("BFS", f);
    for (bfslista *node = bfs; node != NULL; node = node->prox)
    {
        Antena *a = node->antena;
        fwrite(&a->x, sizeof(int), 1, f);
        fwrite(&a->y, sizeof(int), 1, f);
        fwrite(&a->freq, sizeof(char), 1, f);
    }
    return 1;
}

/**
 * @brief   Função para escrever caminhos em um ficheiro bin
 * @param caminho lista de caminho
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverCaminho(Caminho *caminho, FILE *f)
{
    escreverBloco("CAM", f);
    fwrite(&caminho->tamanho, sizeof(int), 1, f);
    for (NoCaminho *node = caminho->head; node != NULL; node = node->prox)
    {
        fwrite(&node->id_antena, sizeof(int), 1, f);
    }
    return 1;
}

/**
 * @brief Função para escrever interseçoes em um ficheiro bin
 * @param lista lista de inter
 * @param f ficheiro binário aberto
 * @return 1 = sucesso
 */

int escreverIntersecoes(interlista *lista, FILE *f)
{
    escreverBloco("INT", f);
    for (interlista *i = lista; i != NULL; i = i->prox)
    {
        fwrite(&i->x1, sizeof(int), 1, f);
        fwrite(&i->y1, sizeof(int), 1, f);
        fwrite(&i->x2, sizeof(int), 1, f);
        fwrite(&i->y2, sizeof(int), 1, f);
        fwrite(&i->x3, sizeof(int), 1, f);
        fwrite(&i->y3, sizeof(int), 1, f);
        fwrite(&i->x4, sizeof(int), 1, f);
        fwrite(&i->y4, sizeof(int), 1, f);
        fwrite(&i->freq, sizeof(char), 1, f);
    }
    return 1;
}

/**
 * @brief Função para ler e escrever na consola o conteudo do ficheiro binário
 * @param nome_ficheiro nome do ficheiro binário
 * @return 1 = sucesso
 */

int lerFicheiroEscreverOutput(const char *nome_ficheiro)
{
    FILE *f = fopen(nome_ficheiro, "rb"); // read-binary
    if (!f)
    {
        // perror("Erro ao abrir ficheiro binário");
        return 0;
    }

    char bloco[4] = {0}; //  bloco de 3 caractéres mais o terminador '/n'

    while (fread(bloco, sizeof(char), 3, f) == 3)
    {
        bloco[3] = '\0';

        //  Antena
        if (strcmp(bloco, "ANT") == 0)
        {
            int total;
            fread(&total, sizeof(int), 1, f);
            //("=== ANTENAS (%d) ===\n", total);

            for (int i = 0; i < total; i++)
            {
                int id, x, y;
                char freq;

                fread(&id, sizeof(int), 1, f);
                fread(&x, sizeof(int), 1, f);
                fread(&y, sizeof(int), 1, f);
                fread(&freq, sizeof(char), 1, f);

                printf("Antena ID %d: (%d, %d) - %c\n", id, x, y, freq);
            }
        }

        // DFS
        else if (strcmp(bloco, "DFS") == 0)
        {
            printf("=== DFS ===\n");
            int x, y;
            char freq;
            while (fread(&x, sizeof(int), 1, f) == 1 &&
                   fread(&y, sizeof(int), 1, f) == 1 &&
                   fread(&freq, sizeof(char), 1, f) == 1)
            {
                printf("Antena DFS: (%d, %d) - %c\n", x, y, freq);

                long pos = ftell(f); // salva a pos atual
                char next[4] = {0};
                fread(next, sizeof(char), 3, f);

                fseek(f, pos, SEEK_SET); // volta posição anterior

                if (strncmp(next, "BFS", 3) == 0 ||
                    strncmp(next, "CAM", 3) == 0 ||
                    strncmp(next, "INT", 3) == 0)
                {
                    break;
                }
            }
        }

        // BFS
        else if (strcmp(bloco, "BFS") == 0)
        {
            printf("=== BFS ===\n");
            int x, y;
            char freq;
            while (fread(&x, sizeof(int), 1, f) == 1 &&
                   fread(&y, sizeof(int), 1, f) == 1 &&
                   fread(&freq, sizeof(char), 1, f) == 1)
            {
                printf("Antena BFS: (%d, %d) - %c\n", x, y, freq);

                long pos = ftell(f); // salva a pos atual
                char next[4] = {0};
                fread(next, sizeof(char), 3, f);

                fseek(f, pos, SEEK_SET); // volta posição anterior

                if (strncmp(next, "CAM", 3) == 0 || strncmp(next, "INT", 3) == 0)
                {
                    break;
                }
            }
        }

        //  Caminho
        else if (strcmp(bloco, "CAM") == 0)
        {
            printf("=== CAMINHO ===\n");
            int tamanho;
            fread(&tamanho, sizeof(int), 1, f);
            for (int i = 0; i < tamanho; i++)
            {
                int id;
                fread(&id, sizeof(int), 1, f);
                printf("Antena no caminho: ID %d\n", id);
            }
        }

        //  Interseção
        else if (strcmp(bloco, "INT") == 0)
        {
            printf("=== INTERSEÇÕES ===\n");
            int x1, y1, x2, y2, x3, y3, x4, y4;
            char freq;
            while (fread(&x1, sizeof(int), 1, f) == 1 &&
                   fread(&y1, sizeof(int), 1, f) == 1 &&
                   fread(&x2, sizeof(int), 1, f) == 1 &&
                   fread(&y2, sizeof(int), 1, f) == 1 &&
                   fread(&x3, sizeof(int), 1, f) == 1 &&
                   fread(&y3, sizeof(int), 1, f) == 1 &&
                   fread(&x4, sizeof(int), 1, f) == 1 &&
                   fread(&y4, sizeof(int), 1, f) == 1 &&
                   fread(&freq, sizeof(char), 1, f) == 1)
            {
                printf("Interseção: (%d,%d)-(%d,%d) com (%d,%d)-(%d,%d), freq: %c\n",
                       x1, y1, x2, y2, x3, y3, x4, y4, freq);
            }
        }

        else
        {
            printf("Bloco desconhecido: %s\n", bloco);
            return 0;
            break;
        }
    }

    fclose(f);
    return 1;
}

#pragma endregion

/**
 * @brief   Função para libertar memória
 * @param dfs lista de dfs
 * @param bfs lista de dfs
 * @param ints lista de interseções
 * @return 1 = sucesso
 */

int freeMem(dfslista *dfs, bfslista *bfs, interlista *ints)
{
    while (dfs)
    {
        dfslista *temp = dfs;
        dfs = dfs->prox;
        free(temp);
    }

    while (bfs)
    {
        bfslista *temp = bfs;
        bfs = bfs->prox;
        free(temp);
    }

    while (ints)
    {
        interlista *tmp = ints;
        ints = ints->prox;
        free(tmp);
    }
    return 1;
}
