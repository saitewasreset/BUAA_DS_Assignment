/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 10005

void remove_blank(char *str);
char *replace(char *begin, char *end, char *content);
char *s_gets(char *dst, int n);
char *calc_and_replace(char *s);
void calc_and_replace_first_class(char *s);
void calc_and_replace_second_class(char *s);
void reverse(char *s);
void itoa(int a, char *s);
char *to_last_op(char *s, char *begin_border);
int find_next_first_class(char *s);
int find_next_second_class(char *s);
void process_result(char *s);

int main(void) {
    /*
    char str[MAXLEN] = {0};
    s_gets(str, MAXLEN);
    remove_blank(str);
    char test[] = "123*456";
    calc_and_replace(test);
    */
    /*
    char test[100] = "abcdef";
    char content[] = "1";
    replace(test + 1, test + 3, content);
    puts(test);
    */
    /*
    char test[] = "abcde";
    reverse(test);
    puts(test);
    */
    /*
    char test[100] = {0};
    itoa(-23456, test);
    puts(test);
    */
    /*
    char test[100] = "234*567+123";
    char * ret = to_last_op(test + 7, test);
    puts(test);
    puts(ret);*/
    char str[MAXLEN] = {0};
    s_gets(str, MAXLEN);
    remove_blank(str);
    calc_and_replace_first_class(str);
    calc_and_replace_second_class(str);
    process_result(str);
    puts(str);
    return 0;
}

char *s_gets(char *dst, int n) {
    char *ret = fgets(dst, n, stdin);
    int i = 0;
    if (ret != NULL) {
        while (dst[i] != '\n' && dst[i] != '\0') {
            i++;
        }
        if (dst[i] == '\n') {
            dst[i] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                continue;
            }
        }
    }

    return ret;
}

void remove_blank(char *str) {
    int i, j;
    for (i = j = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}

// return a pointer points to next character after the result
char *calc_and_replace(char *s) {
    char *_s = s;
    int sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    int op1 = 0;
    while (*s >= '0' && *s <= '9') {
        op1 = 10 * op1 + *s - '0';
        s++;
    }
    char o = *s;
    s++;
    op1 *= sign;
    int op2 = 0;
    while (*s >= '0' && *s <= '9') {
        op2 = 10 * op2 + *s - '0';
        s++;
    }

    int result = 0;
    switch (o) {
    case '+':
        result = op1 + op2;
        break;
    case '-':
        result = op1 - op2;
        break;
    case '*':
        result = op1 * op2;
        break;
    case '/':
        result = op1 / op2;
        break;
    }

    char result_str[MAXLEN] = {0};
    itoa(result, result_str);
    char *new_end = replace(_s, s, result_str);
    return new_end;
}

// [begin, end)
// return a pointer points to end(in new place)
char *replace(char *begin, char *end, char *content) {
    int end_len = (int)strlen(end);
    char *end_temp = (char *)malloc((unsigned)(end_len + 1) * sizeof(char));
    strcpy(end_temp, end);
    for (; *content != '\0'; begin++, content++) {
        *begin = *content;
    }
    char *ret = begin;
    for (int i = 0; i < end_len; i++) {
        *begin = end_temp[i];
        begin++;
    }
    *begin = '\0';
    free(end_temp);
    return ret;
}

void reverse(char *s) {
    char temp = 0;
    for (int i = 0, j = (int)strlen(s) - 1; i < j; i++, j--) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

void itoa(int a, char *s) {
    int i = 0;
    int sign = a;
    if (a < 0) {
        a = -a;
    }
    if (a == 0) {
        s[0] = '0';
        s[1] = '\0';
        return;
    }
    while (a > 0) {
        s[i] = (char)(a % 10) + '0';
        a /= 10;
        i++;
    }
    if (sign > 0) {
        s[i] = '\0';
    } else {
        s[i] = '-';
        s[i + 1] = '\0';
    }

    reverse(s);
}

// * /
void calc_and_replace_first_class(char *s) {
    int next = 0;
    char *_s = s;
    while ((next = find_next_first_class(s)) != -1) {
        char *begin = to_last_op(s + next, _s);
        char *next_begin = calc_and_replace(begin);
        s = next_begin;
    }
}
// + -
void calc_and_replace_second_class(char *s) {
    int next = 0;
    char *_s = s;
    while ((next = find_next_second_class(s)) != -1) {
        char *begin = to_last_op(s + next, _s);
        char *next_begin = calc_and_replace(begin);
        s = next_begin;
    }
}

int find_next_first_class(char *s) {
    int len = (int)strlen(s);
    int first_pos = -1;
    for (int i = 0; i < len; i++) {
        if (s[i] == '*' || s[i] == '/') {
            first_pos = i;
            break;
        }
    }
    return first_pos;
}

int find_next_second_class(char *s) {
    int len = (int)strlen(s);
    int first_pos = -1;
    for (int i = 0; i < len; i++) {
        if (s[i] == '+' || s[i] == '-') {
            first_pos = i;
            break;
        }
    }
    return first_pos;
}
char *to_last_op(char *s, char *begin_border) {
    // skip current op
    s--;
    for (; s != begin_border; s--) {
        if (isdigit(*s)) {
            continue;
        } else {
            break;
        }
    }
    if (s == begin_border) {
        return begin_border;
    } else {
        return s + 1;
    }
}

void process_result(char *s) {
    int len = (int)strlen(s);
    s[len - 1] = '\0';
}