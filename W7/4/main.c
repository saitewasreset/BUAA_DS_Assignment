#include "Graph.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// No hiding from me, *BUG*

#define STATION_FILENAME "bgstations.txt"
#define NAME_BUFFER_LEN 255
#define MAX_PATH_LEN 512
#define MAX_INTERCHANGE_COUNT 255
#define MAX_STATION_COUNT 1024

void printPath(struct Graph *graph, size_t sourceStationId,
               size_t targetStationId);
struct StationInfo **readMap(struct Graph *graph);
int stationCmp(const void *a, const void *b);

int main(void) {
    struct Graph *graph = Graph_create(MAX_STATION_COUNT);

    struct StationInfo **stationList = readMap(graph);

    char sourceStation[NAME_BUFFER_LEN] = {0};
    char targetStation[NAME_BUFFER_LEN] = {0};

    scanf("%s", sourceStation);
    scanf("%s", targetStation);

    size_t sourceStationId = 0;
    size_t targetStationId = 0;

    for (size_t i = 0; i < graph->vertexCount; i++) {
        struct GraphVertex *currentStationVertex = &(graph->vertexList[i]);
        if (strcmp(currentStationVertex->data->stationName, sourceStation) ==
            0) {
            sourceStationId = i;
        } else if (strcmp(currentStationVertex->data->stationName,
                          targetStation) == 0) {
            targetStationId = i;
        }
    }

    printPath(graph, sourceStationId, targetStationId);

    for (size_t i = 0; i < graph->vertexCount; i++) {
        struct StationInfo *currentInfo = stationList[i];
        free(currentInfo->stationName);
        free(currentInfo);
    }
    free(stationList);
    Graph_destroyGraph(graph);
    return 0;
}

struct StationInfo **readMap(struct Graph *graph) {
    FILE *f = fopen(STATION_FILENAME, "r");

    struct StationInfo **stationList = (struct StationInfo **)malloc(
        MAX_STATION_COUNT * sizeof(struct StationInfo *));
    size_t stationListLen = 0;

    size_t lineCount = 0;
    fscanf(f, "%zu", &lineCount);

    size_t firstStationVertexId = SIZE_MAX;
    char *firstStationName = NULL;
    size_t prevStationVertexId = SIZE_MAX;
    size_t currentStationVertexId = SIZE_MAX;

    for (size_t i = 0; i < lineCount; i++) {
        size_t currentLineId = 0;
        size_t currentLineStationCount = 0;
        fscanf(f, "%zu%zu", &currentLineId, &currentLineStationCount);

        firstStationVertexId = SIZE_MAX;
        firstStationName = NULL;
        prevStationVertexId = SIZE_MAX;
        currentStationVertexId = SIZE_MAX;

        for (size_t j = 0; j < currentLineStationCount; j++) {
            char stationNameBuffer[NAME_BUFFER_LEN] = {0};
            int interchange = 0;

            fscanf(f, "%s%d", stationNameBuffer, &interchange);

            bool alreadyInserted = false;
            struct StationInfo *currentStationInfo = NULL;
            if (interchange) {
                for (size_t k = 0; k < stationListLen; k++) {
                    if (strcmp(stationList[k]->stationName,
                               stationNameBuffer) == 0) {
                        alreadyInserted = true;
                        currentStationVertexId = stationList[k]->vertexId;
                        currentStationInfo = stationList[k];
                        break;
                    }
                }
            }

            if (!alreadyInserted) {
                currentStationInfo =
                    (struct StationInfo *)malloc(sizeof(struct StationInfo));
                char *stationName = (char *)malloc(
                    (strlen(stationNameBuffer) + 1) * sizeof(char));

                if (j == 0) {
                    firstStationName = stationName;
                }

                strcpy(stationName, stationNameBuffer);
                currentStationInfo->stationName = stationName;
                currentStationInfo->interchange = (bool)interchange;
                currentStationInfo->lineId[0] = currentLineId;
                currentStationInfo->lineCount = 1;

                currentStationVertexId =
                    Graph_insertVertex(graph, currentStationInfo);

                currentStationInfo->vertexId = currentStationVertexId;
                stationList[stationListLen] = currentStationInfo;
                stationListLen++;
            } else {
                currentStationInfo->lineId[currentStationInfo->lineCount] =
                    currentLineId;
                currentStationInfo->lineCount++;
                if (j == 0) {
                    firstStationName = graph->vertexList[currentStationVertexId]
                                           .data->stationName;
                }
            }

            if (j == 0) {
                firstStationVertexId = currentStationVertexId;
            }

            if (j != 0 && strcmp(stationNameBuffer, firstStationName) == 0) {
                // loop
                Graph_insertEdge(graph, prevStationVertexId,
                                 firstStationVertexId, 1);
            } else if (prevStationVertexId != SIZE_MAX) {
                Graph_insertEdge(graph, prevStationVertexId,
                                 currentStationVertexId, 1);
            }

            prevStationVertexId = currentStationVertexId;
        }
    }

    qsort(stationList, stationListLen, sizeof(struct StationInfo *),
          stationCmp);
    fclose(f);
    return stationList;
}

int stationCmp(const void *a, const void *b) {
    const struct StationInfo *sA = *((const struct StationInfo **)a);
    const struct StationInfo *sB = *((const struct StationInfo **)b);

    return strcmp(sA->stationName, sB->stationName);
}

void printPath(struct Graph *graph, size_t sourceStationId,
               size_t targetStationId) {
    size_t *minPathViaVertex = Graph_Dijkstra(graph, sourceStationId);

    size_t currentStation = targetStationId;

    size_t stationList[MAX_PATH_LEN] = {0};
    size_t stationListLen = 0;

    while (currentStation != sourceStationId) {
        if (stationListLen != 0) {
            if (strcmp(graph->vertexList[currentStation].data->stationName,
                       graph->vertexList[stationList[stationListLen - 1]]
                           .data->stationName) == 0) {
                currentStation = minPathViaVertex[currentStation];
                continue;
            }
        }
        stationList[stationListLen] = currentStation;
        stationListLen++;
        currentStation = minPathViaVertex[currentStation];
    }

    stationList[stationListLen] = sourceStationId;
    stationListLen++;

    for (size_t i = 0; i < stationListLen / 2; i++) {
        size_t temp = stationList[i];
        stationList[i] = stationList[stationListLen - i - 1];
        stationList[stationListLen - i - 1] = temp;
    }
    /*
    for (size_t i = 0; i < stationListLen; i++) {
        printf("%s ", graph->vertexList[stationList[i]].data->stationName);
    }
    putchar('\n');
    */

    // FIX: what if begin station is interchange?

    size_t currentLine = graph->vertexList[stationList[0]].data->lineId[0];
    size_t currentLineLen = 0;
    struct StationInfo *currentStationInfo =
        graph->vertexList[stationList[0]].data;
    if (graph->vertexList[stationList[0]].data->interchange) {
        currentLine = SIZE_MAX;
    } else {
        printf("%s", currentStationInfo->stationName);
    }

    for (size_t i = 1; i < stationListLen; i++) {
        currentStationInfo = graph->vertexList[stationList[i]].data;

        if (!currentStationInfo->interchange) {
            if (currentStationInfo->lineId[0] != currentLine) {
                if (i <= 1 ||
                    !graph->vertexList[stationList[i - 2]].data->interchange) {
                    if (currentLine != SIZE_MAX) {
                        printf("-%zu(%zu)-", currentLine, currentLineLen);
                    }
                    printf("%s", graph->vertexList[stationList[i - 1]]
                                     .data->stationName);
                    currentLine = currentStationInfo->lineId[0];
                    currentLineLen = 0;
                } else {
                    size_t newLine = currentStationInfo->lineId[0];
                    size_t j = i - 1;
                    bool found = false;
                    while (1) {
                        struct StationInfo *s =
                            graph->vertexList[stationList[j]].data;

                        bool hasOld = false;
                        bool hasNew = false;
                        size_t k;
                        for (k = 0; k < s->lineCount; k++) {
                            if (s->lineId[k] == newLine) {
                                hasNew = true;
                            } else if (s->lineId[k] == currentLine) {
                                hasOld = true;
                            }
                        }

                        if (hasOld && hasNew) {
                            printf("-%zu(%zu)-", currentLine,
                                   currentLineLen - (i - j - 1));
                            printf("%s", graph->vertexList[stationList[j]]
                                             .data->stationName);
                            currentLine = newLine;
                            currentLineLen = i - j - 1;
                            found = true;
                            break;
                        }

                        if (j == 0) {
                            break;
                        }
                        j--;
                    }

                    if (!found) {

                        size_t interchangeLineId = 0;
                        struct StationInfo *first =
                            graph->vertexList[stationList[i - 2]].data;
                        struct StationInfo *second =
                            graph->vertexList[stationList[i - 1]].data;

                        for (size_t m = 0; m < first->lineCount; m++) {
                            for (size_t n = 0; n < second->lineCount; n++) {
                                if (first->lineId[m] == second->lineId[n]) {
                                    interchangeLineId = first->lineId[m];
                                }
                            }
                        }

                        printf("-%zu(%zu)-", currentLine, currentLineLen - 1);
                        // printf("%s")
                        printf("%s-%zu(1)-%s", first->stationName,
                               interchangeLineId, second->stationName);
                        currentLine = newLine;
                        currentLineLen = 0;
                    }
                }
            }
        }

        currentLineLen++;
    }
    printf("-%zu(%zu)-", currentLine, currentLineLen);
    printf("%s", currentStationInfo->stationName);
    putchar('\n');
    free(minPathViaVertex);
}