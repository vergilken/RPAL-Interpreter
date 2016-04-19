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

EnvironmentElement * CSE :: getStackEnvironment ( )
{
    auto it = Stack.end ( );
    while ( (*it) -> getType ( ) != ENVIRONMENT ) --it;
    return ( dynamic_cast<EnvironmentElement * > ( * it ) );
}

int CSE :: judgeOrOperator( int _type1,  int _type2 )
{
    if ( _type1 == FALSE && _type2 == FALSE ) return FALSE;
    else if  ( _type1== FALSE && _type2 == TRUE ) return TRUE;
    else if ( _type1 == TRUE && _type2 == FALSE ) return TRUE;
    else if  ( _type1 == TRUE &&_type2 == TRUE  ) return TRUE;
    else return DUMMY;
}

 int CSE :: judgeAndOperator ( int _type1, int _type2 )
 {
    if ( _type1 == FALSE && _type2 == FALSE ) return FALSE;
    else if  ( _type1== FALSE && _type2 == TRUE ) return FALSE;
    else if ( _type1 == TRUE && _type2 == FALSE ) return FALSE;
    else if  ( _type1 == TRUE &&_type2 == TRUE  ) return TRUE;
     else return DUMMY;
 }

 int CSE :: judgeValue ( ELEMENT * var1, ELEMENT * var2  )
 {
    if ( var1 -> getType ( ) != var2 -> getType ( )  )  return COMPAREERROE;
    switch ( var1 -> getType ( ) )
    {
    case INTEGER :
        {
            if ( ( var1 -> getValue ( ) ).compare ( var2 -> getValue ( ) ) == 0 ) return TRUE;
            else return FALSE;
        }
    case STRING:
        {
            if ( ( var1 -> getValue ( ) ).compare ( var2 -> getValue ( ) ) == 0 ) return TRUE;
            else return FALSE;
        }
    case TUPLE:
        {
            long m = ( ( dynamic_cast<TupleElement * > ( var1 ) ) ->  getTuple ( ) ).size ( );
            long n = ( ( dynamic_cast<TupleElement * > ( var2 ) ) ->  getTuple ( ) ).size ( );
            if ( m != n ) return FALSE;

            auto ivec1 =   ( dynamic_cast<TupleElement * > ( var1 ) ) ->  getTuple ( ) ;
            auto ivec2 =    ( dynamic_cast<TupleElement * > ( var2 ) ) ->  getTuple ( ) ;
            auto iter1 = ivec1.begin ( );
            auto iter2 = ivec2.begin ( );

            while ( iter1 != ivec1.end ( ) )
            {
                if ( judgeValue ( *iter1, *iter2  ) == TRUE )
               {
                   ++ iter1; ++iter2;
               }
               else return FALSE;
            }
            return TRUE;
        }
        default : return FALSE;
    }
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

void CSE :: LeftRecusive ( TreeNode * index )
{
  if ( index == nullptr ) return;
  switch ( index -> getType ( ) )
  {
  case LAMBDA :
    {
      ELEMENT *  temp = new LambdaElement ( index, CurrentEnvironment );
      Control.push_back ( temp );
      LeftRecusive( index ->RightSibling );
      return;
    }
  case TERNERY :
    {
      ELEMENT * Then = new DeltaElement ( index -> LeftChild -> RightSibling, THEN );
      ELEMENT * Else = new DeltaElement ( index -> LeftChild -> RightSibling -> RightSibling, ELSE );
      Control.push_back ( Then );
      Control.push_back ( Else );
      ELEMENT* beta = new OperationElement ( BETA );
      Control.push_back ( beta );
      TreeNode * temp = index -> LeftChild -> RightSibling ;
      index -> LeftChild -> RightSibling = nullptr;
      LeftRecusive( index -> LeftChild );
      index -> LeftChild -> RightSibling = temp;
      LeftRecusive( index -> RightSibling );
      return;
    }
  case TAU :
    {
      int num = index ->Num_Child ( );
      ELEMENT * Tau = new TauElement ( num );
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
    cout <<endl;
    cout<<"Current Environment is " << count_environment_num ( ) <<endl;
    cout<< "** CONTROL ** :" << endl;
    for ( ; iter_control != Control.end ( ); ++iter_control )
    {
        (*iter_control) -> print ( );
        cout<<endl;
    }
    cout<<endl;
   cout<<"** STACK **:"<<endl;
    for ( ; iter_stack != Stack.begin ( ) -1; --iter_stack )
    {
        (*iter_stack) -> print ( );
        cout<<endl;
    }
}

void CSE :: Execute ( )
{
    Flatern( root );
    #if 0
    print ( );
    cout<<endl;
    cout<<"*****************"<<endl;
    #endif

    while ( !Control.empty ( ) )
    {
      CSE_Machine ( );
    #if 0
    print ( );
    cout<<endl;
    cout<<"*****************"<<endl;
    #endif
    }
    if ( Control.empty ( ) )
        cout <<" ";
}

#if 1
void CSE :: CSE_Machine ( )
{
        switch ( Control.back ( ) -> getType ( ) )
      {
       case ENVIRONMENT :
        {
            int num = ( dynamic_cast<EnvironmentElement*> (Control.back ( ) ) ) ->getID ( );
            if( num == 0 )
            {
                 for ( auto it = Stack.begin ( ); it != Stack.end ( );  )
            {
                 if ( (*it) -> getType ( ) == ENVIRONMENT && ( dynamic_cast<EnvironmentElement*> ( *it ) ) ->getID ( ) == num  )
                {
                    Control.pop_back ( );
                    Stack.erase ( it );
                    return;
                }
                else
                    ++it;
            }
            }
            for ( auto it = Stack.begin ( ); it != Stack.end ( );  )
            {
                 if ( (*it) -> getType ( ) == ENVIRONMENT && ( dynamic_cast<EnvironmentElement*> ( *it ) ) ->getID ( ) == num  )
                {
                    Control.pop_back ( );
                    Stack.erase ( it );
                    CurrentEnvironment = getStackEnvironment ( );
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
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );

           switch ( Stack.back ( ) -> getType ( )  )
           {
           case LAMBDA:
            {
             //cout<<"The lambda environment is  " <<m<<endl;
            TreeNode * index =  ( dynamic_cast<LambdaElement*> (Stack.back ( ) ) ) -> getParameter ( );
            if ( index == nullptr ) return;
            ELEMENT * environment = new EnvironmentElement ( (dynamic_cast<LambdaElement*> (Stack.back ( ) ) ) -> getEnvironment ( ) , ++ m );
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
             size_t m = stol ( Stack.back ( ) -> getValue ( ) );
             Stack.pop_back ( );
             Stack.push_back ( tempTuple.at ( m-1 ) );
            return;
            }

    // InnerFunction:
           case PRINT:
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                if ( Stack.back ( ) -> getType ( ) == ENVIRONMENT )
                {
                ELEMENT * temp = new OperationElement ( DUMMY );
                Stack.push_back ( temp );
                return;
                }
                else
                {
                Stack.back ( ) -> print ( );
                Stack.pop_back ( );
                ELEMENT * temp = new OperationElement ( DUMMY );
                Stack.push_back ( temp );
                return;
                }
            }

           case ITOS :
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                if ( Stack.back ( ) -> getType ( ) != INTEGER )
                {
                    cout << " Invalid ItoS use - Argument is not an Integer" <<endl;
                    exit ( 0 );
                }
                string str = Stack.back ( ) -> getValue ( );
                Stack.pop_back ( );
                ELEMENT * temp = new StringElement ( str );
                Stack.push_back ( temp );
                return;
            }

           case ISTRUTHVALUE :
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                int type = Stack.back ( ) -> getType ( );
                if ( type == TRUE || type == FALSE )
                {
                 Stack.pop_back ( );
                ELEMENT * temp = new OperationElement ( TRUE );
                Stack.push_back ( temp );
                return;
                }
                else
                {
                 Stack.pop_back ( );
                 ELEMENT * temp = new OperationElement ( FALSE );
                 Stack.push_back ( temp );
                 return;
                }
            }

           case ISSTRING :
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                if ( Stack.back ( ) -> getType ( ) == STRING )
                {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( TRUE );
                    Stack.push_back ( temp );
                    return;
                }
                else
                {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( FALSE );
                    Stack.push_back ( temp );
                    return;
                }
            }

           case ISINTEGER :
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                if ( Stack.back ( ) -> getType ( ) == INTEGER )
                {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( TRUE );
                    Stack.push_back ( temp );
                    return;
                }
                else
                {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( FALSE );
                    Stack.push_back ( temp );
                    return;
                }
            }

           case ISTUPLE :
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                if ( Stack.back ( ) -> getType ( ) == TUPLE || Stack.back ( ) -> getType ( ) == NIL )
                {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( TRUE );
                    Stack.push_back ( temp );
                    return;
                }
                else
                {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( FALSE );
                    Stack.push_back ( temp );
                    return;
                }
            }

           case ORDER :
            {
                delete Stack.back ( );
                Stack.back ( ) = nullptr;
                Stack.pop_back ( );
                switch ( Stack.back ( ) -> getType ( ) )
                {
                case TUPLE :
                {
                  ELEMENT * temp = new IntegerElement ( to_string ( ( ( dynamic_cast<TupleElement * > ( Stack.back ( ) ) ) -> getTuple ( ) ).size ( ) ) );
                  Stack.pop_back ( );
                  Stack.push_back ( temp );
                  return;
                }
                case NIL :
                {
                  ELEMENT * temp = new IntegerElement ( "0" );
                  Stack.pop_back ( );
                  Stack.push_back ( temp );
                  return;
                }
                default :
                {
                  cout << "Attempt to find the order of a non-tuple" <<endl;
                  exit ( 0 );
                }
                }
            }

                case STERN:
                {
                 delete Stack.back ( );
                 Stack.back ( ) = nullptr;
                 Stack.pop_back ( );
                 if ( Stack.back ( ) -> getType ( ) != STRING )
                 {
                     cout << "Invalid Stern use in line 1 - Argument is not a string" <<endl;
                     exit ( 0 );
                 }
                 string str = Stack.back ( ) -> getValue ( );
                 Stack.pop_back ( );
                 str.erase ( 0, 1 );
                 ELEMENT * temp = new StringElement ( str );
                 Stack.push_back ( temp );
                 return;
                }

                case STEM :
                {
                delete Stack.back ( );
                 Stack.back ( ) = nullptr;
                 Stack.pop_back ( );
                  if ( Stack.back ( ) -> getType ( ) != STRING )
                 {
                     cout << "Invalid Stem use - Argument is not a string" <<endl;
                     exit ( 0 );
                 }
                 string str =  ( ( dynamic_cast<StringElement * > ( Stack.back ( ) ) ) -> getValue ( ) );
                 Stack.pop_back ( );
                 str.erase ( 1 );
                 ELEMENT * temp = new StringElement ( str);
                 Stack.push_back ( temp );
                 return;
                }

                case CONC :
                {
                 delete Stack.back ( );
                 Stack.back ( ) = nullptr;
                 Stack.pop_back ( );
                 if ( Stack.back ( ) -> getType ( ) != STRING )
                 {
                    cout << "Non-strings used in conc call" <<endl;
                    exit ( 0 );
                 }
                 string str1 = Stack.back ( ) -> getValue ( );
                 Stack.pop_back ( );
                if ( Stack.back ( ) -> getType ( ) != STRING )
                 {
                    cout << "Non-strings used in conc call" <<endl;
                    exit ( 0 );
                 }
                 string str2 = Stack.back ( ) -> getValue ( );
                 Stack.pop_back ( );
                 str2.insert ( 0, str1 );
                 ELEMENT * temp = new StringElement ( str2 );
                 Stack.push_back ( temp );
                 return;
                }

                case Y :
                {
                  delete Stack.back ( );
                  Stack.back ( ) = nullptr;
                  Stack.pop_back ( );
                  if ( Stack.back ( ) -> getType ( ) == LAMBDA )
                  {
                      ELEMENT * temp = new RecElement ( Stack.back ( ) );
                      Stack.pop_back ( );
                      Stack.push_back ( temp );
                      return;
                  }
                  else
                  {
                    cout << "Parse error: \"Expected  Nothing left to parse\", " << "but " <<  (Stack.back ( ) )  -> getValue ( )  << "was there" <<endl;
                    exit ( 0 );
                  }
                }

               case RECELEMENT :
               {
                 ELEMENT * temp1 = new OperationElement ( GAMMA );
                 ELEMENT * temp2 = new OperationElement ( GAMMA );
                 Control.push_back ( temp1 );
                 Control.push_back ( temp2 );
                 Stack.push_back ( ( dynamic_cast<RecElement * > (Stack.back ( ) ) ) -> getLambda ( ) );
                 return;
               }
            default : return;
           }
        }

      case IDENTIFIER:
        {
         ELEMENT * temp = CurrentEnvironment->  getParameter ( Control.back ( ) -> getValue ( ) );
         if ( temp != nullptr)
         {
          Stack.push_back ( temp );
          Control.pop_back ( );
          return;
         }
         else if (temp == nullptr )
         {
            for ( int i = 0; i < 10 ; ++i )
          {
             if ( Control.back ( ) -> getValue ( ).compare ( InnFunctions [i] ) == 0 )
             {
               delete Control.back ( );
               Control.back ( ) = nullptr;
               Control.pop_back ( );
               ELEMENT * temp =  new OperationElement ( i + PRINT );
               Stack.push_back ( temp );
               return;
             }
         }
          cout<<"Undeclared Identifier <"<<Control.back ( ) -> getValue ( ) <<">" <<endl;
          exit ( 0 );
        }
    }

// Rule 9 ( tuple formation )
      case TAU:
        {
            if ( ( dynamic_cast<TauElement * > (Control.back ( ) ) ) -> get_Num_Tuple ( ) == 0 )
           {
               delete Control.back ( );
               Control.back ( ) = nullptr;
               Control.pop_back ( );
               ELEMENT * temp = new OperationElement ( NIL );
               Stack.push_back ( temp );
               return;
            }
            int temp = ( dynamic_cast<TauElement * > (Control.back ( ) ) ) -> get_Num_Tuple ( ) ;
            ELEMENT * temp_tuple = new TupleElement ( 0 );
            delete Control.back ( );
            Control.back ( ) = nullptr;
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

      case NEG:
        {
          Control.pop_back ( );
          if ( Stack.back ( ) -> getType ( ) == INTEGER )
          {
             long m = ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
             ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> setIntValue ( -m );
             return;
          }
          else
          {
              cout << "Cannot negate non-integer value" <<endl;
              exit ( 0 );
          }
        }

      case PLUS:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string (num1+ num2) );
            Stack.push_back ( temp );
            return;
        }

      case MULTIPLY:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string (num1*num2) );
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
            ELEMENT * temp = new IntegerElement ( to_string (num1- num2) );
            Stack.push_back ( temp );
            return;
        }

      case DIVIDE:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string (num1/ num2) );
            Stack.push_back ( temp );
            return;
        }

      case POWER:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER ) return ;
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
            ELEMENT * temp = new IntegerElement ( to_string ( pow ( num1, num2 ) ) );
            Stack.push_back ( temp );
            return;
        }

      case AUG :
        {
          delete Control.back ( );
          Control.back ( ) = nullptr;
          Control.pop_back ( );
          if ( Stack.back ( ) -> getType ( ) == NIL )
          {
            Stack.pop_back ( );
            ELEMENT * temp = new TupleElement ( 0 );
             ( dynamic_cast<TupleElement * >  ( temp ) ) -> InsertElement ( Stack.back ( ) );
             Stack.pop_back ( );
             Stack.push_back ( temp );
             return;
          }
         else if  ( Stack.back( ) -> getType ( ) == TUPLE )
        {
            auto it = Stack.end ( ) -2;
           ( dynamic_cast<TupleElement * > (Stack.back ( ) ) ) -> InsertElement ( * it );
           Stack.erase ( it );
           return;
        }
        else
        {
            cerr << "aug operation error."<<endl;
            return;
        }
    }

      case OR:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            int type1 = Stack.back ( ) -> getType ( );
            Stack.pop_back ( );
            int type2 = Stack.back ( ) -> getType ( );
            Stack.pop_back ( );
            int temp =  judgeOrOperator( type1, type2 );
            if ( temp == FALSE )
            {
                ELEMENT * temp = new OperationElement ( FALSE );
                Stack.push_back ( temp );
            }
            else if ( temp == TRUE )
            {
                ELEMENT * temp = new OperationElement ( TRUE );
                Stack.push_back ( temp );
            }
            else if ( temp == DUMMY )
            {
                cout << "Invalid value used in logical expression \'or\'" <<endl;
                exit ( 0 );
            }
            return;
        }

      case AND_LOGICAL:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            int type1 = Stack.back ( ) -> getType ( );
            Stack.pop_back ( );
            int type2 = Stack.back ( ) -> getType ( );
            Stack.pop_back ( );
            int temp =  judgeAndOperator( type1, type2 );
            if ( type1 == ENVIRONMENT && type2 == ENVIRONMENT ) return;
             if ( temp == FALSE )
            {
                ELEMENT * temp = new OperationElement ( FALSE );
                Stack.push_back ( temp );
            }
            else if ( temp == TRUE )
            {
                ELEMENT * temp = new OperationElement ( TRUE );
                Stack.push_back ( temp );
            }
            else if ( temp == DUMMY )
            {
                cout << "Invalid value used in logical expression \'&\'" <<endl;
                exit ( 0 );
            }
            return;
        }

      case GR :
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'gr\'"<<endl;
                exit ( 0 );
            }
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
              if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'gr\'"<<endl;
                exit ( 0 );
            }
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back (  );
            if ( num1 > num2 )
            {
               ELEMENT * temp = new OperationElement ( TRUE );
               Stack.push_back ( temp );
            }
            else
            {
                ELEMENT * temp = new OperationElement ( FALSE );
                Stack.push_back ( temp );
            }
            return;
        }

        case GE :
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'ge\'"<<endl;
                exit ( 0 );
            }
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
              if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'ge\'"<<endl;
                exit ( 0 );
            }
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back (  );
            if ( num1 >= num2 )
            {
               ELEMENT * temp = new OperationElement ( TRUE );
               Stack.push_back ( temp );
            }
            else
            {
                ELEMENT * temp = new OperationElement ( FALSE );
                Stack.push_back ( temp );
            }
            return;
        }

        case LS :
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'ls\'"<<endl;
                exit ( 0 );
            }
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
              if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'ls\'"<<endl;
                exit ( 0 );
            }
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back (  );
            if ( num1 < num2 )
            {
               ELEMENT * temp = new OperationElement ( TRUE );
               Stack.push_back ( temp );
            }
            else
            {
                ELEMENT * temp = new OperationElement ( FALSE );
                Stack.push_back ( temp );
            }
            return;
        }

        case LE :
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            long num1 = 0, num2= 0;
            if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'le\'"<<endl;
                exit ( 0 );
            }
            num1 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back ( );
              if ( Stack.back ( ) -> getType ( ) !=  INTEGER )
            {
                cerr << "Illegal Operands for \'le\'"<<endl;
                exit ( 0 );
            }
            num2 =  ( dynamic_cast<IntegerElement * > (Stack.back ( ) ) ) -> getIntValue ( );
            Stack.pop_back (  );
            if ( num1 <= num2 )
            {
               ELEMENT * temp = new OperationElement ( TRUE );
               Stack.push_back ( temp );
            }
            else
            {
                ELEMENT * temp = new OperationElement ( FALSE );
                Stack.push_back ( temp );
            }
            return;
        }

        case EQ:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            auto it = Stack.end( ) -1;
            if (  judgeValue ( *it, *( it-1 )  )  == TRUE )
            {
               Stack.pop_back ( );
               Stack.pop_back ( );
               ELEMENT * temp = new OperationElement ( TRUE );
               Stack.push_back ( temp );
               return;
            }
            else if ( judgeValue ( *it, *( it-1 )  )  == FALSE )
            {
               Stack.pop_back ( );
               Stack.pop_back ( );
               ELEMENT * temp = new OperationElement ( FALSE );
               Stack.push_back ( temp );
               return;
            }
            else
            {
                cout << "Illegal Operands for \'eq\'" <<endl;
                exit ( 0 );
            }
        }

          case NE:
        {
            delete Control.back ( );
            Control.back ( ) = nullptr;
            Control.pop_back ( );
            auto it = Stack.end( ) -1;
            if (  judgeValue ( *it, *( it-1 )  )  == TRUE )
            {
               Stack.pop_back ( );
               Stack.pop_back ( );
               ELEMENT * temp = new OperationElement ( FALSE );
               Stack.push_back ( temp );
               return;
            }
            else if ( judgeValue ( *it, *( it-1 )  )  == FALSE )
            {
               Stack.pop_back ( );
               Stack.pop_back ( );
               ELEMENT * temp = new OperationElement ( TRUE );
               Stack.push_back ( temp );
               return;
            }
            else
            {
                cout << "Illegal Operands for \'ne\'" <<endl;
                exit ( 0 );
            }
        }

          case NOT :
            {
                delete Control.back ( );
                Control.back ( ) = nullptr;
                Control.pop_back ( );
                switch ( Stack.back ( ) -> getType ( ) )
                {
                case TRUE :
                    {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( FALSE );
                    Stack.push_back ( temp );
                    return;
                    }
                case FALSE :
                    {
                    Stack.pop_back ( );
                    ELEMENT * temp = new OperationElement ( TRUE );
                    Stack.push_back ( temp );
                    return;
                    }
                default :
                {
                  cout << "Non-boolean for \'not\' application"  << endl;
                  exit ( 0 );
                }
                }
            }

          case BETA :
           {
             delete Control.back ( );
             Control.back ( ) = nullptr;
             Control.pop_back ( );
             int type = Stack.back ( ) -> getType ( );
             delete Stack.back ( );
             Stack.back ( ) = nullptr;
             Stack.pop_back ( );

             if ( type == TRUE )
             {
                 Control.pop_back ( );
                TreeNode * temp =  ( dynamic_cast<DeltaElement * > ( Control.back ( ) ) ) -> getLocationOpt ( );
                TreeNode * temp1 = temp -> getRightSibling ( );
                temp -> RightSibling = nullptr;
                Control.pop_back ( );

                Flatern( temp );
                temp -> RightSibling = temp1;
                return;
             }

             else if ( type == FALSE )
             {
                TreeNode * temp =  ( dynamic_cast<DeltaElement * > ( Control.back ( ) ) ) -> getLocationOpt ( );
                TreeNode * temp1 = temp ->getRightSibling( );
                temp -> RightSibling = nullptr ;
                Control.pop_back ( );
                Control.pop_back ( );

                Flatern( temp );
                temp -> RightSibling = temp1;
                return;
             }
             else
             {
                cout << "Non-boolean result used in conditional" << endl;
                exit ( 0 );
             }
            }

          //end of operator
      default :
         {
            Stack.push_back ( Control.back ( ) );
            Control.pop_back ( );
            return;
        }
      }
    }
    #endif





