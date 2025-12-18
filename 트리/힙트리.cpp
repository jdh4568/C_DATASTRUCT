#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 200   // 힙의 최대 크기 + 1 (index 1부터 사용)
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (!n)

/*
    힙에 저장되는 요소 구조체
    key 값이 힙의 우선순위 기준이 된다.
*/
typedef struct {
    int key;    // 우선순위(비교 기준)
    /* 다른 데이터 필드 추가 가능 */
} element;

/* 힙 배열과 현재 노드 개수 */
element heap[MAX_ELEMENTS];
int n = 0;

/* =====================================================
   최대 힙 삽입 연산
   item을 힙에 삽입하면서 위로 올라가며 재정렬
   ===================================================== */
void insert_max_heap(element item, int *n) {
    int i;

    if (HEAP_FULL(*n)) {
        fprintf(stderr, "힙이 가득 찼습니다.\n");
        exit(1);
    }

    /* 힙의 마지막 위치에 새 노드 추가 */
    i = ++(*n);

    /* 부모 노드와 비교하며 위로 이동 (up-heap) */
    while ((i != 1) && (item.key > heap[i / 2].key)) {
        heap[i] = heap[i / 2];  // 부모 노드를 아래로 이동
        i /= 2;                 // 부모 위치로 이동
    }

    /* 올바른 위치에 삽입 */
    heap[i] = item;
}

/* =====================================================
   최대 힙 삭제 연산
   루트 노드(최대값)를 삭제 후 힙 재구성
   ===================================================== */
element delete_max_heap(int *n) {
    int parent, child;
    element item, temp;

    if (HEAP_EMPTY(*n)) {
        fprintf(stderr, "힙이 비어 있습니다.\n");
        exit(1);
    }

    /* 삭제할 루트 노드 저장 */
    item = heap[1];

    /* 마지막 노드를 임시 저장 */
    temp = heap[(*n)--];

    parent = 1;
    child = 2;

    /* 아래로 내려가며 재정렬 (down-heap) */
    while (child <= *n) {
        /* 두 자식 중 더 큰 자식 선택 */
        if ((child < *n) && (heap[child].key < heap[child + 1].key))
            child++;

        /* 자식보다 크거나 같으면 종료 */
        if (temp.key >= heap[child].key)
            break;

        /* 자식을 부모 위치로 올림 */
        heap[parent] = heap[child];
        parent = child;
        child *= 2;
    }

    /* 최종 위치에 temp 저장 */
    heap[parent] = temp;

    return item;
}

/* =====================================================
   힙 출력 함수 (배열 상태 확인용)
   ===================================================== */
void print_heap(int n) {
    printf("현재 힙 상태: ");
    for (int i = 1; i <= n; i++) {
        printf("%d ", heap[i].key);
    }
    printf("\n");
}

/* =====================================================
   main 함수
   ===================================================== */
int main() {
    element e;

    printf("=== 최대 힙 삽입 ===\n");

    int values[] = {30, 50, 10, 20, 60, 40};
    int size = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < size; i++) {
        e.key = values[i];
        printf("%d 삽입\n", e.key);
        insert_max_heap(e, &n);
        print_heap(n);
    }

    printf("\n=== 최대 힙 삭제 ===\n");

    while (!HEAP_EMPTY(n)) {
        e = delete_max_heap(&n);
        printf("삭제된 최대값: %d\n", e.key);
        print_heap(n);
    }

    return 0;
}
