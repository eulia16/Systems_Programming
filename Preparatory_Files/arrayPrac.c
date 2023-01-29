#include <stdio.h>
#include <stdlib.h>
//include function prototypes

double cube(double num);
int max(int num1, int num2);
void fun();

int main(){

printf("%d\n",max(20,12));

//fun();
//printf("%lf",cube(3.0));
//int array[] = {1,2,4,5,53,2,34,43,5,6,7};
//printf("%d\n",array[4]);
return 0;
}

int max(int num1, int num2){
if(num1>num2)
	return num1;
else 
	return num2;

}


double cube(double num){

return num*num*num;
}


void fun(){
printf("the functions name is fuckMe");
}
