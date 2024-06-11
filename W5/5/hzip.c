#include "libhzip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char *inputFilename = NULL;

    if ((argc < 2) || (argc > 3)) {
        printf("Usage: %s [-u] <filename>\n", argv[0]);
        exit(1);
    }

    if (argc == 2) {
        inputFilename = argv[1];
    }

    if (argc == 3) {
        if (strcmp(argv[1], "-u") != 0) {
            printf("Usage: %s [-u] <filename>\n", argv[0]);
            exit(1);
        }
        inputFilename = argv[2];
    }

    char *separatorPtr = strchr(inputFilename, '.');
    if (separatorPtr == NULL) {
        printf("File extension error!\n");
        exit(1);
    }

    size_t inputFileStemLen = (size_t)(separatorPtr - inputFilename);
    size_t inputFileExtensionLen = strlen(inputFilename) - inputFileStemLen - 1;

    char *inputFileStem = (char *)malloc((inputFileStemLen + 1) * sizeof(char));
    char *inputFileExtension =
        (char *)malloc((inputFileExtensionLen + 1) * sizeof(char));

    memcpy(inputFileStem, inputFilename, inputFileStemLen * sizeof(char));
    inputFileStem[inputFileStemLen] = '\0';

    memcpy(inputFileExtension, inputFilename + inputFileStemLen + 1,
           inputFileExtensionLen);
    inputFileExtension[inputFileExtensionLen] = '\0';

    FILE *src = NULL;
    FILE *dst = NULL;
    char *outputFilename = NULL;

    if (argc == 2) {

        if (strcmp(inputFileExtension, "txt") != 0) {
            printf("File extension error!\n");
            exit(1);
        }

        outputFilename =
            (char *)malloc((inputFileStemLen + 5 + 1) * sizeof(char));
        strcpy(outputFilename, inputFileStem);
        strcat(outputFilename, ".hzip");

        src = fopen(inputFilename, "r");

        if (src == NULL) {
            printf("Error opening: %s for reading.\n", inputFilename);
            exit(1);
        }

        dst = fopen(outputFilename, "w");

        if (dst == NULL) {
            printf("Error opening %s for writing.\n", outputFilename);
            exit(1);
        }

        compress(src, dst);
    } else {
        if (strcmp(inputFileExtension, "hzip") != 0) {
            printf("File extension error!\n");
            exit(1);
        }

        outputFilename =
            (char *)malloc((inputFileStemLen + 4 + 1) * sizeof(char));
        strcpy(outputFilename, inputFileStem);
        strcat(outputFilename, ".txt");

        src = fopen(inputFilename, "r");

        if (src == NULL) {
            printf("Error opening: %s for reading.\n", inputFilename);
            exit(1);
        }

        dst = fopen(outputFilename, "w");

        if (dst == NULL) {
            printf("Error opening %s for writing.\n", outputFilename);
            exit(1);
        }
        decompress(src, dst);
    }

    fclose(src);
    fclose(dst);
    free(inputFileStem);
    free(inputFileExtension);
    free(outputFilename);

    return 0;
}