#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define INF 99999
#define FALSE 0
#define TRUE 1

/* ===== 인접 리스트 노드 ===== */
typedef struct node {
    int vertex;        // 도착 정점
    int weight;        // 가중치
    struct node* link;
} Node;

/* ===== 그래프 ===== */
typedef struct GraphType {
    int n;                  // 정점 개수
    Node* adj_list[MAX_VERTICES];
} GraphType;

/* ===== 그래프 초기화 ===== */
void init(GraphType* g) {
    g->n = 0;
    for (int i = 0; i < MAX_VERTICES; i++)
        g->adj_list[i] = NULL;
}

/* ===== 정점 삽입 ===== */
void insert_vertex(GraphType* g, int v) {
    if (g->n >= MAX_VERTICES) {
        printf("정점 개수 초과\n");
        return;
    }
    g->n++;
}

/* ===== 간선 삽입 (방향 그래프) ===== */
void insert_edge(GraphType* g, int from, int to, int weight) {
    Node* p = (Node*)malloc(sizeof(Node));
    p->vertex = to;
    p->weight = weight;
    p->link = g->adj_list[from];
    g->adj_list[from] = p;
}

/* ===== 인접 리스트 출력 ===== */
void print_graph(GraphType* g) {
    Node* p;
    printf("\n인접 리스트 그래프 구성\n");
    for (int i = 0; i < g->n; i++) {
        printf("[%d] :", i);
        for (p = g->adj_list[i]; p; p = p->link) {
            printf(" -> %d(%d)", p->vertex, p->weight);
        }
        printf("\n");
    }
}

/* ===== 최소 거리 정점 선택 ===== */
int choose(int distance[], int n, short int found[]) {
    int min = INF;
    int minpos = -1;

    for (int i = 0; i < n; i++) {
        if (distance[i] < min && !found[i]) {
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}

/* ===== 다익스트라 (인접 리스트) ===== */
void dijkstra(GraphType* g, int start) {
    int distance[MAX_VERTICES];
    short int found[MAX_VERTICES];
    Node* p;

    /* 초기화 */
    for (int i = 0; i < g->n; i++) {
        distance[i] = INF;
        found[i] = FALSE;
    }

    distance[start] = 0;

    for (int i = 0; i < g->n; i++) {
        int u = choose(distance, g->n, found);
        if (u == -1) break;

        found[u] = TRUE;

        /* 인접 리스트를 이용한 거리 갱신 */
        for (p = g->adj_list[u]; p; p = p->link) {
            if (!found[p->vertex]) {
                if (distance[u] + p->weight < distance[p->vertex]) {
                    distance[p->vertex] = distance[u] + p->weight;
                }
            }
        }
    }

    /* 결과 출력 */
    printf("Distance : ");
    for (int i = 0; i < g->n; i++)
        printf("%3d ", distance[i]);
    printf("\n");
}

/* ===== 메모리 해제 ===== */
void free_graph(GraphType* g) {
    Node* p, * tmp;
    for (int i = 0; i < g->n; i++) {
        p = g->adj_list[i];
        while (p) {
            tmp = p;
            p = p->link;
            free(tmp);
        }
        g->adj_list[i] = NULL;
    }
}

/* ===== main ===== */
int main(void) {
    GraphType g;
    int n, from, to, w, start;

    init(&g);

    printf("11.1. 인접 리스트 형태의 방향성 그래프 생성\n");
    printf("노드 수 (n) : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        insert_vertex(&g, i);

    while (1) {
        printf("에지 정보 (from to weight) : ");
        scanf("%d %d %d", &from, &to, &w);
        if (from == -1 && to == -1 && w == -1)
            break;
        insert_edge(&g, from, to, w);
    }

    print_graph(&g);

    printf("\n11.2. 최단 경로 (단일 출발점, Dijkstra)\n");
    while (1) {
        printf("시작 노드 (v) : ");
        scanf("%d", &start);
        if (start == -1)
            break;
        dijkstra(&g, start);
    }

    free_graph(&g);
    return 0;
}
