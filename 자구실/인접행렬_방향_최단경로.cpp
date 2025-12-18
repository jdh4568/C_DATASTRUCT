#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1
#define INT_MAX 99999

typedef struct GraphType {
    int n;
    int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;

void init(GraphType* g) {
    int r, c;
    g->n = 0;
    for (r = 0; r < MAX_VERTICES; r++)
        for (c = 0; c < MAX_VERTICES; c++){
            if (r == c) g->adj_mat[r][c] = 0;
            else g->adj_mat[r][c] = INT_MAX;
        }

}
int choose(int distance[], int n, short int found[])
{
    int i, min, minpos;

    min = INT_MAX;
    minpos = -1;

    for (i = 0; i < n; i++)
        if (distance[i] < min && !found[i]) {
            min = distance[i];
            minpos = i;
        }

    return minpos;
}
void print_adj_mat(GraphType* g) {
    for (int i = 0; i < g->n; i++) {
        printf("\t[%2d]", i);
    }
    printf("\n");
    for (int i = 0; i < g->n; i++) {
        printf("[%2d]\t", i);
        for (int j = 0; j < g->n; j++) {
            printf("%d\t", g->adj_mat[i][j]);
        }
        printf("\n");
    }
}

void shortestPath(int v, int cost[][MAX_VERTICES], int distance[], int n, short int found[])
{
    int i, u, w;

    for (i = 0; i < n; i++) {
        found[i] = FALSE;
        distance[i] = cost[v][i];
    }

    found[v] = TRUE;
    distance[v] = 0;

    for (i = 0; i < n - 2; i++) {
        u = choose(distance, n, found);
        found[u] = TRUE;
        for (w = 0; w < n; w++)
            if (!found[w])
                if (distance[u] + cost[u][w] < distance[w])
                    distance[w] = distance[u] + cost[u][w];
    }
}



void allCosts(int cost[][MAX_VERTICES],
    int distance[][MAX_VERTICES], int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            distance[i][j] = cost[i][j];

    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (distance[i][k] + distance[k][j] < distance[i][j])
                    distance[i][j] = distance[i][k] + distance[k][j];
}

void insert_vertex(GraphType* g, int v) {
    if (((g->n) + 1) > MAX_VERTICES) {
        fprintf(stderr, "overflow");
        return;
    }
    g->n++;
}

void insert_edge(GraphType* g, int from, int to, int w) {
    if (from >= g->n || to >= g->n) {
        fprintf(stderr, "vertex key error");
        return;
    }
    g->adj_mat[from][to]= w;
}


int main() {
    int n, from, to, w, start;
    GraphType* g;
    g = (GraphType*)malloc(sizeof(GraphType));
    init(g);
    // 1
    printf("﻿﻿11.1. 인접 행렬 형태의 방향성 그래프 생성\n노드 수 (n) : ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        insert_vertex(g, i);
    while (1) {
        printf("﻿에지 정보 (from to weight) : ");
        scanf("%d %d %d", &from, &to, &w);
        if (from == -1 && to == -1 && w == -1) break;
        insert_edge(g, from, to, w);
        
    }
    printf("\n﻿인접 행렬을 이용한 그래프의 구성 :\n");
    print_adj_mat(g);


    // 2
    printf("\n﻿11.2. 최단 경로 (단일 출발점)\n");

    int distance[MAX_VERTICES];
    short int found[MAX_VERTICES];

    while (1) {
        printf("\n시작 노드 (v) : ");
        scanf("%d", &start);

        if (start == -1)
            break;

        shortestPath(start, g->adj_mat, distance, g->n, found);

        printf("Distance : ");
        for (int i = 0; i < g->n; i++) {
            printf("%3d ", distance[i]);
        }
        printf("\n");
    }


    // 3
    printf("﻿\n11.3. 최단 경로 (모든 경로)\nAll Path Distance :\n");
    int allDist[MAX_VERTICES][MAX_VERTICES];
    allCosts(g->adj_mat, allDist, g->n);


    printf("\t");
    for (int i = 0; i < g->n; i++) {
        printf("[%2d]\t", i);
    }
    printf("\n");



    for (int i = 0; i < g->n; i++) {
        printf("[%2d]\t", i);
        for (int j = 0; j < g->n; j++) {
            printf("%d\t", allDist[i][j]);
        }
        printf("\n");
    }

}