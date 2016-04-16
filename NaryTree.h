#ifndef NARYTREE_H
#define NARYTREE_H

#include "TreeNode.h"

class NaryTree
{
    public:
        NaryTree(TreeNode* root);
        virtual ~NaryTree();
        void PreOrder(int level);
        TreeNode* getRoot();

    private:
        TreeNode*_root;

};
#endif // NARYTREE_H
