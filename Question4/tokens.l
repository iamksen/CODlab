%%
#include[ ]*<.* printf("Header file : %s\n",yytext);
#define[ ]+.*   printf("Macro : %s\n", yytext);
auto      printf("Keyword : %s\n", yytext);
const     printf("Keyword : %s\n", yytext);
double    printf("Keyword : %s\n", yytext);
float     printf("Keyword : %s\n", yytext);
int       printf("Keyword : %s\n", yytext);
short     printf("Keyword : %s\n", yytext);
struct    printf("Keyword : %s\n", yytext);
unsigned  printf("Keyword : %s\n", yytext);
break     printf("Keyword : %s\n", yytext);
continue  printf("Keyword : %s\n", yytext);
else      printf("Keyword : %s\n", yytext);
for       printf("Keyword : %s\n", yytext);
long      printf("Keyword : %s\n", yytext);
signed    printf("Keyword : %s\n", yytext);
switch    printf("Keyword : %s\n", yytext);
void      printf("Keyword : %s\n", yytext);
case      printf("Keyword : %s\n", yytext);  
default   printf("Keyword : %s\n", yytext);
enum      printf("Keyword : %s\n", yytext);
goto      printf("Keyword : %s\n", yytext);
register  printf("Keyword : %s\n", yytext);
sizeof    printf("Keyword : %s\n", yytext);
typedef   printf("Keyword : %s\n", yytext);
volatile  printf("Keyword : %s\n", yytext);
char      printf("Keyword : %s\n", yytext);
do        printf("Keyword : %s\n", yytext);
extern    printf("Keyword : %s\n", yytext);
if        printf("Keyword : %s\n", yytext);
return    printf("Keyword : %s\n", yytext);
static    printf("Keyword : %s\n", yytext);
union     printf("Keyword : %s\n", yytext);
while     printf("Keyword : %s\n", yytext);

"//"(.*)	                 printf("Single line Comment : %s\n", yytext);
"/*"([^*]|\*+[^*/])*\*+"/"   printf("Multiline Comment : %s\n", yytext);

[_a-zA-Z][a-zA-Z0-9]*        printf("Identifier : %s\n", yytext);
[1-9][0-9] |
0           printf("Interger\n");
[(|)|{|}|,] printf("Delimiter : %s\n",yytext);
\t          printf("Tabspace\n");
\n          printf("Newline\n");
.           ;
%%
main(){
  yylex();
}