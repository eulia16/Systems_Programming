#include <stdio.h>
#include <stdlib.h>

int main(){

int age=30;
int * pAge = &age;
double gpa = 3.6;
double * pGpa = &gpa;
char grade = 'A';
char * pGrade = &grade;
//this prints out a normal memory address
printf("%p\n",pGpa);
//to 'dereference' a pointer, you must change two things
//first, in the print statement you must add an asterisk as it appears in the 
//variable definition i.e printf("%d\n", *pAge);
//you must also change the %p to a %d as it is the value in the memory address
//now, not just the memory address itself
printf("%d\n", *pAge);

return 0;
}
