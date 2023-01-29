#include <stdio.h>

int main(){
int a = 10;
int *p;
//assigning pointer variable the memory address of a
p = &a;
printf("Value of p: %p\n", p);
printf("Value of &p: %p\n",&p);
printf("Value of &a: %p\n", &a);
printf("Value of *p: %d\n", *p);
printf("Value of a: %d\n", a);






return 0;
}
