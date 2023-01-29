#include <stdio.h>
#include <stdlib.h>

int main(){
//makes a file pointer to a file(employees.txt), then will write to it using "w"
//, if the file already exists it will overwrite it, to append to a file use "a"//, and to read from a file use "r"
//FILE *fPointer = fopen("employees.txt","w");


//this will allow you to append to a file, not just overwrite it(because of     //the 'a'


File *fPointer  = fopen("employees.txt","a");

//this function allows the user to print info to the specified file
fprintf(fPointer, "Jim, SalesMan\nPam, Receptionist\nOscar, Accounting");

//always close a file in C
fclose(fPointer);

return 0; 
}
