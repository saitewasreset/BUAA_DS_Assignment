#include "Graph.h"
#include <stdio.h>

void f(struct GraphVertex *v);

int main(void) {
    size_t vertexCount, edgeCount;
    vertexCount = edgeCount = 0;
    scanf("%zu%zu", &vertexCount, &edgeCount);
    struct Graph *graph = Graph_fromPattern(vertexCount, edgeCount);

    Graph_DFS(graph, 0, f);
    putchar('\n');
    Graph_BFS(graph, 0, f);
    putchar('\n');

    size_t toDelete = 0;
    scanf("%zu", &toDelete);

    Graph_deleteVertex(graph, toDelete);
    Graph_DFS(graph, 0, f);
    putchar('\n');
    Graph_BFS(graph, 0, f);
    putchar('\n');

    Graph_destroyGraph(graph);

    return 0;
}

void f(struct GraphVertex *v) { printf("%d ", v->data); }