/* definitions */

%{
#include<stdio.h>
#include "y.tab.h"

int yycol_old = 0;

int yycol = 0;

extern int yylineno;

extern int ST_insert_yy(const char*, int);

#define setcol {yycol = yycol_old+1; yycol_old += yyleng;}

%}

digit       [0-9]
number      0|[1-9]{digit}*

letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*

newline     \r\n|\n\r|\r|\n
whitespace  [ \t]+

comment_good 	"/*"([^*]|"*"[^/])*"*/"
comment_unterm	"/*"([^*]|"*"[^/])*$

character \'(\\.|[^'\n\r])*\'

string_good	\"(\\.|[^"\n\r])*\"
string_unterm	(\"(\\.|[^"\n\r])*)$
%%

 /* rules */

{comment_good}	{setcol; /* valid comment*/}
{comment_unterm} {setcol;
			 fprintf(stderr,"Error at line %d, column %d: Unterminated comment ", yylineno, yycol);
  			 return ERROR;}

{number}        {setcol;
                 yylval.value = atoi(yytext); 
			 return INTCONST;}

{character}    	{
			  setcol; 
			  if (yyleng > 3)
 			  {
			    if ((yyleng > 4) ||
			        (yytext[1] != '\\'))
			    {
			      fprintf(stderr,"Error at line %d, comlumn %d: char literal contains more than one character\n", yylineno, yycol); 
			      return ERROR;
			    }
			    else
			    {
			      if (yytext[2] == '\'')
			      {
			        yylval.value = (int)'\'';
			      }
			      else if (yytext[2] == '\"')
			      {
			        yylval.value = (int)'\"';
			      }
			      else if (yytext[2] == 't')
			      {
			        yylval.value = (int)'\t';
			      }
			      else if (yytext[2] == 'n')
			      {
			        yylval.value = (int)'\n';
			      }
			      else if (yytext[2] == '\\')
			      {
			        yylval.value = (int)'\\';
			      }
			      else
			      {
			        fprintf(stderr,"Error at line %d, comlumn %d: unknown escape sequence %s\n", yylineno, yycol, yytext); 
			        return ERROR;
			      }

			    }
			  }
			  else if (yyleng == 3)
			  {
                    yylval.value = (int)yytext[1];
			  }
			  else
			  {
			    yylval.value = 0;
			  }
                  return CHARCONST;
		     }

{string_good}	{
			  setcol; 

			  int len = yyleng;
			  char temp[len];
			  int i;
			  int offset=1;
			  for (i=0; i<len; i++)
			  {
			    if (yytext[i+offset] == '\\')
			    {
			      if (yytext[i+offset+1] == '\'')
			      {
			        temp[i] = '\'';
			      }
			      if (yytext[i+offset+1] == '"')
			      {
			        temp[i] = '"';
			      }
			      else if (yytext[i+offset+1] == 't')
			      {
			        temp[i] = '\t';
			      }
			      else if (yytext[i+offset+1] == 'n')
			      {
			        temp[i] = '\n';
			      }
			      else if (yytext[i+offset+1] == '\\')
			      {
			        temp[i] = '\\';
			      }
			      else
			      {
			        fprintf(stderr,"Error at line %d, comlumn %d: String literal contains unknown escape sequence\n", yylineno, yycol); 
			        return ERROR;
			      }

			      offset++;
			    }
			    else if (yytext[i+offset] == '"')
			    {
			      len = i+1;
			      break;
			    }
			    else
			    {
			      temp[i] = yytext[i+offset];
			    }
			  }

			  temp[len-1] = 0;
			  yylval.value = ST_insert_yy(temp, len);

                  return STRCONST;
			}

{string_unterm}	{setcol;
			 fprintf(stderr,"Error at line %d, column %d: Unterminated string\n", yylineno, yycol);
  			 return ERROR;}


if|IF           {setcol; return KWD_IF;}
else|ELSE       {setcol; return KWD_ELSE;}
while|WHILE     {setcol; return KWD_WHILE;}
int|INT         {setcol; return KWD_INT;}
string|STRING   {setcol; return KWD_STRING;}
char|CHAR       {setcol; return KWD_CHAR;}
return|RETURN   {setcol; return KWD_RETURN;}
void|VOID       {setcol; return KWD_VOID;}

"+"             {setcol; return OPER_ADD;}
"-"             {setcol; return OPER_SUB;}
"*"             {setcol; return OPER_MUL;}
"/"             {setcol; return OPER_DIV;}
"<="            {setcol; return OPER_LTE;}
">="            {setcol; return OPER_GTE;}
"<"             {setcol; return OPER_LT;}
">"             {setcol; return OPER_GT;}
"=="            {setcol; return OPER_EQ;}
"="             {setcol; return OPER_ASGN;}
"!="            {setcol; return OPER_NEQ;}

"["             {setcol; return LSQ_BRKT;}
"]"             {setcol; return RSQ_BRKT;}
"{"             {setcol; return LCRLY_BRKT;}
"}"             {setcol; return RCRLY_BRKT;}
"("             {setcol; return LPAREN;}
")"             {setcol; return RPAREN;}

","             {setcol; return COMMA;}
";"             {setcol; return SEMICLN;}

{identifier}    {setcol; 
		 char temp[yyleng+1];
		 int i;
		 for (i=0; i<=yyleng; i++)
		   temp[i] = yytext[i];
		 temp[yyleng] = 0;

                 yylval.value = ST_insert_yy(temp, yyleng+1);
                 return ID;}
{newline}       {yylineno++; yycol = 1; yycol_old = 0;}
{whitespace}    {setcol; /* skip whitespace */}

.               {setcol; 
			 fprintf(stderr,"Error at line %d, column %d: Illegal token\n", yylineno, yycol);
			 return ERROR;}

%%


/* user routines */
