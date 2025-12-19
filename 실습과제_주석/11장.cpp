#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1
#define INT_MAX 99999   // 무한대(연결되지 않음을 표현)

/*
    ===== 인접 행렬 기반 방향 그래프 + 최단 경로 알고리즘 =====

    [그래프 표현]
    - 인접 행렬(adj_mat)
    - adj_mat[i][j] = i → j 로 가는 가중치
    - 연결되지 않은 경우 INT_MAX
    - 자기 자신(i == j)은 비용 0

    [구현 알고리즘]
    11.1) 인접 행렬 형태의 방향 그래프 생성
    11.2) 단일 출발점 최단 경로 (Dijkstra)
    11.3) 모든 쌍 최단 경로 (Floyd-Warshall)
*/

/* ===== 그래프 구조체 ===== */
typedef struct GraphType {
    int n;   // 정점 개수
    int adj_mat[MAX_VERTICES][MAX_VERTICES]; // 인접 행렬
} GraphType;

/*
    ===== init =====
    - 그래프 초기화
    - 정점 개수 n = 0
    - 자기 자신으로 가는 비용은 0
    - 나머지는 INT_MAX로 초기화
*/
void init(GraphType* g) {
    int r, c;
    g->n = 0;

    for (r = 0; r < MAX_VERTICES; r++)
        for (c = 0; c < MAX_VERTICES; c++) {
            if (r == c)
                g->adj_mat[r][c] = 0;
            else
                g->adj_mat[r][c] = INT_MAX;
        }
}

/*
    ===== choose =====
    - Dijkstra 알고리즘에서 사용
    - 아직 방문하지 않은 정점 중
      distance 값이 가장 작은 정점 선택

    distance[] : 시작 정점으로부터의 현재까지의 최단 거리
    found[]    : 이미 최단 거리가 확정된 정점 여부

    반환값:
    - 선택된 정점 번호(minpos)
*/
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

/*
    ===== print_adj_mat =====
    - 인접 행렬을 표 형태로 출력
*/
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

/*
    ===== shortestPath (Dijkstra 알고리즘) =====
    - 단일 출발점 최단 경로 계산

    v        : 시작 정점
    cost[][] : 인접 행렬 (가중치)
    distance[] : 시작 정점으로부터의 최단 거리 결과
    found[]    : 최단 거리 확정 여부

    알고리즘 흐름:
    1) 시작 정점 v에서 직접 갈 수 있는 거리로 distance 초기화
    2) 가장 가까운 미방문 정점 u 선택
    3) u를 거쳐 다른 정점으로 가는 경로가 더 짧으면 distance 갱신
*/
void shortestPath(int v, int cost[][MAX_VERTICES],
                  int distance[], int n, short int found[])
{
    int i, u, w;

    // 초기화
    for (i = 0; i < n; i++) {
        found[i] = FALSE;
        distance[i] = cost[v][i];
    }

    found[v] = TRUE;
    distance[v] = 0;

    // 정점 수 - 2 번 반복
    for (i = 0; i < n - 2; i++) {
        u = choose(distance, n, found);
        found[u] = TRUE;

        for (w = 0; w < n; w++)
            if (!found[w])
                if (distance[u] + cost[u][w] < distance[w])
                    distance[w] = distance[u] + cost[u][w];
    }
}

/*
    ===== allCosts (Floyd-Warshall 알고리즘) =====
    - 모든 정점 쌍 간 최단 경로 계산

    distance[i][j] :
    - i에서 j로 가는 최단 경로 비용

    핵심 아이디어:
    - k를 중간 정점으로 사용했을 때
      i → k → j 경로가 더 짧으면 갱신
*/
void allCosts(int cost[][MAX_VERTICES],
              int distance[][MAX_VERTICES], int n)
{
    int i, j, k;

    // 초기 distance = cost
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            distance[i][j] = cost[i][j];

    // Floyd-Warshall
    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (distance[i][k] + distance[k][j] < distance[i][j])
                    distance[i][j] = distance[i][k] + distance[k][j];
}

/*
    ===== insert_vertex =====
    - 정점 하나 추가
    - 실제로는 n 값만 증가 (인접 행렬은 이미 초기화됨)
*/
void insert_vertex(GraphType* g, int v) {
    if (((g->n) + 1) > MAX_VERTICES) {
        fprintf(stderr, "overflow");
        return;
    }
    g->n++;
}

/*
    ===== insert_edge =====
    - 방향 그래프에서 간선 삽입
    - from → to 로 가중치 w 설정

    ⚠ 무방향 그래프라면
       adj_mat[to][from] = w 도 추가해야 함
*/
void insert_edge(GraphType* g, int from, int to, int w) {
    if (from >= g->n || to >= g->n) {
        fprintf(stderr, "vertex key error");
        return;
    }
    g->adj_mat[from][to] = w;
}

/*
    ===== main =====
*/
int main() {
    int n, from, to, w, start;
    GraphType* g;

    g = (GraphType*)malloc(sizeof(GraphType));
    init(g);

    /*
        11.1 인접 행렬 형태의 방향성 그래프 생성
    */
    printf("11.1. 인접 행렬 형태의 방향성 그래프 생성\n노드 수 (n) : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        insert_vertex(g, i);

    while (1) {
        printf("에지 정보 (from to weight) : ");
        scanf("%d %d %d", &from, &to, &w);
        if (from == -1 && to == -1 && w == -1) break;
        insert_edge(g, from, to, w);
    }

    printf("\n인접 행렬을 이용한 그래프의 구성 :\n");
    print_adj_mat(g);

    /*
        11.2 단일 출발점 최단 경로 (Dijkstra)
    */
    printf("\n11.2. 최단 경로 (단일 출발점)\n");

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

    /*
        11.3 모든 쌍 최단 경로 (Floyd-Warshall)
    */
    printf("\n11.3. 최단 경로 (모든 경로)\nAll Path Distance :\n");

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

    return 0;
}
