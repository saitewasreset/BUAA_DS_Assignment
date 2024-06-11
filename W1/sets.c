#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define RESIZE_FACTOR 2U
#define DEFAULT_CAPACITY 10000U

typedef int Item;

struct List {
    Item * data;
    size_t len;
    size_t capacity;
};

struct List * newList(size_t capacity);
void freeList(struct List * target);
Item get(struct List * target, size_t index);
void append(struct List * target, Item a);
void read(struct List * target);
int inList(struct List * target, Item a);

int main(void)
{
    struct List * a = newList(DEFAULT_CAPACITY);
    struct List * b = newList(DEFAULT_CAPACITY);

    read(a);
    read(b);

    for (size_t i = 0; i < a->len; i ++)
    {
        if (!inList(b, get(a, i)))
        {
            printf("%d ", get(a, i));
        }
        
    }

    freeList(a);
    freeList(b);
    return 0;
}

struct List * newList(size_t capacity)
{
    struct List * result = (struct List *) malloc(sizeof(struct List));
    result->data = (Item *) malloc(capacity * sizeof(Item));
    result->capacity = capacity;
    result->len = 0;
    return result; 
}

void freeList(struct List * target)
{
    free(target->data);
    free(target);
}

inline Item get(struct List * target, size_t index)
{
    return target->data[index];
}

inline void append(struct List * target, Item a)
{
    if (target->len >= target->capacity)
    {
        size_t new_capacity = target->capacity * RESIZE_FACTOR;
        target->data = realloc(target->data, new_capacity * sizeof(Item));
        target->capacity = new_capacity;
    }
    target->data[target->len] = a;
    target->len ++; 
}

void read(struct List * target)
{
    Item temp = 0;
    while (1)
    {
        scanf("%d", &temp);
        if (temp == -1)
        {
            break;
        }
        append(target, temp);
    }
    
}

int inList(struct List * target, Item a)
{
    for (size_t i = 0; i < target->len; i ++)
    {
        if (get(target, i) == a)
        {
            return 1;
        }
    }
    return 0;
}