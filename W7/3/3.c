#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

int edgeIdCmp(const void *a, const void *b);

int main(void) {
    size_t vertexCount, edgeCount;
    vertexCount = edgeCount = 0;
    scanf("%zu%zu", &vertexCount, &edgeCount);
    struct Graph *graph = Graph_fromPattern(vertexCount, edgeCount);

    /*char dotBuffer[2048] = {0};
    Graph_generateDotGraph(graph, dotBuffer);
    puts(dotBuffer);
    */
    size_t minWeight = 0;
    size_t *edgeIdList = (size_t *)malloc((vertexCount - 1) * sizeof(size_t));
    struct Graph *mst = Graph_MSTPrim(graph, 0, &minWeight, edgeIdList);
    /*
    Graph_generateDotGraph(mst, dotBuffer);
    puts(dotBuffer);
    */
    qsort(edgeIdList, vertexCount - 1, sizeof(size_t), edgeIdCmp);

    printf("%zu\n", minWeight);

    for (size_t i = 0; i < vertexCount - 1; i++) {
        printf("%zu ", edgeIdList[i]);
    }
    putchar('\n');
    Graph_destroyGraph(graph);
    Graph_destroyGraph(mst);
    free(edgeIdList);
    return 0;
}

int edgeIdCmp(const void *a, const void *b) {
    size_t eA = *((const size_t *)a);
    size_t eB = *((const size_t *)b);
    if (eA < eB) {
        return -1;
    } else if (eA > eB) {
        return 1;
    } else {
        return 0;
    }
}