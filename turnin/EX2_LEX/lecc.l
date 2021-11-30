

/*Definition Section*/
%{
#include<stdio.h>
int line_count = 0; 
int keyword_count = 0; 
int ident_count = 0; 

%}


/*Rule Section*/
%%



asm|auto|bool|break|case|catch|char|class|const|const_cast {line_count++;keyword_count++; printf("keyword\n"); }
contiune|default|delete|do|double|else|enum|explicit|export {line_count++;keyword_count++; printf("keyword\n"); }
extern|false|float|for|friend|goto|if|inline|int|long|mutable {line_count++;keyword_count++; printf("keyword\n"); }
namespace|new|operator|private|protected|public|register {line_count++;keyword_count++; printf("keyword\n"); }
reinterpret_cast|return|short|signed|sizeof|static|static_cast {line_count++;keyword_count++; printf("keyword\n"); }
struct|switch|template|this|throw|true|try|typedef|typeid|typename {line_count++;keyword_count++; printf("keyword\n"); } 
union|unsigned|using|virtual|void|volatile|wchar_t|while {line_count++;keyword_count++; printf("keyword\n"); }

([a-zA-Z][0-9])+|[a-zA-Z]* {line_count++;printf("%s", yytext); ident_count++; printf("    Identifier   line: ");printf("%d",line_count);printf("%s","\n");  }  

^[0-9]+ {line_count++; printf("Not a Identifier\n");}

.|\n; 
%%

/*call the yywrap function*/
int yywrap()					
{
return 1;
}


/*Auxiliary function*/
/*Driver function*/
int main(int arg, char *argc[])
{
/*call the yylex function.*/
yyin = fopen(argc[1],"r"); 
yylex();
printf("\n");  
fclose(yyin); 
}
