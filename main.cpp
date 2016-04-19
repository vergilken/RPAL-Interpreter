#include "Utl.h"

using namespace std;

int main(int argc, char *argv[])
{
/******Test single Rpal programming******/
 #if 0
    cout << "Hello world!" << endl;
    FILE *RpalCode = fopen("democode","r");

   Parser* parser = new Parser(RpalCode);
   parser->getAstTree();
   parser -> StandardizeAST ( );
   //parser ->PreOrder(0);
   cout<<endl;
   cout<<"***************"<<endl;
   CSE * cse = new CSE ( parser );
   cse -> Execute ( );

   #endif

/****************RPAL Translator********************/
   #if 1
     int c;
     int option_index = 0;
     static struct option long_options[] = {
           {"ast",       required_argument,   0,  'a'},
           {"list",       required_argument,   0,  'l'},
           {"st",         required_argument,   0,  's'},
           {"result", required_argument,   0,  'r'},
           {0,   0,   0,  0 }
       };
    const char *short_opts = "a:l:s:r:";
    if((c = getopt_long_only(argc, argv, short_opts, long_options, &option_index))!=-1)
      {
          switch ( c ) {
            case 'a' :
                AstGenerating(optarg);
                break;
            case 'l' :
                ListGenerating(optarg);
                break;
            case 's' :
                STGenerating(optarg);
                break;
            case 'r' :
                ResultGenerating ( optarg );
                break;
            case '?' :
                if ( optopt == 'a' || optopt == 'l' || optopt == 's' || optopt == 'r' )
                    cout<<"Argument missing."<<endl;
                else
                    cout<<"Usage: p1 [-ast] [-l] [-st] [-r] FILE"<<endl;
                return 1;
            default :
                abort();
        }
   }
   #endif

    return 0;
}
