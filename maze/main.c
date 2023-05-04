#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_LINE 100

struct table {
    int row;
    int col;
    int** table;
};
struct dot {
    int x;
    int y;
    int direct; //0.up 1.right 2.down 3.left
    int dis; // 점의 이동거리
};
struct table input(const char* name) {
    FILE* input_file = fopen(name, "r");
    if (input_file == NULL) {
        printf("error");
    }
    int row, col;
    int** table;
    fscanf(input_file, "%d %d", &row, &col);

    table = (int**)malloc(sizeof(int*) * row);

    for (int i = 0; i < row; i++) {
        table[i] = (int*)malloc(sizeof(int) * col);
    }
    //table 만들기
    for (int a = 0; a < row; a++) {
        for (int b = 0; b < col; b++) {
            fscanf(input_file, "%d", &table[a][b]);
            printf("%d ", table[a][b]);
        }
        printf("\n");
    }
    //table 체우기
    struct table result;
    result.row = row;
    result.col = col;
    result.table = table;
    return result;
}

int* findmin(int* a, int* b, int* c) {
    if (*a == 0 && *b == 0 && *c == 0) {
        return a;
    }
    else {
        int i = 1;
        while (1) {
            if (*a == i) {
                return a;
            }
            else if (*b == i) {
                return b;
            }
            else if (*c == i) {
                return c;
            }
            i++;
        }
    }
}

void array_copy(struct dot roots[], struct dot root[], int dis) {
    for (int i = 0; i <= dis; i++) {
        roots[i].x = root[i].x;
        roots[i].y = root[i].y;
        roots[i].dis = root[i].dis;
        roots[i].direct = root[i].direct;
    }
}

int maze(struct table table, struct dot p, struct dot roots[], int root_dir[], struct dot visit[]) {
    if (table.table[p.y][p.x] == 3) {
        roots[p.dis].x = p.x;
        roots[p.dis].y = p.y;
        roots[p.dis].dis = p.dis;
        roots[p.dis].direct = p.direct;
        root_dir[p.dis] = 'e';//end
        //roots에 저장
        return p.dis;

    }//end를 만나면 return 1;
    else if (table.table[p.y][p.x] == 1) {
        return 0;
    }//벽을 만나면 return 0;
    else {
        struct dot root[3][MAX_LINE];
        //각 3방향의 copy점들을 초기화
        struct dot p_copy[3];

        visit[p.dis].x = p.x;
        visit[p.dis].y = p.y;
        visit[p.dis].dis = p.dis;
        visit[p.dis].direct = p.direct;

        for (int i = 0; i < p.dis; i++) {
            if (visit[i].x == p.x && visit[i].y == p.y) {
                return 0;
            }
        }

        struct dot visit_copy[3][MAX_LINE];
        for (int i = 0; i < 3; i++) {
            array_copy(visit_copy[i], visit, p.dis);
        }

        int root_direct[3][MAX_LINE] = { {0,},{0,},{0,} };
        p_copy[0].direct = (p.direct + 3) % 4;//왼쪽
        p_copy[1].direct = (p.direct);//직진
        p_copy[2].direct = (p.direct + 1) % 4;//오른쪽

        for (int i = 0; i < 3; i++) {
            if (p_copy[i].direct == 0) {
                p_copy[i].x = p.x;
                p_copy[i].y = p.y - 1;
            }//위쪽으로 한칸
            else if (p_copy[i].direct == 1) {
                p_copy[i].x = p.x + 1;
                p_copy[i].y = p.y;
            }//오른쪽으로 한칸
            else if (p_copy[i].direct == 2) {
                p_copy[i].x = p.x;
                p_copy[i].y = p.y + 1;
            }//아래쪽으로 한칸
            else if (p_copy[i].direct == 3) {
                p_copy[i].x = p.x - 1;
                p_copy[i].y = p.y;
            }//왼쪽으로 한칸
            p_copy[i].dis = p.dis + 1;
        }

        //Recursive
        int a = maze(table, p_copy[0], root[0], root_direct[0], visit_copy[0]);
        int b = maze(table, p_copy[1], root[1], root_direct[1], visit_copy[1]);
        int c = maze(table, p_copy[2], root[2], root_direct[2], visit_copy[2]);

        int* min = findmin(&a, &b, &c);

        //return
        if (*min == 0) {
            return 0;
        }//길이 없음
        else {
            if (min == &a) {
                array_copy(roots, root[0], a);
                array_copy(root_dir, root_direct[0], a);
                root_dir[p.dis] = (char)'l';//왼쪽
            }
            else if (min == &b) {
                array_copy(roots, root[1], b);
                array_copy(root_dir, root_direct[1], b);
                root_dir[p.dis] = (char)'s';//직진
            }
            else if (min == &c) {
                array_copy(roots, root[2], c);
                array_copy(root_dir, root_direct[2], c);
                root_dir[p.dis] = (char)'r';//오른쪽
            }
            //최소 root값의 root값을 roots에 저장
            roots[p.dis].x = p.x;
            roots[p.dis].y = p.y;
            roots[p.dis].dis = p.dis;
            roots[p.dis].direct = p.direct;

            return *min;
        }//한 개 이상의 return 1값을 가지면 최소 root로 상위에 전달
    }
}

int main() {
    struct table table = input("input.txt");

    struct dot p;//시작점

    for (int i = 0; i < table.row; i++) {
        if (table.table[i][0] == 2) {
            p.x = 0;
            p.y = i;
            p.direct = 1;
            break;
        }//만약 왼편에 입구가 있으면 오른쪽을 바라봄 1
        else if (table.table[i][table.col - 1] == 2) {
            p.x = table.row - 1;
            p.y = i;
            p.direct = 3;
            break;
        }//만약 오른편에 입구가 있으면 왼쪽을 바라봄 3
    }
    for (int i = 0; i < table.col; i++) {
        if (table.table[0][i] == 2) {
            p.x = i;
            p.y = 0;
            p.direct = 2;
            break;
        }//만약 윗편에 입구가 있으면 밑쪽을 바라봄 2
        else if (table.table[table.row - 1][i] == 2) {
            p.x = i;
            p.y = table.col - 1;
            p.direct = 0;
            break;
        }//만약 오른편에 입구가 있으면 윗쪽을 바라봄 0
    }
    p.dis = 0;
    struct dot roots[MAX_LINE];
    int root_direct[MAX_LINE];//방향을 저장하는 배열
    struct dot root_visit[MAX_LINE];

    int final_dis = maze(table, p, roots, root_direct, root_visit);

    printf("dis : %d\n", final_dis);
    for (int i = 0; i <= final_dis; i++) {
        printf("(%d, %d) %c\n", roots[i].x, roots[i].y, (char)root_direct[i]);
    }
    return 0;
}
//이건 왜 안돼
