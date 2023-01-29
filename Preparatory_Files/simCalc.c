#include <stdio.h>
#include <stdlib.h>

int main(){

int num1, num2;

char oper;

printf("Please enter the first number: \n");
scanf("%d",&num1);

printf("Please enter the second number: \n");
scanf("%d",&num2);

printf("Enter the operation you would like to perform:\n");
scanf(" %c",&oper);

//printf("%c",oper);
if(oper == '*'){
int ans;
ans  = num1*num2;	
printf("%d",ans);
}
if(oper== '/'){
int ans;
ans  = num1/num2;
printf("%d",ans);
}
if(oper == '+'){
int ans;
ans = num1+num2;
printf("%d",ans);
}
if(oper == '-'){
int ans;
ans = num1-num2;
printf("%d",ans);
 }   
return 0;
}
