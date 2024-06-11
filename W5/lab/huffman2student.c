// �ļ�ѹ��-Huffmanʵ��
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 32

struct tnode { // Huffman���ṹ
    char c;
    int weight; // ���ڵ�Ȩ�أ�Ҷ�ڵ�Ϊ�ַ������ĳ��ִ���
    struct tnode *left, *right;
};
int Ccount[128] = {
    0}; // ���ÿ���ַ��ĳ��ִ�������Ccount[i]��ʾASCIIֵΪi���ַ����ִ���
struct tnode *Root = NULL; // Huffman���ĸ��ڵ�
char HCode[128][MAXSIZE] = {
    {0}}; // �ַ���Huffman���룬��HCode['a']Ϊ�ַ�a��Huffman���루�ַ�����ʽ��
int Step = 0; // ʵ�鲽��
FILE *Src, *Obj;

void statCount();   // ����1��ͳ���ļ����ַ�Ƶ��
void createHTree(); // ����2������һ��Huffman�������ڵ�ΪRoot
void makeHCode();   // ����3������Huffman������Huffman����
void atoHZIP(); // ����4������Huffman���뽫ָ��ASCII���ı��ļ�ת����Huffman���ļ�

void print1();                // �������1�Ľ��
void print2(struct tnode *p); // �������2�Ľ��
void print3();                // �������3�Ľ��
void print4();                // �������4�Ľ��

int main() {
    if ((Src = fopen("input.txt", "r")) == NULL) {
        fprintf(stderr, "%s open failed!\n", "input.txt");
        return 1;
    }
    if ((Obj = fopen("output.txt", "w")) == NULL) {
        fprintf(stderr, "%s open failed!\n", "output.txt");
        return 1;
    }
    scanf("%d", &Step); // ���뵱ǰʵ�鲽��

    statCount(); // ʵ�鲽��1��ͳ���ļ����ַ����ִ�����Ƶ�ʣ�
    if (Step == 1)
        print1();  // ���ʵ�鲽��1���
    createHTree(); // ʵ�鲽��2�������ַ�Ƶ��������Ӧ��Huffman��
    if (Step == 2)
        print2(Root); // ���ʵ�鲽��2���
    makeHCode(); // ʵ�鲽��3������RootΪ���ĸ���Huffman��������ӦHuffman����
    if (Step == 3)
        print3(); // ���ʵ�鲽��3���
    if (Step >= 4)
        atoHZIP(),
            print4(); // ʵ�鲽��4����Huffman��������ѹ���ļ��������ʵ�鲽��4���

    fclose(Src);
    fclose(Obj);

    return 0;
}

// ��ʵ�鲽��1����ʼ

void statCount() {
    Ccount['\0'] = 1;
    int ch = 0;
    while ((ch = fgetc(Src)) != EOF) {
        if (ch != '\n') {
            Ccount[ch]++;
        }
    }
}

// ��ʵ�鲽��1������

// ��ʵ�鲽��2����ʼ

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

// ��ʵ�鲽��2������

// ��ʵ�鲽��3����ʼ

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

// ��ʵ�鲽��3������

// ��ʵ�鲽��4����ʼ

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

// ��ʵ�鲽��4������

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

    printf("\nԭ�ļ���С��%ldB\n", in_size);
    printf("ѹ�����ļ���С��%ldB\n", out_size);
    printf("ѹ���ʣ�%.2f%%\n", (float)(in_size - out_size) * 100 / in_size);
}
