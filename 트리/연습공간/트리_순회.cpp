#include<stdio.h>
#include<stdlib.h>
typedef struct TreeNode{
    int data;
    struct TreeNode* lnode;
    struct TreeNode* rnode;
}TreeNode;

TreeNode inorder(TreeNode* root){
    printf("inorder : ");
    if(root){
        inorder(root->lnode);
        printf("%d ", root->data);
        inorder(root->rnode);
    }
}

TreeNode postorder(TreeNode *root){
    printf("postorder : ");
    if(root){
        postorder(root->lnode);
        postorder(root->rnode);
        printf("%d ",root->data);
    }
}



TreeNode preorder(TreeNode * root){
    printf("preorder : ");
    if(root){
        printf("%d ", root->data);
        preorder(root->lnode);
        preorder(root->rnode);
    }
}


int main(){
    TreeNode n1 = {1, NULL, NULL};
TreeNode n2 = {4, &n1, NULL};
TreeNode n3 = {16, NULL, NULL};
TreeNode n4 = {25, NULL, NULL};
TreeNode n5 = {20, &n3, &n4};
TreeNode n6 = {15, &n2, &n5};
TreeNode *root = &n6;
    inorder(root);
    printf("\n");
}