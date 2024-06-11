#include "Graph.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void Graph_eDFS(struct Graph *graph, size_t vertexId, size_t endVertexId,
                       int *path, void *pathList, size_t *pathLen,
                       size_t *pathCount, bool *visited, bool *nodeVisited,
                       size_t level);
static int edgeCmp(const void *a, const void *b);
struct Graph *Graph_fromPattern(size_t vertexCount, size_t edgeCount) {
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->vertexCount = vertexCount;
    graph->vertexCapacity = vertexCount;

    struct GraphVertex *vertexList =
        (struct GraphVertex *)malloc(vertexCount * sizeof(struct GraphVertex));
    graph->vertexList = vertexList;

    for (size_t i = 0; i < vertexCount; i++) {
        graph->vertexList[i].data = i;
        graph->vertexList[i].edgeCount = 0;
        graph->vertexList[i].edgeList = NULL;
        graph->vertexList[i].valid = true;
    }

    int maxEdgeId = 0;

    for (size_t i = 0; i < edgeCount; i++) {
        int d;
        scanf("%d", &d);

        if (d > maxEdgeId) {
            maxEdgeId = d;
        }

        size_t vertexA, vertexB;
        vertexA = vertexB = 0;
        scanf("%zu%zu", &vertexA, &vertexB);

        Graph_insertEdge(graph, vertexA, vertexB, d);
    }

    graph->maxEdgeId = maxEdgeId;
    return graph;
}

size_t Graph_insertVertex(struct Graph *graph, VertexData data) {
    size_t vertexId = graph->vertexCount;

    graph->vertexList[vertexId].data = data;
    graph->vertexList[vertexId].edgeList = NULL;
    graph->vertexList[vertexId].edgeCount = 0;
    graph->vertexList[vertexId].valid = true;

    graph->vertexCount++;
    return vertexId;
}

void Graph_insertEdge(struct Graph *graph, size_t vertexA, size_t vertexB,
                      int weight) {
    struct GraphEdgeNode *node =
        (struct GraphEdgeNode *)malloc(sizeof(struct GraphEdgeNode));

    node->weight = weight;
    node->otherVertex = vertexB;

    if (graph->vertexList[vertexA].edgeCount == 0) {
        graph->vertexList[vertexA].edgeList = node;
        node->next = NULL;
    } else {
        node->next = graph->vertexList[vertexA].edgeList;
        graph->vertexList[vertexA].edgeList = node;
    }

    graph->vertexList[vertexA].edgeCount++;

    struct GraphEdgeNode *otherNode =
        (struct GraphEdgeNode *)malloc(sizeof(struct GraphEdgeNode));

    otherNode->weight = weight;
    otherNode->otherVertex = vertexA;

    if (graph->vertexList[vertexB].edgeCount == 0) {

        graph->vertexList[vertexB].edgeList = otherNode;
        otherNode->next = NULL;
    } else {
        otherNode->next = graph->vertexList[vertexB].edgeList;
        graph->vertexList[vertexB].edgeList = otherNode;
    }

    graph->vertexList[vertexB].edgeCount++;
}

void Graph_deleteVertex(struct Graph *graph, size_t vertexId) {
    struct GraphEdgeNode *currentEdge = graph->vertexList[vertexId].edgeList;
    while (currentEdge != NULL) {
        size_t otherVertexId = currentEdge->otherVertex;
        struct GraphEdgeNode *otherPrev = NULL;
        struct GraphEdgeNode *otherCurrent =
            graph->vertexList[otherVertexId].edgeList;
        while (otherCurrent != NULL) {
            if (otherCurrent->otherVertex == vertexId) {
                if (otherPrev == NULL) {
                    graph->vertexList[otherVertexId].edgeList =
                        otherCurrent->next;
                } else {
                    otherPrev->next = otherCurrent->next;
                }
                free(otherCurrent);
                graph->vertexList[otherVertexId].edgeCount--;
                break;
            } else {
                otherPrev = otherCurrent;
                otherCurrent = otherCurrent->next;
            }
        }

        struct GraphEdgeNode *next = currentEdge->next;
        free(currentEdge);
        currentEdge = next;
    }
    graph->vertexList[vertexId].edgeCount = 0;
    graph->vertexList[vertexId].edgeList = 0;
    graph->vertexList[vertexId].valid = false;
    graph->vertexCount--;
}

void Graph_destroyGraph(struct Graph *graph) {
    for (size_t i = 0; i < graph->vertexCapacity; i++) {
        struct GraphEdgeNode *current = graph->vertexList[i].edgeList;

        while (current != NULL) {
            struct GraphEdgeNode *next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph->vertexList);
    free(graph);
}

void Graph_generateDotGraph(struct Graph *graph, char *outputBuffer) {
    size_t N = (size_t)graph->maxEdgeId;
    bool *visited = (bool *)calloc(N + 1, sizeof(bool));

    const char *dotBegin = "graph G {\nnode [shape = circle;];\n";
    strcpy(outputBuffer, dotBegin);

    char nodeFormatBuffer[DOT_NODE_FORMAT_BUFFER_LEN] = {0};
    for (size_t i = 0; i < graph->vertexCapacity; i++) {
        if (graph->vertexList[i].valid) {
            sprintf(nodeFormatBuffer, "%zu [label = \"%d\";];\n", i,
                    graph->vertexList[i].data);
            strcat(outputBuffer, nodeFormatBuffer);
        }
    }

    for (size_t i = 0; i < graph->vertexCapacity; i++) {
        struct GraphEdgeNode *current = graph->vertexList[i].edgeList;

        while (current != NULL) {
            if (!visited[current->weight]) {
                sprintf(nodeFormatBuffer, "%zu -- %zu [label = \"%d\";];\n", i,
                        current->otherVertex, current->weight);
                strcat(outputBuffer, nodeFormatBuffer);
                visited[current->weight] = true;
            }

            current = current->next;
        }
    }

    strcat(outputBuffer, "}");
    free(visited);
}

size_t Graph_findPath(struct Graph *graph, size_t beginVertexId,
                      size_t endVertexId, void *pathList, size_t *pathLen) {
    size_t pathCount = 0;
    bool *visited =
        (bool *)calloc((size_t)(graph->maxEdgeId + 1), sizeof(bool));
    bool *nodeVisited =
        (bool *)calloc((size_t)(graph->vertexCapacity), sizeof(bool));
    int *path = (int *)malloc((graph->vertexCount - 1) * sizeof(int));

    nodeVisited[beginVertexId] = true;

    Graph_eDFS(graph, beginVertexId, endVertexId, path, pathList, pathLen,
               &pathCount, visited, nodeVisited, 0);

    free(visited);
    free(nodeVisited);
    free(path);
    return pathCount;
}

void Graph_eDFS(struct Graph *graph, size_t vertexId, size_t endVertexId,
                int *path, void *pathList, size_t *pathLen, size_t *pathCount,
                bool *visited, bool *nodeVisited, size_t level) {
    int(*pList)[graph->vertexCount - 1] =
        (int(*)[graph->vertexCount - 1]) pathList;
    if (vertexId == endVertexId) {

        memcpy(pList[*pathCount], path, level * sizeof(int));

        pathLen[*pathCount] = level;
        (*pathCount)++;
        return;
    }

    struct GraphEdgeNode **nodeArray = (struct GraphEdgeNode **)malloc(
        graph->vertexList[vertexId].edgeCount * sizeof(struct GraphEdgeNode *));
    size_t nodeArrayLen = 0;
    struct GraphEdgeNode *current = graph->vertexList[vertexId].edgeList;
    while (current != NULL) {
        nodeArray[nodeArrayLen] = current;
        nodeArrayLen++;
        current = current->next;
    }
    qsort(nodeArray, nodeArrayLen, sizeof(struct GraphEdgeNode *), edgeCmp);
    for (size_t i = 0; i < nodeArrayLen; i++) {
        current = nodeArray[i];
        if (!nodeVisited[current->otherVertex]) {
            nodeVisited[current->otherVertex] = true;
            if (!visited[current->weight]) {
                visited[current->weight] = true;

                path[level] = current->weight;
                pathLen[*pathCount] = level;

                Graph_eDFS(graph, current->otherVertex, endVertexId, path,
                           pathList, pathLen, pathCount, visited, nodeVisited,
                           level + 1);

                visited[current->weight] = false;
            }
            nodeVisited[current->otherVertex] = false;
        }
    }

    free(nodeArray);
}

char **Graph_pathStrList(struct Graph *graph, void *pathList, size_t *pathLen,
                         size_t pathCount) {
    char **result = (char **)malloc(pathCount * sizeof(char *));

    int(*pList)[graph->vertexCount - 1] =
        (int(*)[graph->vertexCount - 1]) pathList;
    for (size_t i = 0; i < pathCount; i++) {
        char *currentPath = (char *)calloc(MAX_PATH_STR_LEN, sizeof(char));
        char *currentChar = currentPath;
        for (size_t j = 0; j < pathLen[i]; j++) {
            int delta = sprintf(currentChar, "%d ", pList[i][j]);
            currentChar += delta;
        }

        result[i] = currentPath;
    }

    return result;
}

int edgeCmp(const void *a, const void *b) {
    const struct GraphEdgeNode *nA = *((const struct GraphEdgeNode **)a);
    const struct GraphEdgeNode *nB = *((const struct GraphEdgeNode **)b);

    if (nA->weight < nB->weight) {
        return -1;
    } else if (nA->weight > nB->weight) {
        return 1;
    } else {
        return 0;
    }
}