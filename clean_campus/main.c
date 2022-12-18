#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<math.h>
#define pi 3.141592

struct dot {
    double x;
    double y;
};

struct line {
    double dis;
    struct dot dot_1;
    struct dot dot_2;
};

struct angle {
    struct dot standard;
    struct dot dot_1;
    struct dot dot_2;
    double angle;
};

double* input(const char*, int*);
double algorithm(int, struct dot*);
struct line distance(struct dot, struct dot);
struct angle angle_algorithm(struct dot, struct dot,struct dot);
int CCW(struct dot, struct dot , struct dot );
void output(const char*, int, double*);

double* input(const char* name, int* num) {
    FILE* input_file = fopen(name, "r");

    //문제 갯수 입력
    int problem_num = 0;
    fscanf(input_file, "%d", &problem_num);
    printf("%d\n", problem_num);

    *num = problem_num;
    double* result = (double*)malloc(sizeof(double) * problem_num);
    for (int i = 0; i < problem_num; i++) {

        //점 갯수 입력
        int dot_num = 0;
        fscanf(input_file, "%d", &dot_num);
        printf("%d\n", dot_num);

        struct dot* table = (struct dot*)malloc(sizeof(struct dot) * dot_num);

        for (int k = 0; k < dot_num; k++) {
            fscanf(input_file, "%lf %lf", &table[k].x, &table[k].y);
            printf("(%lf %lf)\n", table[k].x, table[k].y);
        }

        //algorithm
        double result_each = algorithm(dot_num, table);
        result[i] = result_each;
    }
    
    fclose(input_file);
    return result;
}

double algorithm(int dot_num, struct dot* dot) {
    //시작점 긋기
    struct dot start;
    start.x = 0.0;
    start.y = 0.0;

    //graham's scan

    //가장 작은 y값 찾기
    double min_y = dot[0].y;
    struct dot min_y_dot = dot[0];
    for (int i = 1; i < dot_num; i++) {
        if (min_y > dot[i].y) {
            min_y = dot[i].y;
            min_y_dot = dot[i];
            printf("가장 작은 y값 (%lf , %lf)\n", min_y_dot.x, min_y_dot.y);
        }
    }

    printf("\n");

    //가장 작은 y값의 점에서부터 각도가 낮은 순서대로 배열에 넣기
    struct dot* angle_sort = (struct dot*)malloc(sizeof(struct dot) * (dot_num - 1));
    struct angle* angle = (struct angle*)malloc(sizeof(struct angle) * (dot_num - 1));

    //x축(비교축)
    struct dot x_axis;
    x_axis.x = min_y_dot.x + 1.0;
    x_axis.y = min_y_dot.y;

    int j = 0;
    for (int i = 0; i < dot_num; i++) {
        if (min_y_dot.x == dot[i].x && min_y_dot.y == dot[i].y) {
            continue;
        }

        //각 저장
        angle[j] = angle_algorithm(min_y_dot, x_axis, dot[i]);
        j++;
    }

    //각 정렬
    double min_angle;
    int min_idx = 0;
    for (int i = 0; i < dot_num - 1; i++) {
        min_angle = 361.0;//각 최댓값 360
        for (int k = i; k < dot_num - 1; k++) {
            if (min_angle > angle[k].angle) {
                min_angle = angle[k].angle;
                min_idx = k;
            }
        }
        struct angle temp = angle[i];
        angle[i] = angle[min_idx];
        angle[min_idx] = temp;
    }

    //정렬된 각을 통해 점 정렬
    for (int i = 0; i < dot_num - 1; i++) {
        angle_sort[i] = angle[i].dot_2;
        printf("angle_sort : (%lf,%lf) %lf\n", angle_sort[i].x, angle_sort[i].y, angle[i].angle);
    }

    //초깃값
    struct line line;
    line = distance(min_y_dot, angle_sort[0]);
    double sum_line = 0;
    sum_line += line.dis;

    struct line* connect_line = (struct line*)malloc(sizeof(struct line) * dot_num);
    int connect_line_num = 1;
    connect_line[0] = line;

    //테두리 잇기
    for (int i = 1; i < dot_num-1; i++) {
        int ccw = CCW(line.dot_1, line.dot_2, angle_sort[i]);//-1: 시계 0: 직선 1:반시계
        if (ccw > 0) {
            line = distance(line.dot_2, angle_sort[i]);
            sum_line += line.dis;
            connect_line[connect_line_num] = line;
            connect_line_num++;
            printf("ccw\n");
        }//반시계면 추가
        else if (ccw < 0) {
            sum_line -= line.dis;
            line = distance(line.dot_1, angle_sort[i]);
            sum_line += line.dis;
            connect_line[connect_line_num - 1] = line;
            printf("cw\n");
        }//시계면 
        else {
            line = distance(line.dot_2, angle_sort[i]);
            sum_line += line.dis;
            printf("st\n");
        }//직진

    }
    line = distance(line.dot_2, min_y_dot);
    sum_line += line.dis;
    connect_line[connect_line_num] = line;
    connect_line_num++;
    printf("이어진 라인: %d\n", connect_line_num);
    //마지막 잇기
     
    //최소 거리 찾기(연결된 점 중)
    double min_start = 99999.0;
    struct line start_line;
    for (int i = 0; i < connect_line_num; i++) {
        struct line line = distance(start, connect_line[i].dot_1);
        if (min_start > line.dis) {
            min_start = line.dis;
            start_line = line;
        }
    }
    printf("시작 거리 : %lf (%lf, %lf)\n", min_start, start_line.dot_2);

    // 의문점 senior classman이 군집의 바깥에 있다면 그를 포함해야 하는가?
    // 포함 한다면 마지막 line(min_y 포함) 과 start를 CCW한 후
    // line = distance(line.dot_2, min_y_dot);
    // CCW(line.dot_1, line.dot_2, start)
    // 반시계면 start_line.dis * 2
    // 시계와 직진이면 sum_line -= line.dis;
    // start_line.dis + distance(line.dot_2,start).dis;
    
    //이것은 포함하지 않는다는 가정의 알고리즘( 그저 start_line.dis *2)
    sum_line += 2 * start_line.dis;
    printf("line: %.2lf\n", sum_line);
    return sum_line;
}

struct line distance(struct dot dot_1, struct dot dot_2) {

    struct line result;
    result.dot_1 = dot_1;
    result.dot_2 = dot_2;

    //거리구하기
    double distance = sqrt(pow(dot_1.x - dot_2.x, 2) + pow(dot_1.y - dot_2.y, 2));
    result.dis = distance;

    return result;
}

struct angle angle_algorithm(struct dot standard, struct dot dot_1, struct dot dot_2) {
    
    struct angle result;
    result.standard = standard;
    result.dot_1 = dot_1;
    result.dot_2 = dot_2;

    result.angle = (atan((dot_2.y - standard.y) / (dot_2.x - standard.x))
        - atan((dot_1.y - standard.y) / (dot_1.x - standard.x))) * (180/pi);
    if (result.angle < 0) {
        result.angle += 180;
    }
    return result;
}

int CCW(struct dot standard_1, struct dot standard_2, struct dot dot) {

    return (standard_1.x * standard_2.y) + (standard_2.x * dot.y) +
        (dot.x * standard_1.y) - ((standard_2.x * standard_1.y) + 
            (dot.x * standard_2.y) + (standard_1.x * dot.y));
}

void output(const char* name, int num, double* result) {
    FILE* output_name = fopen(name, "w");

    for (int i = 0; i < num; i++) {
        fprintf(output_name, "%.2lf\n", result[i]);
    }

    fclose(output_name);
}

int main() {

    char input_file[5][20] = { "Input_1.txt","Input_2.txt","Input_3.txt","Input_4.txt","Input_5.txt" };
    char output_file[5][20] = { "Output_1.txt","Output_2.txt","Output_3.txt","Output_4.txt","Output_5.txt" };

    for (int i = 0; i < 5; i++) {
        int problem_num = 0;
        double* result = input(input_file[i], &problem_num);
        output(output_file[i], problem_num, result);
    }
}