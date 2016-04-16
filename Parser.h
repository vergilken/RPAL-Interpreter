#ifndef PARSER_H
#define PARSER_H
#include "NaryTree.h"

class Parser
{
    friend class CSE;
    public:
        Parser(FILE *file);
        virtual ~Parser();
        void BuildTree(int type, string value);
        void BuildTree(int type, unsigned num__pop);
        void PreOrder(const int num);
        string getNextTokenValue(){return NextToken.getValue();}
        int getNextTokenType(){return NextToken.getType();}
        string getTokenValue(){return Token.getValue();}
        int getTokenType(){return Token.getType();}
        void StandardizeAST ( );
        void readToken(string value);
        void getAstTree();
        void readNextToken();

        //Rpal Phrase  grammar Production functions declaring
        void E();
        void Ew();
        void T();
        void Ta();
        void Tc();
        void B();
        void Bt();
        void Bs();
        void Bp();
        void A();
        void At();
        void Af();
        void Ap();
        void R();
        bool Rn();//  boolean function for R() judgment
        void D();
        void Da();
        void Dr();
        void Db();
        void Vb();
        void Vl();

    private:
        vector<NaryTree*>astTree;
        FILE *RpalFile;
        Tokens Token;
        Tokens NextToken;
};
#endif // PARSER_H
