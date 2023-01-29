#include <stdio.h>
#include <stdlib.h>

int main(){
char name[70];

printf("Enter your name: \n");
//
//scanf("%s", name);
//you can use scanf but if you enter a space, ut will reject everything entered after the space i.e. if i entered ethan u as a string, only ethan would get stored inside the name variable
//using fgets can specify the amount of characters you want and is safer
fgets(name, 70, stdin);
printf("This is you age: %s\n",name);
}
