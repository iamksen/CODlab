%%
^[+|-]?[0-9]*[.]([0-9]*)([0-9])$  printf("Float\n");
^[+|-]?[0-9]$                     printf("Single digit Interger\n");
^[+|-]?([1-9][0-9]*)([0-9])$      printf("Interger\n"); //numbers which do not start with 0
.                                 ; 
%%
main(){
  yylex();
}
