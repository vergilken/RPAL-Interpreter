#include "Tokens.h"
#include "Statistics.h"

/* *****Judging characters******/
bool isOperator(char m)
{
  for (char ch:OperatorChar)
      if(ch==m)return 1;
     return 0;
}
bool isPunction(char m)
{
    for(char ch:PunctionChar)
        if(ch==m)return 1;
    return 0;
}
bool ESC(char m){
   for (char ch:ESCChar)
      if(ch==m)return 1;
    return 0;
}

void Lexicol_Error()
{
 cerr<<"Lexical error: "<<"Can not Scanning this token"<<"        ";
 exit(1);
}

void concatennation(char  m,string& token)
{
 token.insert(token.size(),1,m);
}

void retract(FILE *RpalCode) {
    fseek(RpalCode,-1l,SEEK_CUR);
}

string scanner(FILE *RpalCode, int& type)
{
   type=DUMMY;
   char ch=fgetc(RpalCode);
    char nextch;
    string token;

/*****Deleting blanks and comments*****/
    while(ch == ' '||ch == '\t'||ch=='\r'||ch=='\n' )
        ch =  fgetc(RpalCode);
    while(ch=='/')
    {
      nextch=fgetc(RpalCode);
      if(nextch=='/')
             {
                 while(ch!='\n' && ch!=EOF)
                ch=fgetc(RpalCode);
                while(ch == ' '||ch == '\t'||ch=='\r'||ch=='\n' )
                ch =  fgetc(RpalCode);

                }
       else if(nextch!='/')
       {
           retract(RpalCode);
           concatennation(ch,token);
           type=DIVIDE;
           return token;
       }
    }
/*****Scanning Identifiers****/
       if(isalpha(ch)){
        concatennation(ch,token);
        ch=fgetc(RpalCode);
         while((isalpha(ch)||isdigit(ch)||ch=='_')&&ch!=EOF){
            concatennation(ch, token);
            ch = fgetc(RpalCode);
         }
         retract(RpalCode);
   if(token=="true")type=TRUE;
   else if(token=="false")type=FALSE;
   else if(token=="nil")type=NIL;
   else if(token=="in")type=IN;
   else if(token=="where")type=WHERE;
   else if(token=="aug")type=AUG;
   else if(token=="or")type=OR;
   else if(token=="not")type=NOT;
   else if(token=="eq")type=EQ;
   else if(token=="ne")type=NE;
   else if(token=="le")type=LE;
   else if(token=="ls")type=LS;
   else if(token=="ge")type=GE;
   else if(token=="gr")type=GR;
   else if(token=="fn")type=FN;
   else if(token =="let")type=LET;
   else if (token =="rec")type=REC;
   else if(token=="and")type=AND;
   else if(token=="within")type=WITHIN;
   else if(token=="dummy")type=DUMMY;
   else type=IDENTIFIER;
   return token;
       }

/****Judging Integers*****/
    else if(isdigit(ch)){
        while(isdigit(ch))
        {
            concatennation(ch, token);
            ch = fgetc(RpalCode);
        }
        retract(RpalCode);
        type=INTEGER;
        return token;
     }

/******Operators Judging******/
  else if(isOperator(ch)&&ch!='/')
  {
    concatennation(ch,token);
    ch=fgetc(RpalCode);
    char nextch;
    while(isOperator(ch))
    {
       if(ch!='/'){
             concatennation(ch,token);
             ch=fgetc(RpalCode);
       }
       else if(ch=='/')
       {
           nextch=fgetc(RpalCode);
           if(nextch == '/')
             {
                 while(ch!='\t' && ch!='\n' && ch!='\r' && ch!=EOF) ch=fgetc(RpalCode);
                 ch=fgetc(RpalCode);
             }
          else if(nextch != '/')
              {
                retract(RpalCode);
                concatennation(ch,token);
               ch=fgetc(RpalCode);
              }
       }
    }
   retract(RpalCode);
   if(token=="->")type = TERNERY;
   else if ( token == "&" )type = AND_LOGICAL;
   else if  ( token == ">" )type =  GR;
   else if ( token == ">=" )type = GE;
   else if ( token == "<" )type = LS;
   else if ( token == "<=" )type = LE;
   else if ( token == "eq" )type = EQ;
   else if ( token == "ne" )type = NE;
   else if( token =="+" )type = PLUS;
   else if ( token =="-" )type = MINUS;
   else if ( token =="*" )type = MULTIPLY;
   else if ( token == "**" )type = POWER;
   else if ( token == "@" )type = AT;
   else if ( token == "=" ) type = BINDING;
   else type=OPERATOR;
   return token;
  }
     /*******String judging********/
     else if (ch=='\''){
       ch=fgetc(RpalCode);
        while(ch!=EOF&&ch!='\''){
          concatennation(ch,token);
          ch=fgetc(RpalCode);
        }
            if(ch=='\'')
                 {
                     type=STRING;
                    return token;
                }
                else if(ch==EOF)
                    cerr<<"Lexical error: "<<"The string scanning fails."<<"       ";
                   exit(1);
     }

/********Punction judging********/
    else if(ch=='('||ch==')'||ch==';'||ch==','){
    concatennation(ch,token);
    if(token=="(")type = OPENPARENTNESIS;
    else if(token==",")type = COMMA;
    else if(token==")")type =  CLOSEPARENTNESIS;
    else if(token==";")type = SEMICOLON;
    return token;
  }

  /**********End of file judging***************/
   else if(ch==EOF)
   {
    type=ENDOFRPALFILE;
     return "Ending";
   }
    Lexicol_Error();
   return "Error";
}

