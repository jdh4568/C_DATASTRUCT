#include<stdio.h>
#include<stdlib.h>

typedef struct TreeNode{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

// 링크법으로 트리 생성
TreeNode n1 = {1, NULL, NULL};
TreeNode n2 = {4, &n1, NULL};
TreeNode n3 = {16, NULL, NULL};
TreeNode n4 = {25, NULL, NULL};
TreeNode n5 = {20, &n3, &n4};
TreeNode n6 = {15, &n2, &n5};
TreeNode *root = &n6;

// 전위순회
void preorder(TreeNode *root){
    if(root){
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}
void inorder(TreeNode *root){
    if(root){
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}
void postorder(TreeNode *root){
    if(root){
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int main(void){
    printf("전위 순회 : ");
    preorder(root);
    printf("\n");

    printf("중위 순회 : ");
    inorder(root);
    printf("\n");

    printf("전위 순회 : ");
    postorder(root);
    printf("\n");

    return 0;
}