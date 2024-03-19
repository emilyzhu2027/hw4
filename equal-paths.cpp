#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool isLeaf(Node* node) {
    return node != nullptr && node->left == nullptr && node->right == nullptr;
}

int height(Node *root) {
    if (root == NULL){
      return 0;
    }
    return 1 + std::max(height(root->left), height(root->right));
}

int maxHeight(Node * node)
{
    if (node == NULL){
        return 0;
    }
    
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    
    return (1 + max(leftHeight, rightHeight));
}

bool equalPaths(Node * root)
{
  if (root == NULL){
    return true;
  }

  if (isLeaf(root)){
    return true;
  }

  int leftHeight = maxHeight(root->left);
  int rightHeight = maxHeight(root->right);

  if ((leftHeight == 0 && rightHeight == 1) || (leftHeight == 1 && rightHeight == 0)){
    return true;
  }

  return leftHeight == rightHeight;
}

