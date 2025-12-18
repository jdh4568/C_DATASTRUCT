#include <stdio.h>
#include <stdlib.h>

/*
    이진 탐색 트리 노드 구조체
    data 필드는 BST에서 key 값으로 사용된다.
*/
typedef struct node {
    int data;                   // 노드의 키 값
    struct node *left_child;    // 왼쪽 자식 노드
    struct node *right_child;   // 오른쪽 자식 노드
} node;

/* =====================================================
   탐색 연산 (재귀 방식)
   key를 포함한 노드의 포인터를 반환
   존재하지 않으면 NULL 반환
   ===================================================== */
node *search(node *root, int key) {
    if (!root) return NULL; // 트리가 비어있으면 NULL 반환

    if (key == root->data)  // 키가 존재하면 노드 반환
        return root;

    if (key < root->data)   // 키가 작으면 왼쪽 서브트리 탐색
        return search(root->left_child, key);

    return search(root->right_child, key);  // 키가 크면 오른쪽 서브트리 탐색
}

/* =====================================================
   탐색 연산 (반복 방식)
   ===================================================== */
node *iterSearch(node *tree, int key) {
    while (tree != NULL) {  // 트리가 비어있지 않으면 반복
        if (key == tree->data)  // 키가 존재하면 노드 반환
            return tree;

        if (key < tree->data)   // 키가 작으면 왼쪽 서브트리로 이동
            tree = tree->left_child;
        else    // 키가 크면 오른쪽 서브트리로 이동
            tree = tree->right_child;
    }
    return NULL;    // 키가 없으면 NULL 반환
}

/* =====================================================
   삽입을 위한 보조 탐색 함수
   삽입 위치의 부모 노드를 반환
   중복 키일 경우 NULL 반환
   ===================================================== */
node *modified_search(node *root, int key) {
    for (node *ptr = root; ptr != NULL; ) { // 트리가 비어있지 않으면 반복
        if (ptr->data == key)
            return NULL; // 중복 키 존재시 NULL 반환

        if (key < ptr->data) {  // 키가 데이터보다 작으면
            if (ptr->left_child == NULL) return ptr;    // 왼쪽 자식 노드가 NULL이면 현재 노드 반환
            ptr = ptr->left_child; // 왼쪽 자식 노드로 이동
        } else {
            if (ptr->right_child == NULL) return ptr;   // 오른쪽 자식 노드가 NULL이면 현재 노드 반환
            ptr = ptr->right_child; // 오른쪽 자식 노드로 이동
        }
    }
    return NULL; // 트리가 비어 있는 경우
}

/* =====================================================
   삽입 연산
   새로운 키 값을 BST에 삽입
   ===================================================== */
void insert_node(node **root, int num) {    // 트리의 값을 바꿀때는 이중 포인터 사용
    node *parent, *ptr;

    parent = modified_search(*root, num);

    /* 중복 키가 아니거나 트리가 비어 있는 경우만 삽입 */
    if (parent || !(*root)) {   //
        ptr = (node *)malloc(sizeof(node));
        ptr->data = num;
        ptr->left_child = ptr->right_child = NULL;

        if (*root) {
            if (num < parent->data)
                parent->left_child = ptr;
            else
                parent->right_child = ptr;
        } else {
            *root = ptr; // 첫 노드 (root)
        }
    }
}

/* =====================================================
   자식 노드가 2개일 때 사용할
   오른쪽 서브트리의 최소값 노드 반환
   ===================================================== */
node *min_value_node(node *ptr) {
    node *current = ptr;
    while (current && current->left_child != NULL)
        current = current->left_child;
    return current;
}

/* =====================================================
   삭제 연산
   - 리프 노드
   - 자식 노드 1개
   - 자식 노드 2개
   모든 경우를 처리
   ===================================================== */
node *delete_node(node *root, int key) {
    if (root == NULL) return root;

    if (key < root->data)
        root->left_child = delete_node(root->left_child, key);

    else if (key > root->data)
        root->right_child = delete_node(root->right_child, key);

    else {
        /* Case 1: 리프 노드 */
        if (root->left_child == NULL && root->right_child == NULL) {
            free(root);
            return NULL;
        }

        /* Case 2: 자식 노드 1개 */
        else if (root->left_child == NULL) {
            node *temp = root->right_child;
            free(root);
            return temp;
        }
        else if (root->right_child == NULL) {
            node *temp = root->left_child;
            free(root);
            return temp;
        }

        /* Case 3: 자식 노드 2개 */
        node *temp = min_value_node(root->right_child);
        root->data = temp->data;
        root->right_child = delete_node(root->right_child, temp->data);
    }
    return root;
}

/* =====================================================
   중위 순회 (BST 정렬 결과 확인용)
   ===================================================== */
void inorder(node *root) {
    if (root) {
        inorder(root->left_child);
        printf("%d ", root->data);
        inorder(root->right_child);
    }
}

/* =====================================================
   main 함수
   ===================================================== */
int main() {
    node *root = NULL;

    printf("=== 이진 탐색 트리 삽입 ===\n");
    insert_node(&root, 50);
    insert_node(&root, 30);
    insert_node(&root, 70);
    insert_node(&root, 20);
    insert_node(&root, 40);
    insert_node(&root, 60);
    insert_node(&root, 80);

    printf("중위 순회 결과: ");
    inorder(root);
    printf("\n\n");

    printf("=== 탐색 연산 ===\n");
    int key = 40;
    if (search(root, key))
        printf("재귀 탐색: %d 값이 트리에 존재합니다.\n", key);
    else
        printf("재귀 탐색: %d 값이 트리에 존재하지 않습니다.\n", key);

    if (iterSearch(root, key))
        printf("반복 탐색: %d 값이 트리에 존재합니다.\n\n", key);
    else
        printf("반복 탐색: %d 값이 트리에 존재하지 않습니다.\n\n", key);

    printf("=== 삭제 연산 ===\n");

    printf("리프 노드 20 삭제 후: ");
    root = delete_node(root, 20);
    inorder(root);
    printf("\n");

    printf("자식 노드 1개인 30 삭제 후: ");
    root = delete_node(root, 30);
    inorder(root);
    printf("\n");

    printf("자식 노드 2개인 50 삭제 후: ");
    root = delete_node(root, 50);
    inorder(root);
    printf("\n");

    return 0;
}
