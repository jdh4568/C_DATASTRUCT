#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

/* ===== 전역 변수 ===== */
int graph[MAX_VERTICES][MAX_VERTICES];   // 인접 행렬
short int visited[MAX_VERTICES];
int n;

/* ===== 큐 구조체 (BFS용) ===== */
typedef struct queue {
    int vertex;
    struct queue* link;
} Queue;

/* ===== 그래프 초기화 ===== */
void init_graph(int n) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            graph[i][j] = 0;
}

/* ===== 간선 삽입 (무방향 그래프) ===== */
void insert_edge(int u, int v) {
    graph[u][v] = 1;
    graph[v][u] = 1;
}

/* ===== 큐 연산 ===== */
void addq(Queue** front, Queue** rear, int v) {
    Queue* temp = (Queue*)calloc(1, sizeof(Queue));
    temp->vertex = v;
    temp->link = NULL;

    if (*rear)
        (*rear)->link = temp;
    else
        *front = temp;

    *rear = temp;
}

int deleteq(Queue** front, Queue** rear) {
    Queue* temp = *front;
    int v = temp->vertex;

    *front = temp->link;
    if (*front == NULL)
        *rear = NULL;

    free(temp);
    return v;
}

/* ===== 그래프 출력 및 degree 계산 ===== */
void print_graph_and_degree(void) {
    int i, j, degree, max_degree = -1, max_vertex = -1;

    printf("\n  인접 행렬을 이용하여 구성한 그래프\n");
    for (i = 0; i < n; i++) {
        printf("   [%d]:", i);
        degree = 0;
        for (j = 0; j < n; j++) {
            if (graph[i][j]) {
                printf("  %d ->", j);
                degree++;
            }
        }
        printf(" end\n");

        if (degree > max_degree) {
            max_degree = degree;
            max_vertex = i;
        }
    }

    printf("\n  degree가 가장 큰 노드와 degree 값 : 노드 %d, degree %d\n",
        max_vertex, max_degree);

    printf("  노드 %d와 연결된 노드들 :", max_vertex);
    for (j = 0; j < n; j++) {
        if (graph[max_vertex][j])
            printf("  %d ->", j);
    }
    printf(" end\n");
}

/* ===== DFS ===== */
void dfs(int v) {
    int w;
    visited[v] = TRUE;
    printf("%5d", v);

    for (w = 0; w < n; w++) {
        if (graph[v][w] && !visited[w]) {
            dfs(w);
        }
    }
}

/* ===== BFS ===== */
void bfs(int v) {
    int w;
    Queue* front = NULL, * rear = NULL;

    visited[v] = TRUE;
    printf("%5d", v);
    addq(&front, &rear, v);

    while (front) {
        v = deleteq(&front, &rear);
        for (w = 0; w < n; w++) {
            if (graph[v][w] && !visited[w]) {
                visited[w] = TRUE;
                printf("%5d", w);
                addq(&front, &rear, w);
            }
        }
    }
}

/* ===== DFS 신장트리 ===== */
void dfs_spanning(int v) {
    int w;
    visited[v] = TRUE;

    for (w = 0; w < n; w++) {
        if (graph[v][w] && !visited[w]) {
            printf("  (%d, %d)", v, w);
            dfs_spanning(w);
        }
    }
}

/* ===== BFS 신장트리 ===== */
void bfs_spanning(int start) {
    int v, w;
    Queue* front = NULL, * rear = NULL;

    visited[start] = TRUE;
    addq(&front, &rear, start);

    while (front) {
        v = deleteq(&front, &rear);
        for (w = 0; w < n; w++) {
            if (graph[v][w] && !visited[w]) {
                visited[w] = TRUE;
                printf("  (%d, %d)", v, w);   // 신장트리 간선
                addq(&front, &rear, w);
            }
        }
    }
}

/* ===== main ===== */
int main(void) {
    int i, u, v;

    printf("10.1. 인접 행렬 형태의 무방향성 그래프 생성\n");
    printf("  노드 개수(n): ");
    if (scanf("%d", &n) != 1)
        return 0;

    init_graph(n);

    while (1) {
        printf("  에지 정보(vi vj) : ");
        if (scanf("%d %d", &u, &v) != 2)
            return 0;
        if (u == -1 && v == -1)
            break;
        insert_edge(u, v);
    }

    print_graph_and_degree();
    printf("\n");

    /* ===== DFS ===== */
    printf("10.2. 탐색 알고리즘\n");
    printf("  깊이 우선 탐색 (DFS)\n");
    for (v = 0; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        dfs(v);
        printf("\n");
    }

    /* ===== BFS ===== */
    printf("\n  너비 우선 탐색 (BFS)\n");
    for (v = 1; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        bfs(v);
        printf("\n");
    }

    /* ===== DFS 신장트리 ===== */
    printf("\n10.3. 신장트리 알고리즘\n");
    printf("  DFS 신장트리\n");
    for (v = 0; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        dfs_spanning(v);
        printf("\n");
    }

    /* ===== BFS 신장트리 ===== */
    printf("\n  BFS 신장트리\n");
    for (v = 1; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        bfs_spanning(v);
        printf("\n");
    }

    return 0;
}
