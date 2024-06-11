#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LIST_LEN 3600
#define MAX_WORD_LEN 25

#define NHASH 3001
#define MULT 37

size_t wordCount = 0;

struct searchInfo {
    size_t index;
    size_t searchCount;
};

struct IndexInfo {
    size_t beginPos;
    size_t wordCount;
};

struct BucketNode {
    char *word;
    struct BucketNode *next;
};

struct HashTable {
    struct BucketNode **buckets;
    size_t bucketCount;
    size_t len;
};

char **readWordList(void);
void freeWordList(char **wordList);
struct IndexInfo *generateWordIndex(char **wordList);

unsigned int hash(char *str);
struct HashTable *createHashTable(size_t bucketCount);
void destroyBucketNode(struct BucketNode *node);
void destroyHashTable(struct HashTable *table);
void insertHashTable(struct HashTable *table, char *data);

struct searchInfo orderSearch(char **wordList, char *word);
struct searchInfo binarySearch(char **wordList, char *word);
struct searchInfo indexSearch(char **wordList, char *word,
                              struct IndexInfo *index);
struct searchInfo hashTableSearch(struct HashTable *table, char *word);

int main(void) {
    char **wordList = readWordList();

    struct IndexInfo *wordIndex = generateWordIndex(wordList);

    struct HashTable *table = createHashTable(NHASH);

    for (size_t i = 0; i < wordCount; i++) {
        insertHashTable(table, wordList[i]);
    }

    char toSearch[MAX_WORD_LEN] = {0};
    int mode = 0;
    scanf("%s%d", toSearch, &mode);

    struct searchInfo result = {0, 0};

    switch (mode) {
    case 1:
        result = orderSearch(wordList, toSearch);
        break;
    case 2:
        result = binarySearch(wordList, toSearch);
        break;
    case 3:
        result = indexSearch(wordList, toSearch, wordIndex);
        break;
    case 4:
        result = hashTableSearch(table, toSearch);
        break;
    }

    printf("%d %zu\n", result.index > 0 ? 1 : 0, result.searchCount);

    freeWordList(wordList);
    free(wordList);
    free(wordIndex);
    destroyHashTable(table);
    return 0;
}

struct searchInfo orderSearch(char **wordList, char *word) {
    struct searchInfo result = {0, 0};
    char **_wordList = wordList;
    while (*wordList != NULL) {
        int cmpResult = strcmp(*wordList, word);
        result.searchCount++;

        if (cmpResult >= 0) {
            if (cmpResult == 0) {
                result.index = (size_t)(wordList - _wordList);
            }
            break;
        }
        wordList++;
    }
    return result;
}

struct searchInfo binarySearch(char **wordList, char *word) {
    struct searchInfo result = {0, 0};
    size_t lo = 0;
    size_t hi = wordCount - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) / 2;
        int cmpResult = strcmp(word, wordList[mid]);
        result.searchCount++;

        if (cmpResult < 0) {
            hi = mid - 1;
        } else if (cmpResult > 0) {
            lo = mid + 1;
        } else {
            result.index = mid;
            break;
        }
    }

    return result;
}

struct searchInfo indexSearch(char **wordList, char *word,
                              struct IndexInfo *index) {
    struct searchInfo result = {0, 0};
    struct IndexInfo idxInfo = index[(unsigned)word[0]];

    if (idxInfo.wordCount == 0) {
        return result;
    }

    size_t lo = idxInfo.beginPos;
    size_t hi = lo + idxInfo.wordCount - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) / 2;
        if (mid >= 3500)
            return result;
        int cmpResult = strcmp(word, wordList[mid]);
        result.searchCount++;

        if (cmpResult < 0) {
            hi = mid - 1;
        } else if (cmpResult > 0) {
            lo = mid + 1;
        } else {
            result.index = 1;
            break;
        }
    }
    return result;
}

struct searchInfo hashTableSearch(struct HashTable *table, char *word) {
    struct searchInfo result = {0, 0};
    unsigned int wordHash = hash(word);

    struct BucketNode *bucket = table->buckets[wordHash];

    if (bucket == NULL) {
        return result;
    } else {
        struct BucketNode *current = bucket;
        while (current != NULL) {
            int cmpResult = strcmp(word, current->word);
            result.searchCount++;
            if (cmpResult > 0) {
                current = current->next;
            } else if (cmpResult == 0) {
                result.index = 1;
                return result;
            } else {
                return result;
            }
        }
    }
    return result;
}

char **readWordList(void) {
    size_t count = 0;
    FILE *f = fopen("dictionary3000.txt", "r");

    char **wordList = (char **)malloc(WORD_LIST_LEN * sizeof(char *));

    memset(wordList, 0, WORD_LIST_LEN * sizeof(char *));

    char buffer[MAX_WORD_LEN] = {0};

    while (fscanf(f, "%s", buffer) > 0) {
        wordCount++;
        wordList[count] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(wordList[count], buffer);
        count++;
    }

    fclose(f);
    return wordList;
}

void freeWordList(char **wordList) {
    while (*wordList != NULL) {
        free(*wordList);
        wordList++;
    }
}

struct IndexInfo *generateWordIndex(char **wordList) {
    struct IndexInfo *result =
        (struct IndexInfo *)malloc(128 * sizeof(struct IndexInfo));

    memset(result, 0, 128 * sizeof(struct IndexInfo));

    char currentBeginCharacter = 'a';
    for (size_t i = 0; i < wordCount; i++) {
        if (wordList[i][0] == currentBeginCharacter) {
            result[(unsigned)currentBeginCharacter].wordCount++;
        } else {
            currentBeginCharacter = wordList[i][0];
            result[(unsigned)currentBeginCharacter].beginPos = i;
            result[(unsigned)currentBeginCharacter].wordCount++;
        }
    }

    return result;
}

unsigned int hash(char *str) {
    unsigned int h = 0;

    char *p;

    for (p = str; *p != '\0'; p++)

        h = MULT * h + (unsigned)*p;

    return h % NHASH;
}

struct HashTable *createHashTable(size_t bucketCount) {
    struct HashTable *table =
        (struct HashTable *)malloc(sizeof(struct HashTable));
    table->bucketCount = bucketCount;
    table->len = 0;
    table->buckets =
        (struct BucketNode **)malloc(bucketCount * sizeof(struct BucketNode *));
    memset(table->buckets, 0, bucketCount * sizeof(struct BucketNode *));

    return table;
}

void destroyBucketNode(struct BucketNode *node) {
    free(node->word);
    free(node);
}

void destroyHashTable(struct HashTable *table) {
    for (size_t i = 0; i < table->bucketCount; i++) {
        struct BucketNode *currentBucket = table->buckets[i];

        if (currentBucket != NULL) {
            while (currentBucket != NULL) {
                struct BucketNode *next = currentBucket->next;
                destroyBucketNode(currentBucket);
                currentBucket = next;
            }
        }
    }
    free(table);
}

void insertHashTable(struct HashTable *table, char *data) {
    unsigned int dataHash = hash(data);

    struct BucketNode *bucket = table->buckets[dataHash];

    struct BucketNode *newNode =
        (struct BucketNode *)malloc(sizeof(struct BucketNode));
    newNode->word = (char *)malloc((strlen(data) + 1) * sizeof(char));
    strcpy(newNode->word, data);
    newNode->next = NULL;

    if (bucket == NULL) {
        table->buckets[dataHash] = newNode;
    } else {
        struct BucketNode *current = bucket;

        int cmpResult = strcmp(data, current->word);

        if (cmpResult < 0) {
            // insert head
            newNode->next = current;
            table->buckets[dataHash] = newNode;
        } else {
            while ((current->next != NULL) &&
                   (strcmp(data, current->next->word) >= 0)) {
                current = current->next;
            }

            newNode->next = current->next;
            current->next = newNode;
        }
    }

    table->len++;
}