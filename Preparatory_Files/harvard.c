#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(){
char *s, *t;

printf("Enter a string for s: \n");
fgets(s, 20, stdin);

printf("%s\n",s);

t = malloc(strlen(s)+1);

strcpy(t,s);

t[0] = toupper(t[0]);

printf("Entered Value: %s\nCapitalized Value: %s\n",s,t);

free(&t);
printf("%s\n",t);

//typedef char *String;

//String s = "wassup homies";

//printf("%s\n", s);



return 0;
}
