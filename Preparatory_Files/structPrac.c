#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student{
char name[50];
char major[50];
int age;
double gpa;
};


int main(){

struct Student student1;

strcpy( student1.name,"MajorTom");
strcpy( student1.major, "Biology");
student1.age = 23;
student1.gpa = 3.5;

//printf("name: %s",student1.name);
printf("%d",student1.age);


return 0;
}
