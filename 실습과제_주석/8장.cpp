#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    ===== 이진 트리(Binary Tree) 실습 코드 =====

    [구현 기능]
    8.1) 코드 기반 이진 트리 생성
    8.2) 트리 순회 (전위 / 중위 / 후위)
    8.3) 트리 복사(copy)
    8.4) 트리 동질성 검사(equal)
    8.5) 트리 좌우 교환(swap)
    8.6) 랜덤 구조 이진 트리 생성

    [트리 구조]
    - 각 노드는 정수 데이터(data)와
      왼쪽 자식(lchild), 오른쪽 자식(rchild)을 가진다.
*/

/* ===== 이진 트리 노드 정의 ===== */
typedef struct node {
    int data;              // 노드에 저장될 데이터
    struct node* lchild;   // 왼쪽 자식 노드
    struct node* rchild;   // 오른쪽 자식 노드
} node;

/*
    ===== getNode =====
    - 새로운 트리 노드를 동적 할당하여 생성
    - data 값을 저장하고, 자식 포인터는 NULL로 초기화

    반환값:
    - 생성된 노드의 주소
*/
node* getNode(int data) 
{
    node* newNode = (node*)malloc(sizeof(node));

    newNode->data = data;
    newNode->lchild = NULL;
    newNode->rchild = NULL;

    return newNode;
}

/*
    ===== addNode (이진 탐색 트리 방식 삽입) =====
    - node : 현재 서브트리의 루트
    - data : 삽입할 값

    규칙:
    - 현재 노드가 NULL이면 새 노드 생성
    - data < node->data  → 왼쪽 서브트리에 삽입
    - data >= node->data → 오른쪽 서브트리에 삽입

    반환값:
    - 삽입 후 서브트리의 루트
*/
node* addNode(node* node, int data) 
{
    // 트리가 비어 있으면 새 노드를 루트로 생성
    if (node == NULL)
    {
        node = getNode(data);
    }
    // 삽입 값이 현재 노드보다 작으면 왼쪽으로
    else if (node->data > data)
    {
        node->lchild = addNode(node->lchild, data);
    }
    // 삽입 값이 크거나 같으면 오른쪽으로
    else
    {
        node->rchild = addNode(node->rchild, data);
    }

    return node;
}

/*
    ===== inorder (중위 순회) =====
    순서: Left → Root → Right
    - 이진 탐색 트리에서는 오름차순 출력 효과
*/
void inorder(node* root)
{
    if (root == NULL) return;

    inorder(root->lchild);
    printf("%d ", root->data);
    inorder(root->rchild);
}

/*
    ===== preorder (전위 순회) =====
    순서: Root → Left → Right
    - 트리 구조를 복사하거나 저장할 때 유용
*/
void preorder(node* root)
{
    if (root == NULL) return;

    printf("%d ", root->data);
    preorder(root->lchild);
    preorder(root->rchild);
}

/*
    ===== postorder (후위 순회) =====
    순서: Left → Right → Root
    - 트리 삭제, 메모리 해제 시 자주 사용
*/
void postorder(node* root)
{
    if (root == NULL) return;

    postorder(root->lchild);
    postorder(root->rchild);
    printf("%d ", root->data);
}

/*
    ===== make_tree_by_code =====
    - 코드에 의해 고정된 구조의 이진 탐색 트리 생성

            5
           / \
          3   8
           \
            4
           /
          1

    반환값:
    - 생성된 트리의 루트 노드
*/
node* make_tree_by_code() {
    node* tree = NULL;
    tree = addNode(tree, 5);
    tree = addNode(tree, 3);
    tree = addNode(tree, 4);
    tree = addNode(tree, 1);
    tree = addNode(tree, 8);
    return tree;
}

/*
    ===== copy (트리 복사) =====
    - original 트리를 완전히 새로운 트리로 복사
    - 구조와 데이터 모두 동일하지만, 주소는 전부 다름

    알고리즘:
    1) 현재 노드 복사
    2) 왼쪽 서브트리 재귀 복사
    3) 오른쪽 서브트리 재귀 복사
*/
node* copy(node* original)
{
    if (original == NULL)
        return NULL;

    node* temp = (node*)malloc(sizeof(node));
    temp->data = original->data;

    temp->lchild = copy(original->lchild);
    temp->rchild = copy(original->rchild);

    return temp;
}

/*
    ===== equal (트리 동질성 검사) =====
    - 두 트리가 구조와 데이터가 완전히 같은지 검사

    TRUE 조건:
    1) 둘 다 NULL
    2) 둘 다 NULL이 아니고
       - data 동일
       - 왼쪽 서브트리 동일
       - 오른쪽 서브트리 동일
*/
int equal(node* first, node* second)
{
    return ((!first && !second) ||
        (first && second &&
            first->data == second->data &&
            equal(first->lchild, second->lchild) &&
            equal(first->rchild, second->rchild)));
}

/*
    ===== insert_random =====
    - 랜덤 방향(왼쪽/오른쪽)으로 내려가면서 노드 삽입
    - 이진 탐색 트리가 아닌 "임의 구조 이진 트리" 생성 목적

    특징:
    - rand() % 2 → 0(왼쪽) / 1(오른쪽)
    - 비어있는 자식 위치에 삽입되면 종료
*/
node* insert_random(node* root, int value)
{
    if (root == NULL) {
        return getNode(value); 
    }

    node* cur = root;

    while (1) {
        int dir = rand() % 2;  // 0 or 1

        if (dir == 0) {  // 왼쪽 선택
            if (cur->lchild == NULL) {
                cur->lchild = getNode(value);
                break;
            }
            else {
                cur = cur->lchild;
            }
        }
        else {           // 오른쪽 선택
            if (cur->rchild == NULL) {
                cur->rchild = getNode(value);
                break;
            }
            else {
                cur = cur->rchild;
            }
        }
    }

    return root;
}

/*
    ===== swap (트리 좌우 반전) =====
    - 모든 노드에 대해 왼쪽과 오른쪽 자식을 교환
    - 원본 트리는 유지하고, 새로운 트리를 생성

    효과:
    - 트리를 거울 대칭(mirror) 형태로 변환
*/
node* swap(node* original)
{
    if (original == NULL)
        return NULL;

    node* temp = (node*)malloc(sizeof(node));
    temp->data = original->data;

    // 좌우 자식을 서로 바꿔서 복사
    temp->lchild = swap(original->rchild);
    temp->rchild = swap(original->lchild);

    return temp;
}

/*
    ===== make_random_tree =====
    - n개의 노드를 가지는 랜덤 구조 이진 트리 생성
    - 값은 1부터 n까지 순차 삽입

    반환값:
    - 생성된 트리의 루트
*/
node* make_random_tree(int n)
{
    if (n <= 0) return NULL;

    node* root = getNode(1);

    for (int i = 2; i <= n; i++) {
        root = insert_random(root, i);
    }

    return root;
}

/*
    ===== main =====
    - 각 기능별 테스트 및 출력
*/
int main() {

    node* A;
    printf("8.1.1. 트리 구성 (A = make_tree_by_code())\n");
    printf("8.1.2. 트리 순회 (A)");

    A = make_tree_by_code();

    printf("\nInorder(A)  : ");
    inorder(A);

    printf("\nPreorder(A) : ");
    preorder(A);

    printf("\nPostorder(A) : ");
    postorder(A);

    /* ===== 트리 복사 ===== */
    printf("\n\n8.2.1. 트리 복사 (B = copy(A))\n");
    printf("8.2.2. 트리 순회(B)");

    node* B = copy(A);

    printf("\nInorder(B)  : ");
    inorder(B);

    printf("\nPreorder(B) : ");
    preorder(B);

    printf("\nPostorder(B) : ");
    postorder(B);

    /* ===== 동질성 검사 ===== */
    printf("\n\n8.2.3. 트리 동질성 검사 (equal(A,B))\n");
    if (equal(A, B))
        printf("equal(A, B) : TRUE\n");
    else
        printf("equal(A, B) : FALSE\n");

    /* ===== 좌우 반전 ===== */
    printf("\n\n8.2.4. 트리 swap (C = swap(A))\n");
    printf("8.2.5. 트리 순회 (C)");

    node* C = swap(A);

    printf("\nInorder(C)  : ");
    inorder(C);

    printf("\nPreorder(C) : ");
    preorder(C);

    printf("\nPostorder(C) : ");
    postorder(C);

    /* ===== 랜덤 트리 생성 ===== */
    printf("\n\n8.3.1. 랜덤 트리 자동 생성 (D)\n");

    int n;
    printf("\ninput n : ");
    scanf("%d", &n);

    srand((unsigned int)time(NULL)); 

    node* D = make_random_tree(n);

    printf("\n8.3.2. 트리 순회 (D)");

    printf("\nInorder (D) : ");
    inorder(D);

    printf("\nPreorder (D) : ");
    preorder(D);

    printf("\nPostorder (D) : ");
    postorder(D);

    printf("\n");

    return 0;
}
