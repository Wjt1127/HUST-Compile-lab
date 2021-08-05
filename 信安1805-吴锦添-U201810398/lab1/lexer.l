/*yylineno记录行号*/
%option yylineno  
/*定义部分*/
%{
    #include "parser.tab.h"
    #include <string.h>
    #include "def.h"
    int yycolumn = 1;
    #define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno; yylloc.first_column=yycolumn; yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
    typedef union{
        int type_int;
        float type_float;
        char type_char;
        char type_id[32];
        struct node *ptr;
    }YYLVAL;
    #define YYSTYPE YYLVAL
%}
/*辅助定义*/
id [A-Za-z][A-Za-z0-9]*
int [0-9]+
float ([0-9]*\.[0-9]+)|([0-9]+\.)

%%
{int} {yylval.type_int=atoi(yytext);return INT;}
{float} {yylval.type_float=atof(yytext); return FLOAT;}
"int" {strcpy(yylval.type_id,yytext); return TYPE;}
"float" {strcpy(yylval.type_id,yytext); return TYPE;}
"char" {strcpy(yylval.type_id,yytext); return TYPE;}
"return" {return RETURN;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;}
"for" {return FOR;}
{id} {strcpy(yylval.type_id,yytext); return ID;}

";" {return SEMI;}
"," {return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" {strcpy(yylval.type_id,yytext); return RELOP;}
"=" {return ASSIGNOP;}
"+" {return PLUS;}
"-" {return MINUS;}
"+=" {return COMADD;}
"-=" {return COMSUB;}
"++" {return AUTOADD;}
"--" {return AUTOSUB;}
"*" {return STAR;}
"/" {return DIV;}
"&&" {return AND;}
"||" {return OR;}
"!" {return NOT;}
"(" {return LP;}
")" {return RP;}
"[" {return LB;}
"]" {return RB;}
"{" {return LC;}
"}" {return RC;}
[\n] {yycolumn=1;}
[ \r\t] {;}
/*注释处理 单行+多行*/
\/\/[^\n]*  {;}//匹配注释的正则表达式
\/\*(\s|.)*?\*\/ {;}//匹配注释的正则表达式
.   {printf("Error type A: Mysterious character\"%s\" at line %d,column %d\n",yytext,yylineno,yycolumn);}


%%

int yywrap(){
    return 1;
}

