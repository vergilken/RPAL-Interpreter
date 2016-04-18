#include "ELEMENT.h"
#ifndef CSE_H
#define CSE_H

class CSE
{
    public:
        CSE ( Parser * parser );
        virtual ~CSE ( ) { };
        void Flatern ( TreeNode * index );
        void Execute ( );
        void print ( );

        inline int count_environment_num ( )
        {
             return m;
        };

       inline EnvironmentElement * getCurrentEnvironment ( )
       {
            return CurrentEnvironment;
       };

    private:
        TreeNode * root;
        vector <ELEMENT * > Control;
        vector <ELEMENT * > Stack;
        EnvironmentElement * CurrentEnvironment;
        void LeftRecusive ( TreeNode * index );
        void CSE_Machine ( );

        EnvironmentElement * getStackEnvironment ( );
        int  judgeOrOperator ( int _type1,  int _type2 );
        int judgeAndOperator ( int _type1, int _type2 );
        int judgeValue ( ELEMENT * var1, ELEMENT * var2  );

        int m;
};

ELEMENT * TreeNode_To_Element ( TreeNode * node );
#endif // CSE_H
