#ifndef __GRAPH_H
#define __GRAPH_H

#define DOT_NODE_FORMAT_BUFFER_LEN 255

#include <stdbool.h>
#include <stddef.h>

#define MAX_PATH_STR_LEN 2000

typedef int VertexData;

struct GraphEdgeNode {
    int weight;
    size_t otherVertex;
    struct GraphEdgeNode *next;
};

struct GraphVertex {
    VertexData data;
    struct GraphEdgeNode *edgeList;
    size_t edgeCount;
    bool valid;
};

struct Graph {
    struct GraphVertex *vertexList;
    size_t vertexCount;
    size_t vertexCapacity;
    int maxEdgeId;
};

// First, list of Vertex Data
// Then, list of (edgeData vertexA vertexB)
struct Graph *Graph_fromPattern(size_t vertexCount, size_t edgeCount);
size_t Graph_insertVertex(struct Graph *graph, VertexData data);
void Graph_insertEdge(struct Graph *graph, size_t vertexA, size_t vertexB,
                      int weight);
void Graph_deleteVertex(struct Graph *graph, size_t vertexId);
void Graph_destroyGraph(struct Graph *graph);
size_t Graph_findPath(struct Graph *graph, size_t beginVertexId,
                      size_t endVertexId, void *pathList, size_t *pathLen);
char **Graph_pathStrList(struct Graph *graph, void *pathList, size_t *pathLen,
                         size_t pathCount);
void Graph_generateDotGraph(struct Graph *graph, char *outputBuffer);

#endif
