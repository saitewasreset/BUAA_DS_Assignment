#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_COUNT 1000

int main(void) {
    size_t vertexCount, edgeCount;
    vertexCount = edgeCount = 0;
    scanf("%zu%zu", &vertexCount, &edgeCount);

    struct Graph *graph = Graph_fromPattern(vertexCount, edgeCount);
    /*
    char dotBuffer[2048] = {0};
    Graph_generateDotGraph(graph, dotBuffer);
    puts(dotBuffer);
    */
    void *pathList =
        malloc(MAX_PATH_COUNT * (graph->vertexCount - 1) * sizeof(int));
    size_t *pathLen = calloc(MAX_PATH_COUNT, sizeof(size_t));

    size_t pathCount =
        Graph_findPath(graph, 0, vertexCount - 1, pathList, pathLen);

    char **pathStrList = Graph_pathStrList(graph, pathList, pathLen, pathCount);

    // qsort(pathStrList, pathCount, sizeof(char *), strCmp);

    for (size_t i = 0; i < pathCount; i++) {
        puts(pathStrList[i]);
    }

    for (size_t i = 0; i < pathCount; i++) {
        free(pathStrList[i]);
    }
    free(pathStrList);
    Graph_destroyGraph(graph);
    free(pathList);
    free(pathLen);

    return 0;
}
