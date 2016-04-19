#include "ELEMENT.h"

ELEMENT :: ELEMENT (  )
{
    type = DUMMY;
    value = "dummy";
}

int ELEMENT :: getType ( ) const
{
    return type;
}

string ELEMENT :: getValue ( ) const
{
    return value;
}

void ELEMENT :: print ( ) const
{
    cout << "element" ;
}

void EnvironmentElement :: print ( ) const
{
    cout << "environment : " << id ;
}

void EnvironmentElement :: InsertParameter ( string  str, ELEMENT * element )
{
    parameter.insert (pair<string , ELEMENT *> ( str, element ));
}

ELEMENT *  EnvironmentElement :: getParameter ( string str)
{
   auto l_it = parameter.begin ( );
   l_it=parameter.find(str);
   if( l_it == parameter.end ( ) && hasFather ( ) )
   {
           //cout<<"Current Environment is " << count_environment_num ( ) <<endl;
           //cout<<"The father id is " << father -> count_environment_num ( ) <<endl;
           return  father ->getParameter(str);
   }

   else if ( l_it == parameter.end ( ) && !hasFather ( )  )
          return nullptr;

   return l_it -> second;
}

bool EnvironmentElement :: hasFather ( )
{
  return father != nullptr ? true: false;
}

int EnvironmentElement :: getID ( ) const
{
    return id;
}

EnvironmentElement * EnvironmentElement :: getFather ( )
{
    return father;
}

void EnvironmentElement :: printParameters ( )
{
    auto l_it = parameter.begin ( );
    while ( l_it!= parameter.end ( ) )
    {
        cout<< l_it -> first<<"->"<<l_it->second->getType()<<endl;
        l_it++;
    }
}

void LambdaElement :: setEnvironment ( EnvironmentElement * currentEnvironment )
{
    environment = currentEnvironment;
}

void LambdaElement :: print ( ) const
{
   if ( LambdaLocation -> getLeftChild ( ) -> getType ( ) ==COMMA )
    cout << "[lambda closure: (null): 2]" ;
   else
    cout << "[lambda closure: "<< LambdaLocation -> getLeftChild ( ) -> getValue ( ) <<": 2]" ;
}

EnvironmentElement * LambdaElement :: getEnvironment ( )
{
    return environment;
}

int LambdaElement :: getEnvironmentNum ( )
{
    return environment -> getID ( );
}

TreeNode * LambdaElement :: getParameter ( )
{
    return LambdaLocation -> getLeftChild ( );
}

ELEMENT * RecElement :: getLambda ( )
{
    return Lambda;
}

void RecElement :: print ( ) const
{
   cout << "lambda closure recursive ";
}

int TauElement :: get_Num_Tuple ( )
{
    return num_tuple;
}

void TauElement :: print ( ) const
{
    cout<<"tau :" <<num_tuple;
}

void TupleElement :: print ( ) const
{
    cout<<"(";
    auto iter =  TupleArray.begin ( );
    (*iter) -> print ( );
    ++ iter;
    for ( ; iter != TupleArray.end ( ) ; ++iter )
    {
        cout << ", ";
        (*iter) -> print ( );
    }
    cout<<")";
}

void TupleElement :: InsertElement ( ELEMENT * temp )
{
   TupleArray.push_back ( temp );
   ++ num_tuple;
}

vector <ELEMENT * > TupleElement :: getTuple ( )
{
    return TupleArray;
}

void IdentifierElement :: setKey ( bool m )
{
    key = m;
}

void IdentifierElement :: print ( ) const
{
    cout << "Identifier :"<< getValue ( ) ;
}

void IntegerElement :: print ( ) const
{
    cout <<  getIntValue ( ) ;
}

long IntegerElement :: getIntValue ( ) const
{
    return atol ( value.c_str( ) );
}

void IntegerElement :: setIntValue ( long m )
{
    value = to_string ( m );
}

void StringElement :: print ( ) const
{
 string str = getValue ( );
 auto it = str.begin ( );
 for ( ; it != str.end ( ); ++it )
 {
     if ( *it == '\\' && *( it +1 ) == 'n' )
        {
             cout<<endl;
             ++it;
             continue;
        }
    else if ( *it == '\\' && * ( it +1 ) == 't' )
    {
        printf ("\t");
        ++it;
        continue;
    }
    else if ( *it == '\\' && * ( it +1 )  == 'r'  )
    {
        cout<< endl;
        ++it;
        continue;
    }
       cout<<*it;
 }
}

int DeltaElement :: getOption ( ) const
{
    return option;
}

 TreeNode * DeltaElement :: getLocationOpt ( )
 {
   return location_of_opt;
 }

void DeltaElement :: print ( ) const
{
    cout<<"delta";
}

 void OperationElement :: print ( ) const
 {
    if ( type>=TAU && type<=GAMMA )
    {
        cout<< Type[type] ;
        return;
    }
    else if ( type >= PRINT && type <= NULLFUNCTION )
    {
        cout << InnFunctions[type - 53] ;
        return;
    }
    switch ( type )
    {
    case Y :
        {
            cout << "Y";
            return;
        }
    case BETA :
        {
            cout << "BETA";
            return;
        }
    case DELTA :
        {
            cout << "DELTA";
            return;
        }
    case THEN :
        {
            cout << "THEN";
            return;
        }
    case ELSE :
        {
            cout << "ELSE";
            return;
        }
    case NIL :
        {
            cout<<"nil";
            return;
        }
    case TRUE :
        {
            cout << "true";
            return;
        }
    case FALSE :
        {
            cout << "false";
            return;
        }
    case DUMMY :
        {
            cout << "dummy";
            return;
        }
    default :
        {
          cout << "unknown element";
          return;
        }
    }
 }
