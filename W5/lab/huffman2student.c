// 文件压缩-Huffman实现
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 32

struct tnode { // Huffman树结构
    char c;
    int weight; // 树节点权重，叶节点为字符和它的出现次数
    struct tnode *left, *right;
};
int Ccount[128] = {
    0}; // 存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数
struct tnode *Root = NULL; // Huffman树的根节点
char HCode[128][MAXSIZE] = {
    {0}}; // 字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式）
int Step = 0; // 实验步骤
FILE *Src, *Obj;

void statCount();   // 步骤1：统计文件中字符频率
void createHTree(); // 步骤2：创建一个Huffman树，根节点为Root
void makeHCode();   // 步骤3：根据Huffman树生成Huffman编码
void atoHZIP(); // 步骤4：根据Huffman编码将指定ASCII码文本文件转换成Huffman码文件

void print1();                // 输出步骤1的结果
void print2(struct tnode *p); // 输出步骤2的结果
void print3();                // 输出步骤3的结果
void print4();                // 输出步骤4的结果

int main() {
    if ((Src = fopen("input.txt", "r")) == NULL) {
        fprintf(stderr, "%s open failed!\n", "input.txt");
        return 1;
    }
    if ((Obj = fopen("output.txt", "w")) == NULL) {
        fprintf(stderr, "%s open failed!\n", "output.txt");
        return 1;
    }
    scanf("%d", &Step); // 输入当前实验步骤

    statCount(); // 实验步骤1：统计文件中字符出现次数（频率）
    if (Step == 1)
        print1();  // 输出实验步骤1结果
    createHTree(); // 实验步骤2：依据字符频率生成相应的Huffman树
    if (Step == 2)
        print2(Root); // 输出实验步骤2结果
    makeHCode(); // 实验步骤3：依据Root为树的根的Huffman树生成相应Huffman编码
    if (Step == 3)
        print3(); // 输出实验步骤3结果
    if (Step >= 4)
        atoHZIP(),
            print4(); // 实验步骤4：据Huffman编码生成压缩文件，并输出实验步骤4结果

    fclose(Src);
    fclose(Obj);

    return 0;
}

// 【实验步骤1】开始

void statCount() {
    Ccount['\0'] = 1;
    int ch = 0;
    while ((ch = fgetc(Src)) != EOF) {
        if (ch != '\n') {
            Ccount[ch]++;
        }
    }
}

// 【实验步骤1】结束

// 【实验步骤2】开始

struct LinkedListNode {
    struct tnode *treeNode;
    struct LinkedListNode *next;
};
struct LinkedListNode *insertOrdered(struct LinkedListNode *head,
                                     struct tnode *data);
struct LinkedListNode *insertOrdered(struct LinkedListNode *head,
                                     struct tnode *data) {
    struct LinkedListNode *newNode =
        (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
    newNode->treeNode = data;

    struct LinkedListNode *current = head;

    if (current->treeNode->weight > data->weight) {
        newNode->next = head;
        return newNode;
    }

    if ((current->treeNode->weight == data->weight) &&
        (current->treeNode->c > data->c) &&
        ((data->left == NULL) && (data->right == NULL))) {
        newNode->next = head;
        return newNode;
    }

    while ((current->next != NULL) &&
           (current->next->treeNode->weight < data->weight)) {
        current = current->next;
    }

    if (current->next == NULL) {
        current->next = newNode;
        newNode->next = NULL;
        return head;
    } else if (current->next->treeNode->weight > data->weight) {
        newNode->next = current->next;
        current->next = newNode;
        return head;
    } else {
        if ((data->left == NULL) && (data->right == NULL)) {
            while ((current->next != NULL) &&
                   (current->next->treeNode->c < data->c) &&
                   (current->next->treeNode->weight == data->weight)) {
                current = current->next;
            }

            if (current->next == NULL) {
                current->next = newNode;
                newNode->next = NULL;
                return head;
            } else {
                newNode->next = current->next;
                current->next = newNode;
                return head;
            }
        } else {
            while ((current->next != NULL) &&
                   (current->next->treeNode->weight == data->weight)) {
                current = current->next;
            }

            if (current->next == NULL) {
                current->next = newNode;
                newNode->next = NULL;
                return head;
            } else {
                newNode->next = current->next;
                current->next = newNode;
                return head;
            }
        }
    }
}

void createHTree() {
    struct tnode *first = (struct tnode *)malloc(sizeof(struct tnode));
    first->c = '\0';
    first->weight = 1;
    first->left = NULL;
    first->right = NULL;

    struct LinkedListNode *head =
        (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
    head->treeNode = first;
    head->next = NULL;

    for (int i = 1; i < 128; i++) {
        if (Ccount[i] > 0) {
            struct tnode *current =
                (struct tnode *)malloc(sizeof(struct tnode));
            current->c = (char)i;
            current->weight = Ccount[i];
            current->left = NULL;
            current->right = NULL;
            head = insertOrdered(head, current);
        }
    }

    char parentId = 0;

    while (head->next->next != NULL) {
        struct tnode *parent = (struct tnode *)malloc(sizeof(struct tnode));
        parent->left = head->treeNode;
        parent->right = head->next->treeNode;
        parent->c = parentId;
        parent->weight = parent->left->weight + parent->right->weight;

        struct LinkedListNode *newHead = head->next->next;

        free(head->next);
        free(head);

        head = newHead;

        head = insertOrdered(head, parent);
        parentId++;
    }

    struct tnode *final = (struct tnode *)malloc(sizeof(struct tnode));
    final->left = head->treeNode;
    final->right = head->next->treeNode;
    final->c = parentId;
    final->weight = final->left->weight + final->right->weight;

    free(head->next);
    free(head);

    Root = final;
}

// 【实验步骤2】结束

// 【实验步骤3】开始

char currentCode[MAXSIZE] = {0};

void preOrder(struct tnode *node, int currentCodeLen);

void makeHCode() { preOrder(Root, 0); }

void preOrder(struct tnode *node, int currentCodeLen) {
    if (node == NULL) {
        return;
    }

    if ((node->left == NULL) && (node->right == NULL)) {
        currentCode[currentCodeLen] = '\0';
        strcpy(HCode[(unsigned)(node->c)], currentCode);
        return;
    }

    currentCode[currentCodeLen] = '0';
    preOrder(node->left, currentCodeLen + 1);

    currentCode[currentCodeLen] = '1';
    preOrder(node->right, currentCodeLen + 1);
}

// 【实验步骤3】结束

// 【实验步骤4】开始

void atoHZIP() {
    fseek(Src, 0, SEEK_SET);
    fseek(Obj, 0, SEEK_SET);

    int ch = 0;
    unsigned mapped = 0;
    int mappedLen = 0;
    while ((ch = fgetc(Src)) != EOF) {
        if (ch != '\n') {
            char *code = HCode[ch];

            while (*code != '\0') {
                mapped = (mapped << 1) | (unsigned)(*code - '0');
                mappedLen++;

                if (mappedLen == 8) {
                    printf("%x", mapped);
                    fputc((int)mapped, Obj);

                    mapped = 0;
                    mappedLen = 0;
                }

                code++;
            }
        }
    }

    char *code = HCode[0];

    while (*code != '\0') {
        mapped = (mapped << 1) | (unsigned)(*code - '0');
        mappedLen++;

        if (mappedLen == 8) {
            printf("%x", mapped);
            fputc((int)mapped, Obj);

            mapped = 0;
            mappedLen = 0;
        }

        code++;
    }

    if (mappedLen > 0) {
        mapped = mapped << (unsigned)(8 - mappedLen);
        printf("%x", mapped);
        fputc((int)mapped, Obj);
    }
}

// 【实验步骤4】结束

void print1() {
    int i;
    printf("NUL:1\n");
    for (i = 1; i < 128; i++)
        if (Ccount[i] > 0)
            printf("%c:%d\n", i, Ccount[i]);
}

void print2(struct tnode *p) {
    if (p != NULL) {
        if ((p->left == NULL) && (p->right == NULL))
            switch (p->c) {
            case 0:
                printf("NUL ");
                break;
            case ' ':
                printf("SP ");
                break;
            case '\t':
                printf("TAB ");
                break;
            case '\n':
                printf("CR ");
                break;
            default:
                printf("%c ", p->c);
                break;
            }
        print2(p->left);
        print2(p->right);
    }
}

void print3() {
    int i;

    for (i = 0; i < 128; i++) {
        if (HCode[i][0] != 0) {
            switch (i) {
            case 0:
                printf("NUL:");
                break;
            case ' ':
                printf("SP:");
                break;
            case '\t':
                printf("TAB:");
                break;
            case '\n':
                printf("CR:");
                break;
            default:
                printf("%c:", i);
                break;
            }
            printf("%s\n", HCode[i]);
        }
    }
}

void print4() {
    long int in_size, out_size;

    fseek(Src, 0, SEEK_END);
    fseek(Obj, 0, SEEK_END);
    in_size = ftell(Src);
    out_size = ftell(Obj);

    printf("\n原文件大小：%ldB\n", in_size);
    printf("压缩后文件大小：%ldB\n", out_size);
    printf("压缩率：%.2f%%\n", (float)(in_size - out_size) * 100 / in_size);
}
