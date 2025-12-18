#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

/* ===== 그래프 노드 (인접 리스트) ===== */
typedef struct node {
    int vertex;
    struct node* link;
} Node;

Node* graph[MAX_VERTICES];
short int visited[MAX_VERTICES];
int n;

/* ===== 큐 구조체 (BFS용) ===== */
typedef struct queue {
    int vertex;
    struct queue* link;
} Queue;

/* ===== 그래프 초기화 ===== */
void init_graph(int n) {
    int i;
    for (i = 0; i < n; i++)
        graph[i] = NULL;
}

/* ===== 간선 삽입 (무방향 그래프) ===== */
void insert_edge(int u, int v) {
    Node* p;

    p = (Node*)calloc(1, sizeof(Node));
    p->vertex = v;
    p->link = graph[u];
    graph[u] = p;

    p = (Node*)calloc(1, sizeof(Node));
    p->vertex = u;
    p->link = graph[v];
    graph[v] = p;
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
    int i, degree, max_degree = -1, max_vertex = -1;
    Node* p;

    printf("\n  인접 리스트를 이용하여 구성한 그래프\n");
    for (i = 0; i < n; i++) {
        printf("   [%d]:", i);
        degree = 0;
        for (p = graph[i]; p; p = p->link) {
            printf("  %d ->", p->vertex);
            degree++;
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
    for (p = graph[max_vertex]; p; p = p->link)
        printf("  %d ->", p->vertex);
    printf(" end\n");
}

/* ===== DFS ===== */
void dfs(int v) {
    Node* w;
    visited[v] = TRUE;
    printf("%5d", v);

    for (w = graph[v]; w; w = w->link) {
        if (!visited[w->vertex])
            dfs(w->vertex);
    }
}

/* ===== BFS ===== */
void bfs(int v) {
    Node* w;
    Queue* front = NULL, * rear = NULL;

    visited[v] = TRUE;
    printf("%5d", v);
    addq(&front, &rear, v);

    while (front) {
        v = deleteq(&front, &rear);
        for (w = graph[v]; w; w = w->link) {
            if (!visited[w->vertex]) {
                visited[w->vertex] = TRUE;
                printf("%5d", w->vertex);
                addq(&front, &rear, w->vertex);
            }
        }
    }
}

/* ===== DFS 신장트리 ===== */
void dfs_spanning(int v) {
    Node* w;
    visited[v] = TRUE;

    for (w = graph[v]; w; w = w->link) {
        if (!visited[w->vertex]) {
            printf("  (%d, %d)", v, w->vertex);
            dfs_spanning(w->vertex);
        }
    }
}

/* ===== BFS 신장트리 ===== */
void bfs_spanning(int start) {
    Node* w;
    int v;
    Queue* front = NULL, * rear = NULL;

    visited[start] = TRUE;
    addq(&front, &rear, start);

    while (front) {
        v = deleteq(&front, &rear);
        for (w = graph[v]; w; w = w->link) {
            if (!visited[w->vertex]) {
                visited[w->vertex] = TRUE;
                printf("  (%d, %d)", v, w->vertex);  // 신장트리 간선
                addq(&front, &rear, w->vertex);
            }
        }
    }
}

/* ===== 그래프 메모리 해제 ===== */
void free_graph(void) {
    int i;
    Node* p, * tmp;

    for (i = 0; i < n; i++) {
        p = graph[i];
        while (p) {
            tmp = p;
            p = p->link;
            free(tmp);
        }
        graph[i] = NULL;
    }
}

/* ===== main ===== */
int main(void) {
    int i, u, v;

    printf("10.1. 인접 리스트 형태의 무방향성 그래프 생성\n");
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

    free_graph();
    return 0;
}
