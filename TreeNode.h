#ifndef TREENODE_H
#define TREENODE_H

#include "Statistics.h"
#include "Tokens.h"

class TreeNode
{
    friend class NaryTree;
    friend class CSE;
    friend class ELEMENT;
    public:
        TreeNode(string value, int type);
        TreeNode(int type);
        TreeNode()=default;
        virtual ~TreeNode();
        void addChild(TreeNode* child);
        void addSibling(TreeNode* sibling);
        TreeNode * getLeftChild ( );
        TreeNode * getRightSibling ( );
        string getValue ();
        int getType ();
        int Num_Child ( );
        void PreOrder(int level);
        void AST_TO_ST ();
    private:
        TreeNode* LeftChild;
        TreeNode* RightSibling;
        string _value;
        int _type;
        string type_to_string( );
        TreeNode * zigzag ( TreeNode * temp );
        void Standardize( );
};
#endif // TREENODE_H
