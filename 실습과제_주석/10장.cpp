#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
    ===== 인접 리스트 기반 무방향 그래프 + DFS/BFS 실습 =====

    [그래프 표현 방식]
    - 인접 리스트 (Adjacency List)
    - graph[i] : 정점 i에 인접한 노드들의 연결 리스트

    [구현 기능]
    10.1) 무방향 그래프 생성 + 각 정점의 degree 계산
    10.2) 깊이 우선 탐색(DFS), 너비 우선 탐색(BFS)
    10.3) DFS / BFS 기반 신장 트리(Spanning Tree) 생성

    [그래프 특성]
    - 무방향 그래프 (insert_edge에서 양방향 삽입)
    - 최대 정점 수: MAX_VERTICES
*/

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

/* ===== 인접 리스트 노드 ===== */
typedef struct node {
    int vertex;          // 인접한 정점 번호
    struct node* link;   // 다음 인접 노드
} Node;

/* ===== BFS용 큐 노드 ===== */
typedef struct queue {
    int vertex;          // 큐에 저장될 정점 번호
    struct queue* link;  // 다음 큐 노드
} Queue;

/* ===== 전역 변수 ===== */
Node* graph[MAX_VERTICES];   // 인접 리스트 배열
short int visited[MAX_VERTICES]; // 방문 여부 배열
int n;                       // 정점 개수

/*
    ===== init_graph =====
    - 그래프 초기화
    - 각 정점의 인접 리스트를 NULL로 설정
*/
void init_graph(int n) {
    int i;
    for (i = 0; i < n; i++) {
        graph[i] = NULL;
    }
}

/*
    ===== insert_edge =====
    - 무방향 그래프에서 간선 (u, v) 삽입

    동작:
    1) u → v 삽입
    2) v → u 삽입

    → 무방향 그래프이므로 양쪽 모두 연결
*/
void insert_edge(int u, int v) {
    Node* p;

    // u의 인접 리스트에 v 추가
    p = (Node*)calloc(1, sizeof(Node));
    p->vertex = v;
    p->link = graph[u];
    graph[u] = p;

    // v의 인접 리스트에 u 추가
    p = (Node*)calloc(1, sizeof(Node));
    p->vertex = u;
    p->link = graph[v];
    graph[v] = p;
}

/*
    ===== addq =====
    - BFS를 위한 큐 삽입(enqueue)

    front, rear를 이중 포인터로 받는 이유:
    - 큐가 비어 있을 때 front와 rear를 동시에 변경해야 하기 때문
*/
void addq(Queue** front, Queue** rear, int v) {
    Queue* temp = (Queue*)calloc(1, sizeof(Queue));
    temp->vertex = v;
    temp->link = NULL;

    if (*rear) {
        (*rear)->link = temp;
    }
    else {
        *front = temp;
    }
    *rear = temp;
}

/*
    ===== deleteq =====
    - BFS를 위한 큐 삭제(dequeue)

    반환값:
    - 큐에서 제거된 정점 번호
*/
int deleteq(Queue** front, Queue** rear) {
    Queue* temp = *front;
    int v = temp->vertex;

    *front = temp->link;
    if (*front == NULL)
        *rear = NULL;

    free(temp);
    return v;
}

/*
    ===== print_graph_and_degree =====
    - 인접 리스트 출력
    - 각 정점의 degree(차수) 계산
    - degree가 가장 큰 정점 출력
*/
void print_graph_and_degree(void) {
    int i, degree, max_degree = -1, max_vertex = -1;
    Node* p;

    printf("\n  인접 리스트를 이용하여 구성한 그래프\n");
    for (i = 0; i < n; i++) {
        printf("   [%d]:", i);
        degree = 0;

        for (p = graph[i]; p != NULL; p = p->link) {
            printf("  %d ->", p->vertex);
            degree++;  // 인접 노드 수 = degree
        }
        printf(" end\n");

        // 최대 degree 정점 갱신
        if (degree > max_degree) {
            max_degree = degree;
            max_vertex = i;
        }
    }

    printf("\n  degree가 가장 큰 노드와 degree 값 : 노드 %d, degree %d\n",
        max_vertex, max_degree);

    printf("  노드 %d와 연결된 노드들 :", max_vertex);
    for (p = graph[max_vertex]; p != NULL; p = p->link) {
        printf("  %d ->", p->vertex);
    }
    printf(" end\n");
}

/*
    ===== dfs (깊이 우선 탐색) =====
    - 재귀 호출 방식
    - 방문한 정점을 즉시 출력

    알고리즘:
    1) 현재 정점 방문 표시
    2) 인접한 정점 중 방문하지 않은 정점으로 재귀 이동
*/
void dfs(int v) {
    Node* w;

    visited[v] = TRUE;
    printf("%5d", v);

    for (w = graph[v]; w; w = w->link) {
        if (!visited[w->vertex]) {
            dfs(w->vertex);
        }
    }
}

/*
    ===== bfs (너비 우선 탐색) =====
    - 큐를 이용한 반복 구조
    - 가까운 정점부터 차례대로 방문

    알고리즘:
    1) 시작 정점 방문 및 큐 삽입
    2) 큐에서 하나 꺼내 인접 정점 탐색
    3) 방문하지 않은 정점은 방문 표시 후 큐 삽입
*/
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

/*
    ===== dfs_spanning =====
    - DFS 기반 신장 트리 생성
    - 실제 간선 (부모, 자식)만 출력

    출력되는 간선 개수:
    - 연결 그래프일 경우 (n - 1)개
*/
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

/*
    ===== bfs_spanning =====
    - BFS 기반 신장 트리 생성
    - 큐를 사용하여 레벨 순서로 트리 구성
*/
void bfs_spanning(int v) {
    Node* w;
    Queue* front = NULL, * rear = NULL;

    visited[v] = TRUE;
    addq(&front, &rear, v);

    while (front) {
        v = deleteq(&front, &rear);

        for (w = graph[v]; w; w = w->link) {
            if (!visited[w->vertex]) {
                visited[w->vertex] = TRUE;
                printf("  (%d, %d)", v, w->vertex);
                addq(&front, &rear, w->vertex);
            }
        }
    }
}

/*
    ===== free_graph =====
    - 그래프의 모든 인접 리스트 노드 메모리 해제
*/
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

/*
    ===== main =====
*/
int main(void) {
    int i;
    int u, v;

    printf("10.1. 인접 리스트 형태의 무방향성 그래프 생성\n");
    printf("  노드 개수(n): ");
    if (scanf("%d", &n) != 1)
        return 0;

    init_graph(n);

    // 간선 입력
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

    /*
        ===== DFS =====
        - 짝수 정점을 시작 노드로 사용
    */
    printf("10.2. 탐색 알고리즘(DFS, BFS)\n");
    printf("  깊이 우선 탐색 (DFS)\n");
    for (v = 0; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        dfs(v);
        printf("\n");
    }

    /*
        ===== BFS =====
        - 홀수 정점을 시작 노드로 사용
    */
    printf("\n  너비 우선 탐색 (BFS)\n");
    for (v = 1; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        bfs(v);
        printf("\n");
    }

    /*
        ===== DFS 신장 트리 =====
    */
    printf("\n10.3. 신장트리 알고리즘\n");
    printf("  DFS 신장트리\n");
    for (v = 0; v < n; v += 2) {
        printf("  시작노드 %d :", v);
        for (i = 0; i < n; i++) visited[i] = FALSE;
        dfs_spanning(v);
        printf("\n");
    }

    free_graph();
    return 0;
}
