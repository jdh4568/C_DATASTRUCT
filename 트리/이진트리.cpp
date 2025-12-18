#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

/* ===== 노드 정의 ===== */
typedef struct node* tree_pointer;

struct node {
    int data;
    struct node* left_child;
    struct node* right_child;
};

/* ===== 스택 (iterative inorder용) ===== */
int top = -1;
tree_pointer stack[MAX_STACK_SIZE];

void push(tree_pointer item) {
    stack[++top] = item;
}

tree_pointer pop() {
    if (top == -1) return NULL;
    return stack[top--];
}

/* ===== 큐 (level order용) ===== */
int front = 0, rear = 0;
tree_pointer queue[MAX_QUEUE_SIZE];

void addq(tree_pointer item) {
    queue[rear++] = item;
}

tree_pointer deleteq() {
    if (front == rear) return NULL;
    return queue[front++];
}

/* ===== 재귀 순회 ===== */
void inorder(tree_pointer ptr) {
    if (ptr) {
        inorder(ptr->left_child);
        printf("%d ", ptr->data);
        inorder(ptr->right_child);
    }
}

void preorder(tree_pointer ptr) {
    if (ptr) {
        printf("%d ", ptr->data);
        preorder(ptr->left_child);
        preorder(ptr->right_child);
    }
}

void postorder(tree_pointer ptr) {
    if (ptr) {
        postorder(ptr->left_child);
        postorder(ptr->right_child);
        printf("%d ", ptr->data);
    }
}

/* ===== 비순환 inorder ===== */
void iter_inorder(tree_pointer node) {
    top = -1;
    for (;;) {
        for (; node; node = node->left_child)
            push(node);

        node = pop();
        if (!node) break;

        printf("%d ", node->data);
        node = node->right_child;
    }
}

/* ===== 레벨 순회 ===== */
void level_order(tree_pointer ptr) {
    front = rear = 0;
    if (!ptr) return;

    addq(ptr);
    while (1) {
        ptr = deleteq();
        if (!ptr) break;

        printf("%d ", ptr->data);

        if (ptr->left_child)
            addq(ptr->left_child);
        if (ptr->right_child)
            addq(ptr->right_child);
    }
}

/* ===== 트리 복사 ===== */
tree_pointer copy(tree_pointer original) {
    tree_pointer temp;
    if (original) {
        temp = (tree_pointer)malloc(sizeof(struct node));
        temp->data = original->data;
        temp->left_child = copy(original->left_child);
        temp->right_child = copy(original->right_child);
        return temp;
    }
    return NULL;
}

/* ===== 트리 동일성 검사 ===== */
int equal(tree_pointer first, tree_pointer second) {
    return ((!first && !second) ||
        (first && second &&
            first->data == second->data &&
            equal(first->left_child, second->left_child) &&
            equal(first->right_child, second->right_child)));
}

/* ===== 노드 개수 ===== */
int get_node_count(tree_pointer ptr) {
    int count = 0;
    if (ptr != NULL)
        count = 1 + get_node_count(ptr->left_child)
        + get_node_count(ptr->right_child);
    return count;
}

/* ===== 리프 노드 개수 ===== */
int get_leaf_count(tree_pointer ptr) {
    int count = 0;
    if (ptr != NULL) {
        if (ptr->left_child == NULL && ptr->right_child == NULL)
            return 1;
        else
            count = get_leaf_count(ptr->left_child)
            + get_leaf_count(ptr->right_child);
    }
    return count;
}

int main() {
    /* ===== 노드 생성 ===== */
    tree_pointer root = (tree_pointer)malloc(sizeof(struct node));
    tree_pointer n2   = (tree_pointer)malloc(sizeof(struct node));
    tree_pointer n3   = (tree_pointer)malloc(sizeof(struct node));
    tree_pointer n4   = (tree_pointer)malloc(sizeof(struct node));
    tree_pointer n5   = (tree_pointer)malloc(sizeof(struct node));
    tree_pointer n6   = (tree_pointer)malloc(sizeof(struct node));

    /* ===== 데이터 설정 ===== */
    root->data = 1;
    n2->data = 2;
    n3->data = 3;
    n4->data = 4;
    n5->data = 5;
    n6->data = 6;

    /* ===== 링크 연결 ===== */
    root->left_child = n2;
    root->right_child = n3;

    n2->left_child = n4;
    n2->right_child = n5;

    n3->left_child = NULL;
    n3->right_child = n6;

    n4->left_child = n4->right_child = NULL;
    n5->left_child = n5->right_child = NULL;
    n6->left_child = n6->right_child = NULL;

    /* ===== 트리 순회 ===== */
    printf("Inorder traversal      : ");
    inorder(root);
    printf("\n");

    printf("Preorder traversal     : ");
    preorder(root);
    printf("\n");

    printf("Postorder traversal    : ");
    postorder(root);
    printf("\n");

    printf("Iterative inorder      : ");
    iter_inorder(root);
    printf("\n");

    printf("Level order traversal  : ");
    level_order(root);
    printf("\n");

    /* ===== 노드 / 리프 개수 ===== */
    printf("Node count : %d\n", get_node_count(root));
    printf("Leaf count : %d\n", get_leaf_count(root));

    /* ===== 트리 복사 ===== */
    tree_pointer copied = copy(root);

    printf("Copied tree inorder    : ");
    inorder(copied);
    printf("\n");

    /* ===== 트리 비교 ===== */
    if (equal(root, copied))
        printf("Two trees are equal\n");
    else
        printf("Two trees are NOT equal\n");

    return 0;
}
