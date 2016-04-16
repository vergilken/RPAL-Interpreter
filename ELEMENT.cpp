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
    cout << "element" << endl;
}

void EnvironmentElement :: print ( ) const
{
    cout << "environment : " << id <<endl;
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
    return  father ->getParameter(str);
   else if ( l_it == parameter.end ( ) && !hasFather ( )  )
   {
       l_it = parameter.begin( );
       return l_it -> second;
   }
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
    cout << "lambda closure" << endl;
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

void RecElement :: print ( ) const
{
   cout << "rec "<<endl;
}

int TauElement :: get_Num_Tuple ( )
{
    return num_tuple;
}

void TauElement :: print ( ) const
{
    cout<<"tau"<<endl;
}

void TupleElement :: print ( ) const
{
    cout<<"tuple"<<endl;
}

void TupleElement :: InsertElement ( ELEMENT * temp )
{
   TupleArray.push_back ( temp );
}

vector <ELEMENT * > TupleElement :: getTuple ( )
{
    return TupleArray;
}

void IdentifierElement :: print ( ) const
{
    cout << "Identifier :"<< getValue ( ) <<endl;
}

void IntegerElement :: print ( ) const
{
    cout << "Integer : " << getIntValue ( ) <<endl;
}

long IntegerElement :: getIntValue ( ) const
{
    return atol ( value.c_str( ) );
}

void StringElement :: print ( ) const
{
    cout << "\'"<< getValue ( ) <<"\'"<<endl;
}

void DeltaElement :: print ( ) const
{
    cout<<"delta"<<endl;
}

 void OperationElement :: print ( ) const
 {
     if ( type>=LAMBDA && type<=LET ) cout<< Type[type] <<endl;
    else cout << "unknown element"<<endl;
 }
