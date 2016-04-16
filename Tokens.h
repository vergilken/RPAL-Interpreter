#ifndef TOKENS_H
#define TOKENS_H

#include "Statistics.h"

class Tokens
{
    public:
        Tokens(string _value = "",int _type = DUMMY):value(_value),type(_type){};
        virtual ~Tokens(){};
        string getValue(){return value;}
        int getType(){return type;}
        void setValue(string _value){value = _value;}
        void setType(int _type){type = _type;}
        void identifyToken(string value);

    private:
        string value;
        int type;
};

bool isPunction(char ch);
bool ESC(char ch);
void retract(FILE *fp) ;
bool isOperator(char m);
void Lexicol_Error();
void concatennation(char m, string & token);
string scanner(FILE *RpalCode, int &type);

#endif // TOKENS_H
