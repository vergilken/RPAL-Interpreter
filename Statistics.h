#ifndef STATISTICS_H
#define STATISTICS_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<math.h>

/************Variable Type**************/
#define LAMBDA  0
#define  WHERE 1
#define TAU 2
#define AUG 3
#define TERNERY 4
#define OR 5
#define AND_LOGICAL 6
#define NOT 7
#define GR 8
#define GE 9
#define LS 10
#define LE 11
#define EQ 12
#define NE 13
#define PLUS 14
#define MINUS 15
#define NEG 16
#define MULTIPLY 17
#define DIVIDE 18
#define POWER 19
#define AT 20
#define GAMMA 21
#define TRUE 22
#define FALSE 23
#define NIL 24
#define DUMMY 25
#define WITHIN 26
#define AND 27
#define REC 28
#define BINDING 29
#define FCN_FORM 30
#define EMPTY_BRACKET 31
#define COMMA 32
#define  LET 33
#define IN 34
#define FN 35

#define IDENTIFIER 36
#define STRING 37
#define INTEGER 38
#define Y 39
#define PUNCTION 40
#define OPENPARENTNESIS 41
#define CLOSEPARENTNESIS 42
#define SEMICOLON 43
#define ENDOFRPALFILE 44
#define OPERATOR 45

#define BELTA  46
#define THEN 47
#define ELSE 48
#define ENVIRONMENT 49
#define TUPLE 50
#define DELTA 51

#define COMPAREERROE 52
#define PRINT 53
#define ITOS 54
#define ISTRUTHVALUE 55
#define ISSTRING 56
#define ISINTEGER 57
#define ISTUPLE 58
#define ORDER 59
#define STERN 60
#define STEM 61
#define CONC 62


/************Standardizing Tree Recursive Condition**************/


using namespace std;

const string Type[]={"lambda","where","tau","aug","->","or","&","not","gr","ge","ls","le","eq","ne","+","-","neg","*","/","**","@","gamma","<true>","<false>","<nil>","<dummy>","within","and","rec","=","function_form","()",",","let","in","fn"};
const string key[]={"let","in","fn","where","aug","or","not","gr","ge","ls","le","eq","ne",".","&","+","-","within","and","rec"};
const char OperatorChar[]={'+','-','*','<','>','&','.','@','/',':','=','~','|','$','!','#','%','^','_','[',']','{','}','`','?','"'};
const char PunctionChar[]={'.',';','(',')'};
const char ESCChar[]={'\n',' ','\t','\\'};
const string InnFunctions [] = {"Print", "ItoS", "Istruthvalue", "Isstring", "Isinteger", "Istuple", "Order", "Stern", "Stem", "Conc"};


#endif // STATISTICS_H
