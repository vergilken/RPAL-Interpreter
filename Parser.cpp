#include "Parser.h"

Parser::Parser(FILE *file)
{
    RpalFile=file;
}

Parser::~Parser()
{
    //dtor
}

void Parser::BuildTree(int type, string value)
{
   astTree.push_back(new NaryTree(new TreeNode(value, type)));
}

void Parser::BuildTree(int type, unsigned num_pop)
{
  if( astTree.size()>=num_pop && num_pop>=0){
   TreeNode* root = new TreeNode(type);
   int i = num_pop;
   while(i!=0){
        auto iter = astTree.end()-i;
    root->addChild((*iter)->getRoot());
        i--;
   }
     i = num_pop;
     while(i>0){
            astTree.pop_back();
            i--;
     }
     NaryTree* tree = new NaryTree(root);
     astTree.push_back(tree);
  }
  else
  {
      cout<<"Build tree error: Tokens pop overflow."<<endl;
      cout<<"There are "<<astTree.size()<<" trees in stack. "<<endl;
      exit(1);
  }
}

// Beautifully printing AST
void Parser::PreOrder(const int num){
    if(!astTree.empty())
       {
             for(auto tree:astTree)
                tree->PreOrder(num);
       }
}

//Function using to Generate AST Tree for Rpal Programming
void Parser::readNextToken()
{
    Token.setType(NextToken.getType());
    Token.setValue(NextToken.getValue());
    int type =DUMMY;
    string value = scanner(RpalFile,type);
    NextToken.setType(type);
    NextToken.setValue(value);
}

void Parser::readToken(string value)
{
    if(NextToken.getType()!=ENDOFRPALFILE){
    if(Token.getValue()!=value)
   {
    cerr<<"Unexpected token has been read."<<endl;
    exit(1);
    }
         readNextToken();
    }
}

//AST generating
void Parser::getAstTree()
{
  readNextToken();
  readNextToken();
  E();
 if(NextToken.getType()!=ENDOFRPALFILE)
{
    cerr<<"Parsing process has been interrupted, and the end of rpal file has been reached."<<endl;
    exit(1);
}
}

/////////////////////////////////

void Parser::Vl()
{
    BuildTree(IDENTIFIER,Token.getValue());
    readNextToken();
    int n =0;
    if(Token.getType()==COMMA)
    {
        do{
               readNextToken();
               BuildTree(IDENTIFIER,Token.getValue());
               readNextToken();
               ++n;
              }while(Token.getType()==COMMA);
              BuildTree(COMMA,n+1);
              }
}

void Parser::Vb()
{
  if(Token.getType()==OPENPARENTNESIS)
  {
      readNextToken();
      if(Token.getType()==CLOSEPARENTNESIS)
      {
          readNextToken();
          BuildTree(EMPTY_BRACKET,"()");
      }
      else {
        Vl();
        readToken(")");
      }
  }
      else if (Token.getType()==IDENTIFIER){
        BuildTree(IDENTIFIER,Token.getValue());
        readNextToken();
      }
  else
         {
             cerr<<"Expected token ( or identifier here, but "<<Token.getValue()<<" exists."<<endl;
             exit(1);
         }
}

void Parser::Db()
{
    if(Token.getType()==OPENPARENTNESIS)
    {
        readNextToken();
        D();
        readToken(")");
    }
    else if(Token.getType()==IDENTIFIER)
    {
         switch(NextToken.getType())
         {
             case BINDING:    Vl();      readToken("=");         E();         BuildTree(BINDING,2);   break;
             case COMMA:      Vl();     readToken("=");          E();        BuildTree(BINDING,2);    break;
             default:               BuildTree(IDENTIFIER,Token.getValue());
                                           readNextToken();
                                           int n=0;
                                           do{
                                                 Vb();  ++n;
                                                 }while(Token.getType()==OPENPARENTNESIS || Token.getType()==IDENTIFIER) ;
                                           readToken("=");   E();
                                           BuildTree(FCN_FORM,n+2);  break;
         }
    }
   else {
    cerr<<"Unexpected identifier exists in Db."<<endl;
   exit(1);
   }
}

void Parser::Dr()
{
    if ( Token.getType() != REC ) Db();
    else if ( Token.getType() == REC )
    {
        readNextToken();
        Db();
        BuildTree(REC,1);
    }
}

void Parser::Da()
{
    Dr();
    int n = 0;
    if(Token.getType()==AND)
    {
        do{
            readToken("and");
            ++n;
            Dr();
        }while(Token.getType()==AND);
        BuildTree(AND,n+1);
    }
}

void Parser::D()
{
    Da();
    if(Token.getType()==WITHIN)
    {
        readToken(Token.getValue());
        D();
        BuildTree(WITHIN,2);
    }
}


bool Parser::Rn()
{
   if(Token.getType()==INTEGER)
   {
       BuildTree(INTEGER,Token.getValue());
       readNextToken() ;
       return true;
   }
   else if(Token.getType()==STRING)
   {
       BuildTree(STRING,Token.getValue());
       readNextToken();
       return true;
   }
   else if(Token.getType()==IDENTIFIER)
   {
       BuildTree(IDENTIFIER,Token.getValue());
       readNextToken();
       return true;
   }
   else if(Token.getType()==NIL)
   {
       BuildTree(NIL,Token.getValue());
       readNextToken();
       return true;
   }
   else if(Token.getType()==TRUE)
   {
       BuildTree(TRUE,Token.getValue());
       readNextToken();
       return true;
   }
   else if(Token.getType()==OPENPARENTNESIS)
   {
       readNextToken();
       E();
       readToken(")");
       return true;
   }
   else if(Token.getType()==DUMMY)
   {
       BuildTree(DUMMY,Token.getValue());
       readNextToken();
       return true;
   }
   else if(Token.getType()==FALSE)
   {
       BuildTree(FALSE,Token.getValue());
       readNextToken();
       return true;
   }
   else return false;
}

void Parser::R()
{
   Rn();
   while(Rn())
    BuildTree(GAMMA,2);
}

void Parser::Ap()
{
  R();
  while(Token.getType()==AT)
  {
      readNextToken();
      BuildTree(IDENTIFIER,Token.getValue());
      readNextToken();
      R();
      BuildTree(AT,3);
  }
}

void Parser::Af()
{
    Ap();
    if(Token.getType()==POWER)
    {
        readNextToken();
        Af();
        BuildTree(POWER,2);
    }
}

void Parser::At()
{
    Af();
    while(Token.getType()==MULTIPLY||Token.getType()==DIVIDE)
    {
        int Temp_Token =  Token.getType();
        readNextToken();
        Af();
        switch (Temp_Token)
        {
            case MULTIPLY: BuildTree(MULTIPLY,2);       break;
            case DIVIDE: BuildTree(DIVIDE,2);     break;
            default:    cerr<<"Expected token * or / here."<<endl;  exit(1);
        }
    }
}

void Parser::A()
{
    switch(Token.getType())
    {
        case PLUS: readNextToken();    At();       break;
        case MINUS: readNextToken();    At();  BuildTree(NEG,1);              break;
        default: At();      break;
    }

    while(Token.getType()==PLUS||Token.getType()==MINUS)
    {
        int Temp_Token =  Token.getType();
        readNextToken();
        At();
        switch (Temp_Token)
        {
            case PLUS: BuildTree(PLUS,2);       break;
            case MINUS: BuildTree(MINUS,2);     break;
            default:    cerr<<"Expected token + or - here."<<endl;  exit(1);
        }
    }
}

void Parser::Bp()
{
    A();
    switch(Token.getType())
    {
        case GR: readNextToken();   A();    BuildTree(GR,2);   break;
        case GE: readNextToken();   A();    BuildTree(GE,2);    break;
        case LS:  readNextToken();   A();    BuildTree(LS,2);    break;
        case LE:  readNextToken();   A();    BuildTree(LE,2);    break;
        case EQ: readNextToken();   A();    BuildTree(EQ,2);    break;
        case NE: readNextToken();   A();    BuildTree(NE,2);   break;
    }
}

void Parser::Bs()
{
   if(Token.getType()!=NOT)
    Bp();
   else if(Token.getType()==NOT)
   {
       readToken("not");
       Bp();
       BuildTree(NOT,1);
   }
}

void Parser::Bt()
{
    Bs();
    while(Token.getType()==AND_LOGICAL)
    {
        readToken("&");
        Bs();
        BuildTree(AND_LOGICAL,2);
    }
}

void Parser::B()
{
    Bt();
    while(Token.getType()==OR)
    {
     readToken("or");
     Bt();
     BuildTree(OR,2);
    }
}

void Parser::Tc()
{
    B();
    if(Token.getType()==TERNERY)
    {
        readToken("->");
        Tc();
        readToken("|");
        Tc();
        BuildTree(TERNERY,3);
    }
}

void Parser::Ta()
{
   Tc();
   while(Token.getType()==AUG)
   {
       readToken("aug");
       Tc();
       BuildTree(AUG,2);
   }
}

void Parser::T()
{
   Ta();
   int n =0;
   if(Token.getType()==COMMA)
   {
       do{
        readToken(",");
        Ta();
        ++n;
       }while(Token.getType()==COMMA);
       BuildTree(TAU,n+1);
   }
}

void Parser::Ew()
{
    T();
    if(Token.getType()==WHERE)
    {
        readToken("where");
        Dr();
        BuildTree(WHERE,2);
    }
}

void Parser::E()
{
    switch(Token.getType())
            {
                case LET: readToken("let");   D();
                                 readToken("in");    E();
                                 BuildTree(LET,2);   break;
                case FN:  readToken("fn");
                                 int n =0 ;
                                 do{
                                        Vb(); n++;
                                      } while( Token.getType() == OPENPARENTNESIS );
                                 readToken("."); E();
                                 BuildTree(LAMBDA,n+1);
                                 break;
            }
            Ew();
}

void Parser :: StandardizeAST ()
{
  if ( astTree .size() !=1 )
  {
      cerr << "Standardize Tree error exists."<<endl;
      return;
  }
  TreeNode * index = (*(astTree.begin())) -> getRoot();
  index -> AST_TO_ST ();
}






