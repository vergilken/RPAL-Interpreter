#include "Parser.h"

class ELEMENT
{
    public:
        ELEMENT ( );
        ELEMENT ( int _type ) : type ( _type ){};
        ELEMENT ( int _type, string  _value) : type ( _type ), value ( _value ) { };

        virtual ~ELEMENT( ) {  };
        virtual void print ( ) const;
        string getValue ( )const;
        int getType ( ) const;

    protected:
        int type;
        string  value ;
};

class EnvironmentElement : public ELEMENT
{
    public :
        EnvironmentElement ( ) = default;
        EnvironmentElement ( EnvironmentElement * _father, int _id ) : ELEMENT ( ENVIRONMENT ), father ( _father ), id ( _id ) { };
        ~EnvironmentElement ( ) { };
        void print ( ) const override;
        void InsertParameter ( string str  , ELEMENT * element);
        ELEMENT * getParameter ( string m );
        int getID ( ) const;
        bool hasFather ( );
        EnvironmentElement * getFather ( );
        void printParameters ( );

    protected:
        map<string , ELEMENT *> parameter;
        EnvironmentElement * father;
        int id;
};

class LambdaElement : public ELEMENT
{
    public:
        LambdaElement ( ) = default;
        LambdaElement ( TreeNode * _location, EnvironmentElement * _environment ) : ELEMENT ( LAMBDA ), LambdaLocation ( _location ), environment ( _environment )  { };
        ~LambdaElement ( ) { };
        void print ( ) const override;
        void setEnvironment ( EnvironmentElement * currentEnvironment );
        EnvironmentElement * getEnvironment ( );
        int getEnvironmentNum ( );
        TreeNode * getParameter ( );
    protected:
        TreeNode * LambdaLocation;
        EnvironmentElement *  environment;
};

class RecElement : public ELEMENT
{
    public :
        RecElement ( ) = default;
        RecElement ( TreeNode * _location, EnvironmentElement * _environment ) : ELEMENT ( Y ), LambdaLocation ( _location ), environment ( _environment ) { };
        ~RecElement ( ) { };
        void print ( ) const override;
    protected:
        TreeNode * LambdaLocation;
        EnvironmentElement * environment;

};

class TauElement : public ELEMENT
{
    public :
        TauElement ( ) = default;
        TauElement ( int _num_tuple ) : ELEMENT ( TAU ) , num_tuple ( _num_tuple ) { };
        ~TauElement ( ) { };
        void print ( ) const override;
        int get_Num_Tuple ( );
    protected:
        int num_tuple;
};

class TupleElement : public ELEMENT
{
    public :
        TupleElement ( ) = default;
        TupleElement ( int _num_tuple ) : ELEMENT ( TUPLE ), num_tuple ( _num_tuple ) { };
        ~TupleElement ( ) { };
        void print ( ) const override;
        void InsertElement ( ELEMENT * temp );
        vector <ELEMENT * > getTuple ( );
    protected:
        vector <ELEMENT *> TupleArray;
        int num_tuple;
};

class IdentifierElement : public ELEMENT
{
    public :
        IdentifierElement ( ) = default;
        IdentifierElement ( string  _value ) : ELEMENT ( IDENTIFIER,  _value ) { };
        ~IdentifierElement ( ) { };
        void print ( ) const override;
};

class IntegerElement : public ELEMENT
{
    public :
        IntegerElement ( ) = default;
        IntegerElement ( string  _value ) : ELEMENT ( INTEGER, _value ) { };
        ~IntegerElement ( ) { };
       void print ( ) const override;
       long getIntValue ( ) const;
};

class StringElement : public ELEMENT
{
    public :
       StringElement ( ) = default;
       StringElement ( string  _value ) : ELEMENT ( STRING, _value ) { };
       ~StringElement ( ) { };
       void print ( ) const override;
};

class DeltaElement : public ELEMENT
{
    public :
        DeltaElement ( ) = default;
        DeltaElement ( TreeNode * _location_of_opt ) : ELEMENT ( DELTA ), location_of_opt ( _location_of_opt) { };
        ~DeltaElement ( ) { };
        void print ( ) const override;
    protected :
        TreeNode * location_of_opt;
};

class OperationElement : public ELEMENT
{
    public :
        OperationElement ( ) = default;
        OperationElement ( int _type ) : ELEMENT ( _type ) { };
        ~OperationElement ( ) { };
        void print ( ) const override;
};


