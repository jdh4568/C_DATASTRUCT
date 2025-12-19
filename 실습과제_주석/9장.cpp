#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
    ===== 이진 탐색 트리(BST) 실습 코드 =====

    [프로그램 구성]
    9.1) 미리 정해진 12개의 (key, value) 쌍을 BST에 삽입하여 트리 생성
    9.2) 사용자로부터 key를 입력받아 BST에서 검색(재귀) + 비교 횟수(search_count) 출력
    9.3) 임의의 key로 n개 노드를 삽입하여 BST 생성 시간을 측정하고,
         생성된 트리의 (노드 수 / 높이 / 단말 노드 수) 관찰

    [BST 성질]
    - 어떤 노드의 key 기준:
      leftChild 서브트리의 모든 key < 현재 key
      rightChild 서브트리의 모든 key > 현재 key
*/

typedef struct {
    int key;        // 검색/정렬 기준이 되는 key
    double value;   // key에 대응되는 값 (여기서는 1.0/key)
} element;

/*
    node 구조체를 가리키는 포인터 타입을 treePointer로 정의
    - treePointer는 "트리 노드 주소"를 담는 포인터 변수
*/
typedef struct node* treePointer;

/*
    BST의 노드 구조체
    - data: (key, value)
    - leftChild, rightChild: 좌/우 자식 노드 포인터
*/
typedef struct node {
    element data;
    treePointer leftChild;
    treePointer rightChild;
} node;

/*
    search() 수행 중 "몇 개의 노드를 방문했는지" 세기 위한 전역 변수
    - 검색 과정에서 재귀 호출될 때마다 1씩 증가
*/
int search_count;

/*
    ===== BST 검색(재귀) =====
    tree: 현재 보고 있는 노드(서브트리 root)
    k   : 찾고 싶은 key

    반환:
    - 찾으면 해당 노드의 element 주소 반환 (&(tree->data))
    - 못 찾으면 NULL 반환

    특징:
    - BST 성질을 이용하여 left 또는 right로만 내려감
    - 시간복잡도: 평균 O(log n), 최악(편향 트리) O(n)
*/
element* search(treePointer tree, int k) {
    // 현재 노드가 NULL이면 더 내려갈 곳이 없으므로 실패
    if (!tree) return NULL;

    // 노드 하나를 방문했으므로 count 증가
    search_count++;

    // 현재 노드의 key가 찾는 key와 같으면 성공 -> data 주소 반환
    if (k == tree->data.key) return &(tree->data);

    // 찾는 key가 더 작으면 왼쪽 서브트리로 재귀 검색
    if (k < tree->data.key) {
        return search(tree->leftChild, k);
    }
    // 찾는 key가 더 크면 오른쪽 서브트리로 재귀 검색
    else {
        return search(tree->rightChild, k);
    }
}

/*
    ===== modified_search (삽입을 위한 탐색) =====
    tree: BST 루트
    key : 삽입하려는 key

    반환:
    - 만약 key가 트리에 이미 존재하면: 그 노드 포인터 반환
    - 존재하지 않으면: "삽입될 위치의 부모 노드" 포인터 반환
      (즉, NULL에 도달하기 직전의 마지막 유효 노드)

    사용 이유:
    - insert에서 부모를 알아야 leftChild인지 rightChild인지 연결 가능
*/
treePointer modified_search(treePointer tree, int key) {
    treePointer temp = NULL;  // 마지막으로 방문한 노드(부모 후보)

    // tree가 NULL이 될 때까지 내려감
    while (tree) {
        temp = tree; // 현재 노드를 기억(부모 후보)
        if (key == tree->data.key) return tree; // 이미 존재하면 바로 반환

        if (key < tree->data.key) {
            tree = tree->leftChild; // 왼쪽으로 이동
        }
        else {
            tree = tree->rightChild; // 오른쪽으로 이동
        }
    }
    // tree가 NULL이 된 시점: 삽입 위치를 찾았고 temp는 부모
    return temp;
}

/*
    ===== BST 삽입 =====
    node : "루트 포인터의 주소" (treePointer* 로 받는 이유)
           - 트리가 비어있을 때(*node == NULL) 새 노드를 root에 넣어야 해서
             main의 T1 자체를 바꾸려면 주소로 받아야 함.
    k    : 삽입할 key
    theItem: 삽입할 value

    동작:
    1) modified_search로 부모(temp) 또는 동일 key 노드를 찾음
    2) 동일 key가 이미 있으면 삽입하지 않고 return
    3) 새 노드(ptr) 할당 후 값 채움
    4) 트리가 비어있으면 *node = ptr (root 설정)
       아니면 부모의 left/right에 연결
*/
void insert(treePointer* node, int k, double theItem) {
    treePointer ptr, temp;

    // 삽입될 위치의 부모(temp) 또는 동일 key 노드 탐색
    temp = modified_search(*node, k);

    // temp가 존재하고 temp의 key가 k면, 이미 같은 key가 있음 -> 중복 삽입 방지
    if (temp && temp->data.key == k) {
        return;
    }

    // 새 노드 메모리 할당
    if (!((ptr) = (treePointer)malloc(sizeof(*ptr)))) {
        fprintf(stderr, "메모리 할당 오류\n");
        exit(1);
    }

    // 새 노드 데이터 설정
    ptr->data.key = k;
    ptr->data.value = theItem;
    ptr->leftChild = ptr->rightChild = NULL;

    // 트리가 이미 존재하면(temp는 부모)
    if (*node) {
        // 부모 key보다 작으면 왼쪽 자식으로 연결
        if (k < temp->data.key) {
            temp->leftChild = ptr;
        }
        // 크면 오른쪽 자식으로 연결
        else {
            temp->rightChild = ptr;
        }
    }
    // 트리가 비어있다면 새 노드가 root
    else {
        *node = ptr;
    }
}

/*
    ===== 노드 개수 세기(재귀) =====
    ptr이 NULL이면 0
    아니면 1(현재노드) + 왼쪽 노드 수 + 오른쪽 노드 수
*/
int count_node(treePointer ptr) {
    if (ptr) {
        return 1 + count_node(ptr->leftChild) + count_node(ptr->rightChild);
    }
    return 0;
}

/*
    ===== 트리 높이(깊이) 계산(재귀) =====
    여기서 "높이(깊이)" 정의:
    - NULL의 깊이를 -1로 둠
    - 리프 노드의 깊이는 0
    - 높이 = max(left_height, right_height) + 1

    예)
    - 노드가 1개(root만): left/right는 NULL -> -1
      max(-1,-1)+1 = 0  => 높이 0
*/
int count_depth(treePointer ptr) {
    // 빈 트리(NULL)의 높이를 -1로 정의
    if (!ptr) {
        return -1;
    }

    // 왼쪽/오른쪽 서브트리 높이 계산
    int left_depth = count_depth(ptr->leftChild);
    int right_depth = count_depth(ptr->rightChild);

    // 더 큰 쪽 + 1
    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

/*
    ===== 단말(리프) 노드 수 세기(재귀) =====
    - 리프: leftChild도 NULL, rightChild도 NULL인 노드
*/
int count_leaf(treePointer ptr) {
    // 빈 트리는 리프 0개
    if (!ptr) {
        return 0;
    }

    // 자식이 둘 다 없으면 리프 1개
    if (!ptr->leftChild && !ptr->rightChild) {
        return 1;
    }

    // 아니면 왼쪽 리프 수 + 오른쪽 리프 수
    return count_leaf(ptr->leftChild) + count_leaf(ptr->rightChild);
}

/*
    ===== 랜덤 key로 BST 생성 =====
    n: 삽입 시도 횟수(중복 key가 나오면 실제 노드 수는 n보다 작을 수 있음)

    - key는 0 ~ 100,000,000 범위에서 생성
    - value는 1.0/key 로 저장(단 key==0이면 0으로 나누기라서 삽입하지 않음)
*/
treePointer make_bst(int n) {
    treePointer root = NULL;

    // rand()의 최대값을 double로 두어 0~1 사이 실수로 정규화하기 위한 값
    // 0x7fff는 MSVC 계열에서 rand() 최대값(32767)로 자주 사용되는 값
    const double MAX_RAND = 0x7fff * 1.0;

    for (int i = 0; i < n; i++) {
        // rand()/MAX_RAND -> [0,1] 근사
        // * 100000000.0 -> [0,100,000,000] 근사
        int key = (int)((rand() / MAX_RAND) * 100000000.0);

        // value를 1/key로 설정
        // key==0이면 0으로 나눔이므로 아래에서 제외
        double value = 1.0 / (double)key;

        // key가 0이면 삽입 안 함
        if (key != 0) {
            insert(&root, key, value);
        }
    }
    return root;
}

/*
    ===== 중위 순회(inorder) =====
    BST에서 중위 순회를 하면 key 오름차순으로 출력됨
    (왼쪽 -> 현재 -> 오른쪽)
*/
void inorder(treePointer ptr) {
    if (ptr) {
        inorder(ptr->leftChild);
        printf("(%10d, %f)\n", ptr->data.key, ptr->data.value);
        inorder(ptr->rightChild);
    }
}

/*
    ===== 트리 메모리 해제(후위 순회 방식) =====
    - 자식부터 free하고 마지막에 자신을 free
    - 후위 순회 (left -> right -> node)
*/
void delete_tree(treePointer ptr) {
    if (ptr) {
        delete_tree(ptr->leftChild);
        delete_tree(ptr->rightChild);
        free(ptr);
    }
}

int main() {
    // 난수 시드 설정 (실행할 때마다 다른 난수 생성)
    srand((unsigned int)time(NULL));

    /*
        =========================
        9.1) 12개 데이터로 BST 생성
        =========================
    */
    printf("9.1. 12개의 (key, value)쌍을 생성하여 이진 탐색 트리 생성\n");

    treePointer T1 = NULL; // 처음에는 빈 트리

    // 삽입할 key 목록(중복 없음)
    int keys[] = { 10, 6, 15, 8, 18, 12, 3, 14, 9, 20, 5, 2 };

    // 각 key에 대해 value = 1.0/key 로 삽입
    for (int i = 0; i < 12; i++) {
        insert(&T1, keys[i], 1.0 / (double)keys[i]);
    }

    printf("생성 완료\n\n");

    /*
        =========================
        9.2) key 검색 + 방문 횟수 출력
        =========================
        - 사용자가 key를 입력하면 search()로 찾음
        - search_count는 노드 방문 횟수(비교 횟수 느낌)로 사용
        - -1 입력 시 종료
        - scanf 실패(문자 입력 등) 시 종료
    */
    printf("9.2.\n");
    printf("생성된 이진 검색 트리에서 key값을 활용한 검색\n");

    int search_key;
    element* result;

    while (1) {
        printf("찾고자 하는 key 값을 입력하세요. ");

        // 정수가 아닌 입력이면 scanf가 1을 반환하지 않음 -> 루프 종료
        if (scanf("%d", &search_key) != 1) {
            break;
        }

        // -1이면 검색 종료
        if (search_key == -1) break;

        // 검색 카운트 초기화
        search_count = 0;

        // 검색 수행 (찾으면 element* 반환, 못 찾으면 NULL)
        result = search(T1, search_key);

        if (result) {
            // result가 가리키는 element의 key/value 출력
            printf("count : %d, key: %d의 value: %f\n",
                   search_count, result->key, result->value);
        }
        else {
            printf("key: %d 노드가 없습니다.\n", search_key);
        }
    }

    // 9.1에서 만든 트리(T1) 메모리 해제
    delete_tree(T1);

    /*
        =========================
        9.3) n개 노드 BST 생성 시간 측정 + 통계 출력
        =========================
        - 사용자로부터 n 입력
        - make_bst(n)으로 트리 생성 시간 측정
        - count_node / count_depth / count_leaf로 구조 관찰
        - -1 입력 시 종료

        주의:
        - make_bst는 중복 key가 나오면 삽입을 안 하므로
          "실제 노드 수"는 입력 n보다 작을 수도 있음.
        - BST는 입력 순서에 따라 높이가 크게 달라질 수 있음.
          난수라도 편향이 생기면 높이가 커져 성능이 나빠질 수 있음.
    */
    printf("9.3.\n");
    printf("n개의 노드를 가진 이진트리 생성시간 및 노드 수, 높이(깊이), 단말 노드 수 관찰\n");

    while (1) {
        int n;
        printf("n개의 노드를 가진 이진검색 트리 생성 (n) : ");
        scanf("%d", &n);

        if (n == -1) break;

        // 생성 시작 시간
        clock_t start = clock();

        // n개 삽입 시도(중복 때문에 실제 노드 수는 달라질 수 있음)
        treePointer B = make_bst(n);

        // 생성 종료 시간
        clock_t end = clock();

        // clock() 값 차이를 초 단위로 변환
        double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

        printf("이진검색트리 B의 생성시간: %.3f\n", time_taken);

        // 생성된 트리의 실제 노드 수 / 높이 / 리프 수 계산
        int node_count = count_node(B);
        int tree_depth = count_depth(B);
        int leaf_count = count_leaf(B);

        printf("이진검색트리 B의 노드 수 : %d\n", node_count);
        printf("이진검색트리 B의 높이(깊이) : %d\n", tree_depth);
        printf("이진검색트리 B의 단말노드 수 : %d\n\n", leaf_count);

        // 트리 메모리 해제
        delete_tree(B);
    }

    return 0;
}
