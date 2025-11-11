/* --------------------- FLEX DEFINITIONS SECTION --------------------- */
D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include "parser.h"

void count();
void comment();

%}


/* --------------------- LEXICAL RULES SECTION --------------------- */
%%
"/*"			{ comment(); }

"else"			{ count(); return(ELSE); }
"if"			{ count(); return(IF); }
"while"			{ count(); return(WHILE); }
"?"				{ count(); return(STDIN_GET_NUM); }
"print"		    { count(); return(PRINT); }

{L}({L}|{D})*		{ count(); yylval.sval = strdup(yytext); return(IDENTIFIER); }
{D}+   				{ count(); return(CONSTANT); }
\"(\\.|[^\\"])*\"	{ count(); yylval.sval = strdup(yytext); return(STRING_LITERAL); }

"+="			{ count(); return(ADD_ASSIGN); }
"-="			{ count(); return(SUB_ASSIGN); }
"*="			{ count(); return(MUL_ASSIGN); }
"/="			{ count(); return(DIV_ASSIGN); }
"%="			{ count(); return(MOD_ASSIGN); }

"<="			{ count(); return(LE_OP); }
">="			{ count(); return(GE_OP); }
"=="			{ count(); return(EQ_OP); }
"!="			{ count(); return(NE_OP); }

"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }

","			{ count(); return(','); }
";"			{ count(); return(';'); }

("{"|"<%")		{ count(); return('{'); }
("}"|"%>")		{ count(); return('}'); }

"="			{ count(); return('='); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }

"-"			{ count(); return('-'); }
"+"			{ count(); return('+'); }
"*"			{ count(); return('*'); }
"/"			{ count(); return('/'); }
"%"			{ count(); return('%'); }

[ \t\v\n\f] { count(); }
.			{ fprintf(stderr, "Unknown character: %s\n", yytext); exit(1);}


%%

void comment()
{
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		putchar(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}


int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}