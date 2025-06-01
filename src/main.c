#include <stdio.h>
#include <stdlib.h>
#include "../include/estruturas.h"
#include "../include/func.h"

int main()
{
    int res;
    char *ficheiro_txt = "antenas.txt";

    FILE *output = fopen("output.bin", "wb");

    Grafo *grafo = carregarAntenas(ficheiro_txt);
    res = escreverAntenas(grafo, output);

    res = inserirArestas(grafo);

    //  dfs
    dfslista *dfs = execDfs(grafo, 1);
    res = escreverDFS(dfs, output);

    //  bfs
    bfslista *bfs = execBfs(grafo, 0);
    res = escreverBFS(bfs, output);

    //  caminho
    Caminho cam;
    if (encontrarCaminhoDFS(grafo, 0, 3, &cam))
    {
        res = escreverCaminho(&cam, output);
    }

    // interseção
    interlista *ints = detetarIntersecoes(grafo);
    res = escreverIntersecoes(ints, output);

    fclose(output);

    res = lerFicheiroEscreverOutput("output.bin");
    res = freeMem(dfs, bfs, ints);

    return 0;
}
