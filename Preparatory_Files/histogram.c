#include <stdio.h>

//void histogram(char name[],int number, char strings[][20], int counter);

//#define NUMSTRINGS 10 /*This is the number of strings the array will hold*/
//#define STRINGLEN  20 /*This is the maximum length the string entered can be*/

int main(){

int inputValue, counter;

char string[10][20];

counter =0;
printf("Please enter the amount of values you are going to enter\n");
scanf("%d", &inputValue);

for(int cnt=0; cnt<inputValue;cnt++){
char stringInput[20];
printf("Enter a string of characters: \n");
scanf("%s", stringInput);
histogram(stringInput, sizeof(stringInput), string, counter);

}

return 0;
}

void histogram(char stringInput[], int size, char string[][20], int counter){
printf(" this is the given string %s, and this is the max size %d\n",stringInput, size);
//must include * in *stringInput as it must dereference the pointer(get actual
//value instead of the address
string[counter][19] = *stringInput;
printf("string[counter][]: %c\n", stringInput[counter]);
counter++;
}
