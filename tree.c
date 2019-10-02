#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"

TreeNode* initializeTreeNode(int walletID, int value){
    TreeNode* node = malloc(sizeof(TreeNode));

    node->walletID = walletID;
    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

TreeRoot* initializeRoot(int bitcoinID, int walletID, int startValue){
    TreeRoot* root = malloc(sizeof(TreeRoot));

    root->bitcoinID = bitcoinID;
    root->treenode = initializeTreeNode(walletID, startValue);

    return root;
}

TreeNode* findTreeNode(TreeNode* node, int walletID){
    TreeNode* tmp = node;
    TreeNode* res1;
    TreeNode* res2;

    if(tmp->walletID==walletID && tmp->left==NULL && tmp->right==NULL){
        return tmp;
    }
    else{
        if(tmp->left!=NULL && tmp->right!=NULL){
            res1 = findTreeNode(tmp->left, walletID);
            res2 = findTreeNode(tmp->right, walletID);

            if(res1!=NULL)
                return res1;
            else
                return res2;
        }
    }
}

TreeNode* BitcoinTreeAppend(TreeNode* node, int SwalletID, int RwalletID, int value){
    TreeNode* tmp = node;
    TreeNode* Snode;
    if(tmp->left==NULL && tmp->right==NULL){
        tmp->left = initializeTreeNode(RwalletID, value);
        tmp->right = initializeTreeNode(SwalletID, tmp->value - value);
        if(tmp->value - value < 0){
            printf("error in insert tree node\n");
        }
        return node;
    }
    else{
        Snode = findTreeNode(node, SwalletID);
        Snode->left = initializeTreeNode(RwalletID, value);
        Snode->right = initializeTreeNode(SwalletID, Snode->value - value);
        if(Snode->value - value < 0){
            printf("error in insert tree node1 %d-%d\n", Snode->value ,value);
        }
        return node;
    }
}

void printTreein(TreeNode *root, int space)
{
    if (root == NULL)
        return;

    space += 10;

    printTreein(root->right, space);

    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%d, ($%d)\n", root->walletID, root->value);

    printTreein(root->left, space);
}

void printTree(TreeNode *root)
{
    printTreein(root, 0);
}


void FreeTree(TreeNode *node){
    if (node != NULL) {
        FreeTree(node->right);
        FreeTree(node->left);

        free(node);
    }
}
