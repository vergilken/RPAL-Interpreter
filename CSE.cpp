#include "CSE.h"

ELEMENT * TreeNode_To_Element ( TreeNode * node )
{
    switch ( node -> getType ( ) )
    {
    case IDENTIFIER :
        {
        ELEMENT * temp1 = new IdentifierElement ( node -> getValue ( ) );
        return temp1;
        }
    case STRING :
       {
        ELEMENT * temp2 = new StringElement ( node -> getValue ( ) );
        return temp2;
       }
    case INTEGER :
        {
        ELEMENT * temp3 = new IntegerElement ( node -> getValue ( ) );
        return temp3;
        }
    default :
        {
        ELEMENT * temp4 = new OperationElement ( node -> getType ( ) );
        return temp4;
        }
    }
}

bool isInnerFunction ( const string & str )
{
    for ( auto temp : InnerFunc )
       if(temp.compare ( str)) return false;
    return true;
}

CSE::CSE( Parser * parser )
{
   root = (*( ( parser -> astTree ) .begin ( ) ) )-> getRoot();
   EnvironmentElement * environment = new EnvironmentElement ( nullptr, 0 );
   Control.push_back( environment );
   Stack.push_back ( environment );
   CurrentEnvironment = environment;
   m = 0;
}

int  CSE :: count_environment_num ( )
{
    return m;
}

EnvironmentElement * CSE :: getCurrentEnvironment ( )
{
    return CurrentEnvironment;
}

void CSE :: LeftRecusive ( TreeNode * index )
{
  if ( index == nullptr ) return;
  switch ( index -> getType ( ) )
  {
  case LAMBDA :
    {
      LambdaElement *  temp = new LambdaElement ( index, CurrentEnvironment );
      Control.push_back ( temp );
      LeftRecusive( index ->RightSibling );
      return;
    }
  case TERNERY :
    {
      DeltaElement * Then = new DeltaElement ( index -> LeftChild -> RightSibling );
      DeltaElement * Else = new DeltaElement ( index -> LeftChild -> RightSibling -> RightSibling );
      Control.push_back ( Then );
      Control.push_back ( Else );
      OperationElement * belta = new OperationElement ( BELTA );
      Control.push_back ( belta );
      LeftRecusive( index -> LeftChild );
      LeftRecusive( index -> RightSibling );
      return;
    }
  case TAU :
    {
      int num = index ->Num_Child ( );
      TauElement * Tau = new TauElement ( num );
      Control.push_back ( Tau );
      LeftRecusive( index -> LeftChild );
      LeftRecusive( index -> RightSibling );
      return;
    }
  }
     if ( index -> LeftChild != nullptr  && index -> RightSibling == nullptr   )
       {
        Control.push_back ( TreeNode_To_Element(index) );
        LeftRecusive( index -> LeftChild );
       }
    else if ( index -> LeftChild == nullptr  && index -> RightSibling != nullptr  )
    {
     Control.push_back ( TreeNode_To_Element(index) );
     LeftRecusive( index -> RightSibling );
    }
    else if ( index -> LeftChild == nullptr && index -> RightSibling == nullptr  )
    {
       Control.push_back ( TreeNode_To_Element(index) );
    }
    else if ( index -> LeftChild != nullptr  && index -> RightSibling != nullptr )
    {
        Control.push_back ( TreeNode_To_Element(index) );
        LeftRecusive( index ->  LeftChild );
        LeftRecusive( index -> RightSibling );
    }
    return;
}

void CSE :: Flatern ( TreeNode * index )
{
    LeftRecusive( index );
}

void CSE :: print ( )
{
    auto iter_control = Control.begin ( );
    auto iter_stack = Stack.end( ) - 1;
    cout<< "** CONTROL ** :" << endl;
    for ( ; iter_control != Control.end ( ); ++iter_control )
    {
        (*iter_control) -> print ( );
    }
    cout<<endl;
   cout<<"** STACK **:"<<endl;
    for ( ; iter_stack != Stack.begin ( ) -1; --iter_stack )
    {
        (*iter_stack) -> print ( );
    }
}

void CSE :: Execute ( )
{
    Flatern( root );
    print ( );
    cout<<endl;
    cout<<"*****************"<<endl;

   int  count_num = 1;
    while ( !Control.empty ( ) )
    {
      CSE_Machine ( );
      ++count_num;
      print ( );
    cout<<endl;
    cout<<"*****************"<<endl;
    }
    cout<< count_num <<" Elements have been executed."<<endl;
}

#if 1
void CSE :: CSE_Machine ( )
{
        switch ( Control.back ( ) -> getType ( ) )
      {
       case ENVIRONMENT :
        {
            int num = ( dynamic_cast<EnvironmentElement*> (Control.back ( ) ) ) ->getID ( );
            for ( auto it = Stack.begin ( ); it != Stack.end ( );  )
            {
                 if ( (*it) -> getType ( ) == ENVIRONMENT && ( dynamic_cast<EnvironmentElement*> ( *it ) ) ->getID ( ) == num  )
                {
                    CurrentEnvironment = ( dynamic_cast<EnvironmentElement*> ( *it ) )  -> getFather ( ) ;
                    Control.pop_back ( );
                    Stack.erase ( it );
                    return;
                }
                else
                    ++it;
            }
        }

//  Rule 2 ( stacking a Lambda Expression )
      case LAMBDA :
      {
          ( dynamic_cast<LambdaElement*> (Control.back ( )) ) -> setEnvironment ( CurrentEnvironment );
          Stack.push_back ( Control.back ( ) );
          Control.pop_back ( );
          return;
      }

      case GAMMA :
        {
            Control.pop_back ( );

           switch ( Stack.back ( ) -> getType ( )  )
           {
           case LAMBDA:
            {
             ++m;
            TreeNode * index =  ( dynamic_cast<LambdaElement*> (Stack.back ( ) ) ) -> getParameter ( );
            if ( index == nullptr ) return;
            ELEMENT * environment = new EnvironmentElement ( (dynamic_cast<LambdaElement*> (Stack.back ( ) ) ) -> getEnvironment ( ) , m );
            Stack.pop_back ( );
            CurrentEnvironment = (dynamic_cast<EnvironmentElement*> (environment) );
 // Rule 4 Applying a Lambda-closure
            if ( index -> getType ( ) != COMMA )
            {
             (dynamic_cast<EnvironmentElement*> (environment) ) -> InsertParameter ( index -> getValue ( ), Stack.back ( ) );
              Stack.pop_back ( );
            }

 // Rule 11 n-ary function
            else if ( index -> getType ( ) == COMMA )
            {
              TreeNode * parameter = index -> getLeftChild ( );
              if (Stack.back ( ) -> getType( ) != TUPLE ) return;
              vector<ELEMENT *> tempTuple = (dynamic_cast<TupleElement *> (Stack.back ( ) ) )  -> getTuple ( );
              auto it = tempTuple.begin ( );
              while ( parameter != nullptr && it != tempTuple.end ( ) )
              {
               (dynamic_cast<EnvironmentElement*> (environment) ) -> InsertParameter ( parameter -> getValue ( ), *it );
               parameter = parameter -> getRightSibling ( );
               ++it;
              }
              //(dynamic_cast<EnvironmentElement*> (environment) )->printParameters ( );
               Stack.pop_back ( );

            }

            Stack.push_back ( environment );
            Control.push_back ( environment );
            cout<<endl;

            if ( index -> getRightSibling ( ) != nullptr)
            Flatern ( index -> getRightSibling ( ) );
            return;
            }

    // Rule 10 tuple selection
           case TUPLE :
            {
             vector <ELEMENT * > tempTuple =   ( dynamic_cast<TupleElement*> (Stack.back ( ) ) ) -> getTuple ( );
             Stack.pop_back ( );
             if ( Stack.back ( ) -> getType ( ) != INTEGER ) return;
             size_t m = stoi ( Stack.back ( ) -> getValue ( ) );
             Stack.pop_back ( );
             Stack.push_back ( tempTuple.at ( m-1 ) );
            return;
            }

            default : return;
           }
        }

      case IDENTIFIER:
          {
             bool m = isInnerFunction( Control.back ( ) -> getValue ( ) );
             if( m )
             {
                cout<<"innerfunction"<<endl;
                return;
             }

// Rule 1 Stacking a name
            else
            {
            ELEMENT * temp = CurrentEnvironment->  getParameter ( Control.back ( ) -> getValue ( ) );
            Stack.push_back ( temp );
            Control.pop_back ( );
            return;
            }
          }

// Rule 9 ( tuple formation )
      case TAU:
        {
            if ( ( dynamic_cast<TauElement * > (Control.back ( ) ) ) -> get_Num_Tuple ( ) == 0 ) return;
            int temp = ( dynamic_cast<TauElement * > (Control.back ( ) ) ) -> get_Num_Tuple ( ) ;
            ELEMENT * temp_tuple = new TupleElement ( temp );
            Control.pop_back ( );
             for ( int i = 0; i < temp ; ++i )
             {
               ( dynamic_cast<TupleElement * > (temp_tuple) )  -> InsertElement ( Stack.back ( ) );
                Stack.pop_back ( );
             }
            Stack.push_back ( temp_tuple );
            return;
        }

      case INTEGER :
          {
            Stack.push_back ( Control.back ( ) );
            Control.pop_back ( );
            return;
          }

      case STRING :
          {
            Stack.push_back ( Control.back ( ) );
            Control.pop_back ( );
            return;
          }
// Rule 6 and 7: Unary and Binary Operators
      case PLUS:
        {
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string ( num1 + num2  ) );
            cout<<Stack.back ( ) -> getValue ( )<<endl;        // 判断下一个是不是3       TAT
            Stack.push_back ( temp );
            return;
        }

      case MULTIPLY:
        {
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string ( num1 * num2  ) );
            Stack.push_back ( temp );
            return;
        }

      case MINUS:
        {
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string ( num1 - num2  ) );
            Stack.push_back ( temp );
            return;
        }

      case DIVIDE:
        {
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string ( num1 / num2  ) );
            Stack.push_back ( temp );
            return;
        }

      case POWER:
        {
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string ( pow ( num1, num2 )  ) );
            Stack.push_back ( temp );
            return;
        }





















          //end of operator
      default :
         {
            return;
        }

      }
    }
    #endif





