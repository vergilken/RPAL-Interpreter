#ifndef UTL_H
#define UTL_H
#include <string.h>
#include <getopt.h>
#include "Tokens.h"
#include "CSE.h"

#if 1
void AstGenerating(const char * optarg)
{
        FILE *RpalCode ;
        const char* FilePtr= optarg;
        RpalCode = fopen(FilePtr,"r");
        if(RpalCode==nullptr)
        {
           cerr<<"This file "<< optarg<< " does not exist."<<endl;
           exit(1);
        }
        Parser* parser = new Parser(RpalCode);
        parser->getAstTree();
        parser->PreOrder(0);
        free(RpalCode);
}

void ListGenerating(const char* optarg)
{
        FILE *RpalCode ;
        const char* FilePtr= optarg;
        FilePtr= optarg;
        RpalCode = fopen(FilePtr,"r");
         if(RpalCode==nullptr)
        {
           cerr<<"This file "<< optarg<< " does not exist."<<endl;
           exit(1);
        }
        string str;
        int type=0;
        while(true)
       {
             str=scanner(RpalCode, type);
             if(type == ENDOFRPALFILE)break;
             cout<<str<<endl;
        }
        free(RpalCode);
}

void STGenerating(const char * optarg)
{
        FILE *RpalCode ;
        const char* FilePtr= optarg;
        RpalCode = fopen(FilePtr,"r");
        if(RpalCode==nullptr)
        {
           cerr<<"This file "<< optarg<< " does not exist."<<endl;
           exit(1);
        }
        Parser* parser = new Parser(RpalCode);
        parser->getAstTree();
        parser->StandardizeAST();
        parser ->PreOrder(0);
        free(RpalCode);
}

#endif

#endif // UTL_H
