#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

/* =====================================================
   그래프 노드 구조체 (인접 리스트용)
   vertex : 연결된 정점 번호
   link   : 다음 노드를 가리키는 포인터
   ===================================================== */
typedef struct node {
    int vertex;
    struct node* link;
} node;

/* =====================================================
   그래프 표현
   graph[i] : 정점 i에 인접한 노드들의 연결 리스트의 시작 주소
   visited  : 정점 방문 여부 표시 배열 (DFS, BFS 공용)
   n        : 전체 정점 개수
   ===================================================== */
node* graph[MAX_VERTICES];
short int visited[MAX_VERTICES];
int n;

/* =====================================================
   큐 노드 구조체 (BFS에서 사용)
   vertex : 큐에 저장할 정점 번호
   link   : 다음 큐 노드를 가리킴
   ===================================================== */
typedef struct queue {
    int vertex;
    struct queue* link;
} queue;

/* =====================================================
   큐에 정점 삽입 (enqueue)
   front, rear : 큐의 앞과 뒤 포인터의 주소
   v           : 삽입할 정점 번호
   -----------------------------------------------------
   이중 포인터를 사용하는 이유:
   → front, rear 자체를 함수 내부에서 변경해야 하기 때문
   ===================================================== */
void addq(queue** front, queue** rear, int v) {
    queue* temp = (queue*)malloc(sizeof(queue)); // 새 큐 노드 생성
    temp->vertex = v;
    temp->link = NULL;

    if (*rear)                  // 큐가 비어 있지 않다면
        (*rear)->link = temp;   // 기존 rear 뒤에 연결
    else                        // 큐가 비어 있다면
        *front = temp;          // front도 새 노드를 가리킴

    *rear = temp;               // rear를 새 노드로 갱신
}

/* =====================================================
   큐에서 정점 삭제 (dequeue)
   -----------------------------------------------------
   front가 가리키는 노드를 제거하고 그 정점을 반환
   ===================================================== */
int deleteq(queue** front, queue** rear) {
    queue* temp = *front;       // 삭제할 큐 노드
    int v = temp->vertex;       // 반환할 정점 번호

    *front = temp->link;        // front를 다음 노드로 이동
    if (*front == NULL)         // 큐가 비게 되면
        *rear = NULL;           // rear도 NULL로 설정

    free(temp);                 // 메모리 해제
    return v;
}

/* =====================================================
   그래프 간선 추가 함수
   u -> v 방향의 간선을 인접 리스트에 삽입
   ===================================================== */
void insert_edge(int u, int v) {
    node* temp = (node*)malloc(sizeof(node)); // 새 그래프 노드 생성
    temp->vertex = v;                          // 연결될 정점 번호
    temp->link = graph[u];                    // 기존 리스트 앞에 연결
    graph[u] = temp;                          // 새로운 노드를 리스트의 첫 노드로 설정
}

/* =====================================================
   깊이 우선 탐색 (DFS)
   -----------------------------------------------------
   1. 현재 정점 방문 표시
   2. 인접한 정점 중 방문하지 않은 정점 재귀 호출
   ===================================================== */
void dfs(int v) {
    node* w;

    visited[v] = TRUE;        // 현재 정점 방문 표시
    printf("%5d", v);         // 방문한 정점 출력

    for (w = graph[v]; w; w = w->link)   // 인접 리스트 순회
        if (!visited[w->vertex])         // 방문하지 않았다면
            dfs(w->vertex);              // 재귀 호출
}

/* =====================================================
   너비 우선 탐색 (BFS)
   -----------------------------------------------------
   1. 시작 정점을 방문 처리 후 큐에 삽입
   2. 큐에서 정점을 하나씩 꺼내면서 인접 정점 탐색
   ===================================================== */
void bfs(int v) {
    node* w;
    queue* front = NULL;
    queue* rear = NULL;

    printf("%5d", v);         // 시작 정점 출력
    visited[v] = TRUE;        // 방문 표시
    addq(&front, &rear, v);   // 큐에 삽입

    while (front) {           // 큐가 빌 때까지 반복
        v = deleteq(&front, &rear);  // 큐에서 정점 하나 꺼냄

        for (w = graph[v]; w; w = w->link) {
            if (!visited[w->vertex]) {   // 방문하지 않은 인접 정점이면
                printf("%5d", w->vertex);
                addq(&front, &rear, w->vertex); // 큐에 삽입
                visited[w->vertex] = TRUE;      // 방문 표시
            }
        }
    }
}

/* =====================================================
   방문 배열 초기화 함수
   DFS와 BFS 실행 전 반드시 호출
   ===================================================== */
void reset_visited() {
    for (int i = 0; i < n; i++)
        visited[i] = FALSE;
}

/* =====================================================
   메인 함수
   -----------------------------------------------------
   1. 정점 개수 입력
   2. 간선 입력 (무방향 그래프)
   3. DFS 수행
   4. BFS 수행
   ===================================================== */
int main() {
    int u, v;

    printf("정점 개수 입력: ");
    scanf("%d", &n);

    // 인접 리스트 초기화
    for (int i = 0; i < n; i++)
        graph[i] = NULL;

    printf("간선 입력 (u v), 종료는 -1 -1\n");
    while (1) {
        scanf("%d %d", &u, &v);
        if (u == -1 && v == -1) break;

        insert_edge(u, v);
        insert_edge(v, u);   // 무방향 그래프이므로 반대 방향도 추가
    }

    reset_visited();
    printf("\nDFS 탐색 결과:");
    dfs(0);                  // 정점 0부터 DFS 시작

    reset_visited();
    printf("\n\nBFS 탐색 결과:");
    bfs(0);                  // 정점 0부터 BFS 시작

    printf("\n");
    return 0;
}
