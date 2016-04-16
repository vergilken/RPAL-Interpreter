#include "NaryTree.h"

NaryTree::NaryTree(TreeNode* root)
{
    _root=root;
}

NaryTree::~NaryTree()
{
    //
}

//beautiful printing for AST
void NaryTree::PreOrder(int level)
{
   _root->PreOrder(level);
}

TreeNode* NaryTree::getRoot()
{
    return _root;
}


