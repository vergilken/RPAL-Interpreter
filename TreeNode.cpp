#include "TreeNode.h"

TreeNode :: TreeNode ( string value, int type )
{
    _value = value;
    _type = type;
    LeftChild = nullptr;
    RightSibling = nullptr;
}

TreeNode::TreeNode ( int type )
{
    _type = type;
    LeftChild = nullptr;
    RightSibling = nullptr;
}

TreeNode :: ~TreeNode ( )
{
    //dtor
}

string TreeNode :: getValue ( )
{
    return _value;
}

int TreeNode :: getType ( )
{
    return _type;
}

void TreeNode::addChild ( TreeNode * child )
{
  if ( child != nullptr ){
    if ( LeftChild == nullptr ) LeftChild = child;
    else if ( LeftChild != nullptr )
                    LeftChild -> addSibling( child );
}
}

void TreeNode::addSibling(TreeNode* sibling)
{
   if ( sibling != nullptr )
   {
     if ( RightSibling == nullptr ) RightSibling = sibling;
     else {
           TreeNode * tempNode = RightSibling;
           while ( tempNode -> RightSibling != nullptr )
            tempNode = tempNode -> RightSibling;
           tempNode -> RightSibling = sibling;
     }
   }
}

TreeNode * TreeNode :: getLeftChild ( )
{
    return LeftChild;
}

TreeNode * TreeNode :: getRightSibling ( )
{
    return RightSibling;
}

int TreeNode :: Num_Child ( )
{
    if ( LeftChild == nullptr) return 0;
    TreeNode * index = LeftChild;
    int num = 0;
    while (  index != nullptr )
    {
        index = index -> RightSibling;
        ++ num;
    }
    return num;
}

string TreeNode::type_to_string ( )
{
    if ( _type>=LAMBDA && _type<=LET ) return Type[_type];
    switch ( _type )
    {
        case IDENTIFIER : return "<ID:" + _value + ">";
        case STRING : return "<STR:'" + _value + "'>";
        case INTEGER: return "<INT:" + _value + ">";
        case Y : return "<Y*>";
        default : return "<Unknown Type>";
    }
}

void TreeNode::PreOrder ( int num ){
   for( int i = 0; i<num; ++i ) cout<<".";
   cout << type_to_string() << endl;
   if ( LeftChild != nullptr ) LeftChild -> PreOrder(num+1);
   if( RightSibling != nullptr )RightSibling -> PreOrder( num );
}

TreeNode* TreeNode :: zigzag ( TreeNode * temp )
{
   if ( temp == nullptr || temp ->RightSibling == nullptr ) return temp;
   else if ( temp -> RightSibling ->RightSibling != nullptr  )
   {
       TreeNode * result = new TreeNode ( LAMBDA );
       result -> LeftChild = temp;
       result ->  LeftChild -> RightSibling = zigzag( temp -> RightSibling );
       return result;
   }
      else
   {
       TreeNode * result = new TreeNode ( LAMBDA );
       result -> LeftChild = temp;
       result ->  LeftChild -> RightSibling = temp -> RightSibling;
       return result;
   }
}

void TreeNode :: AST_TO_ST ( )
{
    if ( LeftChild != nullptr ) LeftChild -> AST_TO_ST ( );
    if ( RightSibling != nullptr ) RightSibling -> AST_TO_ST ( );
    Standardize( );
}

void TreeNode :: Standardize ( )
{
    TreeNode * temp;
    if ( _type == LET )
    {
        _type = GAMMA;
        LeftChild -> _type = LAMBDA;
        temp = LeftChild -> RightSibling;
        LeftChild -> RightSibling = LeftChild -> LeftChild -> RightSibling;
        LeftChild -> LeftChild -> RightSibling  = temp;
    }

   else if  ( _type == WHERE )
   {
      _type = GAMMA;
       TreeNode * P = LeftChild;
       TreeNode * X = LeftChild ->RightSibling -> LeftChild;
       TreeNode * E = LeftChild -> RightSibling -> LeftChild -> RightSibling;
       LeftChild -> RightSibling = nullptr;
       X -> RightSibling = nullptr;
       LeftChild = new TreeNode ( LAMBDA );
       LeftChild -> RightSibling = E;
       LeftChild -> LeftChild = X;
       X -> RightSibling = P;
   }

   else if ( _type == WITHIN )
   {
       _type = BINDING;
        TreeNode * x1 = LeftChild -> LeftChild;
        TreeNode * e1 = LeftChild -> LeftChild -> RightSibling;
        TreeNode * x2 = LeftChild -> RightSibling ->LeftChild;
        TreeNode * e2 = LeftChild -> RightSibling -> LeftChild -> RightSibling;
        LeftChild = x2;
        LeftChild -> RightSibling = new TreeNode ( GAMMA );
        LeftChild -> RightSibling -> LeftChild = new TreeNode ( LAMBDA );
        LeftChild -> RightSibling -> LeftChild -> RightSibling = e1;
        LeftChild -> RightSibling -> LeftChild -> LeftChild = x1;
        LeftChild -> RightSibling -> LeftChild -> LeftChild -> RightSibling = e2;
    }

     else if  ( _type == REC )
    {
     _type = BINDING;
      TreeNode * x = LeftChild -> LeftChild;
      TreeNode * e = LeftChild -> LeftChild ->RightSibling;
       LeftChild = x;
       x -> RightSibling = nullptr;
       LeftChild ->  RightSibling = new TreeNode ( GAMMA );
       LeftChild -> RightSibling -> LeftChild = new TreeNode ( Y );
       LeftChild -> RightSibling -> LeftChild -> RightSibling = new TreeNode ( LAMBDA );
       LeftChild -> RightSibling -> LeftChild ->RightSibling -> LeftChild = new TreeNode ( x -> getValue(), x -> getType());
       LeftChild -> RightSibling -> LeftChild-> RightSibling -> LeftChild ->RightSibling = e;
   }

else if  ( _type == FCN_FORM )
{
    _type = BINDING;
    temp = LeftChild -> RightSibling;
    LeftChild ->RightSibling = zigzag( temp );
}

else if ( _type == LAMBDA )
{
  temp = zigzag( LeftChild );
  LeftChild = temp ->LeftChild;
}

  else if ( _type == AND)
  {
    _type = BINDING;
    temp = LeftChild;
    LeftChild = nullptr;
    LeftChild = new TreeNode ( COMMA );
    LeftChild -> RightSibling = new TreeNode ( TAU );
    while ( temp != nullptr )
    {
        if ( LeftChild == nullptr || LeftChild -> RightSibling == nullptr) break;
        LeftChild -> RightSibling -> addChild( temp ->  LeftChild -> RightSibling );
        LeftChild -> addChild( temp -> LeftChild );
        temp ->LeftChild -> RightSibling = nullptr;
        temp = temp ->RightSibling;
    }
  }

  else if ( _type == AT )
  {
      _type = GAMMA;
      TreeNode * e1 = LeftChild;
      TreeNode * n = LeftChild -> RightSibling;
      TreeNode * e2 = LeftChild ->RightSibling -> RightSibling;
      LeftChild  = new TreeNode ( GAMMA );
      LeftChild ->  LeftChild = n;
      LeftChild -> RightSibling = e2;
      n -> RightSibling = nullptr;
      LeftChild -> LeftChild -> RightSibling = e1;
      e1 -> RightSibling = nullptr;
  }
}

