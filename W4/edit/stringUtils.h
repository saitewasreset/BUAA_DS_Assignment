#ifndef __STRING_UTILS_H
#define __STRING_UTILS_H

#include <stddef.h>

#define MAXLEN 550
void String_truncate(char *str, size_t i);
void String_insert(char *target, char *a, long i);
void String_delete(char *target, size_t i, size_t j);

void String_replace(char *target, char *str, size_t i, size_t j);
void String_fullFind(char *str, char *target);

#endif