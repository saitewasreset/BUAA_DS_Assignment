/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>

#define MAX 19

int map[MAX][MAX] = {0};

struct Result {
    int x;
    int y;
    int win;
};

struct Result judge(int (*map)[MAX], int player);

int main(void) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            scanf("%d", &map[i][j]);
        }
    }

    struct Result r = judge(map, 1);
    if (r.win == 1) {
        printf("1:%d,%d\n", r.x + 1, r.y + 1);
        return 0;
    }
    r = judge(map, 2);
    if (r.win == 1) {
        printf("2:%d,%d\n", r.x + 1, r.y + 1);
        return 0;
    }
    printf("No\n");
    return 0;
}

struct Result judge(int (*map)[MAX], int player) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (map[i][j] == player) {
                // ------
                int left_count = 0;
                int left = 0;
                for (int k = j - 1; k >= 0; k--) {
                    if (map[i][k] == player) {
                        left_count++;
                    } else {
                        left = map[i][k];
                        break;
                    }
                }

                int right_count = 0;
                int right = 0;
                for (int k = j + 1; k < MAX; k++) {
                    if (map[i][k] == player) {
                        right_count++;
                    } else {
                        right = map[i][k];
                        break;
                    }
                }

                if (left_count + right_count + 1 == 4) {
                    if ((left == 0) || (right == 0)) {
                        struct Result r = {i, j - left_count, 1};
                        return r;
                    }
                }

                // |
                int up_count = 0;
                int up = 0;
                for (int k = i - 1; k >= 0; k--) {
                    if (map[k][j] == player) {
                        up_count++;
                    } else {
                        up = map[k][j];
                        break;
                    }
                }

                int down_count = 0;
                int down = 0;
                for (int k = i + 1; k < MAX; k++) {
                    if (map[k][j] == player) {
                        down_count++;
                    } else {
                        down = map[k][j];
                        break;
                    }
                }

                if (up_count + down_count + 1 == 4) {
                    if ((up == 0) || (down == 0)) {
                        struct Result r = {i - up_count, j, 1};
                        return r;
                    }
                }

                // xie
                int x_count = 0;
                int x = 0;
                for (int k = 0; k < MAX; k++) {
                    if (((i - k) > 0) && ((j + k) < MAX)) {
                        if (map[i - k][j + k] == player) {
                            x_count++;
                        } else {
                            x = map[i - k][j + k];
                            break;
                        }
                    } else {
                        break;
                    }
                }

                if (x_count == 4) {
                    if (x == 0) {

                        struct Result r = {i - x_count + 1, j + x_count - 1, 1};
                        return r;
                    } else {
                        if (((i + 1) < MAX) && ((j - 1) > 0)) {
                            if (map[i + 1][j - 1] == 0) {
                                struct Result r = {i - x_count + 1,
                                                   j + x_count - 1, 1};
                                return r;
                            }
                        }
                    }
                }

                x_count = 0;
                x = 0;
                for (int k = 0; k < MAX; k++) {
                    if (((i - k) > 0) && ((j - k) > 0)) {
                        if (map[i - k][j - k] == player) {
                            x_count++;
                        } else {
                            x = map[i - k][j - k];
                            break;
                        }
                    } else {
                        break;
                    }
                }

                if (x_count == 4) {
                    if (x == 0) {
                        struct Result r = {i - x_count + 1, j - x_count + 1, 1};
                        return r;
                    } else {
                        if (((i + 1) < MAX) && ((j + 1) < MAX)) {
                            if (map[i + 1][j + 1] == 0) {
                                struct Result r = {i - x_count + 1,
                                                   j - x_count + 1, 1};
                                return r;
                            }
                        }
                    }
                }
            }
        }
    }
    struct Result r = {0, 0, 0};
    return r;
}
