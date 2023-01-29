#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//no clue how long they're going to be (max is 64 bit, but anything less goes) 
//calculates the number if hits compared to the total number of reads

//we will define a struct to hold the valid bit, blockOffsetBits, and tagBits
struct cache{
     int validBit;
     long long tag;
     int LRU;//this is for the scenario where LRU is being used
     int LFUCounter;//this is the for the scenario that LFU is being used
};

int main(void){
//these are the four fundamental paremeters we need before anything else
//theyre the first ones passed in so we'll simply take them in first
int S=0, E=0, B=0, m=0, 
tagBits=0, setIndexBits=0, blockOffsetBits=0, 
hitTime=0,missPen=0,
blockMask=0, setMask=0, tagMask=0, 
compValue=1,//input=0,
numInputsCounter=0, numCycles=0, numHits=0;
unsigned long long input=0;

//gotta add an extra slot for that \0
char replacementPol[4];
unsigned char missHit[50][2];//we can take in a maximum of a 64 bit word

//this will set all the values in missHit to zero just to clean it up a bit
for(int i=0; i<50; ++i){

missHit[i][0] = 0;
missHit[i][1]=0;
}

//we get the four parameters that will define the cache
printf("Please enter the four fundamental parameters of a cache to begin\n");
scanf("%d %d %d %d", &S, &E, &B, &m);


//we get replacement policy
printf("Please enter the replacement policy\n");
scanf("%s", replacementPol);

//we then get the two values for the hardware, hit time and miss penalty
printf("Please enter the hit time and the miss penalty: \n");
scanf("%d %d", &hitTime, &missPen);


printf("fund numbers: %d,%d,%d,%d\nreplacementPol: %s\nhit time and miss penalty: %d %d\n", S, E, B, m, replacementPol, hitTime, missPen);
//we will now call four seperate functions to calculate(S, t,s,b), or, the # of sets, number of tag bits,
//number of set index bits, and # of block offset bits

//we now calculate the number of block offset bits
blockOffsetBits = log2(B);//this is b
//now calculate the setIndexBits
setIndexBits = log2(S);//this is s
//we now calculate the number of tagbits and use simple arithmetic for that
tagBits = m-(setIndexBits + blockOffsetBits);//formula is t = m - (s + b)


//this is to make sure we got the right calculations
printf("m: %d, B: %d, E: %d. S: %d, t: %d, s: %d, b: %d\n", m, B, E, S, tagBits, setIndexBits, blockOffsetBits);
//after calculating the values of S, t, s, and b, we now must construct the dynamic bit masks for the tag and set

//we need to make two bit masks to fit 64 bit words, one for the tag and one for the
//set? come back to later to comfirm for the set and tag
//declare cache, then we'll put it on the heap
typedef struct cache cache;
struct cache *myCache[S][E];//we will initialize a 2d cache, the number of sets by the number of lines(S rows by E sets)
//first we need to set all of the valid bits in the struct to zero as the cache will be cold when we first use it
for(int i=0; i<S;++i){
   for(int j=0; j<E;++j){
//add cache to the heap
myCache[i][j] = malloc(sizeof(cache));
//we will set the valid bit to zero, if it is not set, it will result in a miss
myCache[i][j]->validBit = 0;
myCache[i][j]->LFUCounter =0;
myCache[i][j]->LRU =0;
}
}
//i believe the tag mask must be 37 0's, then 26 1's, and then followed by remaining number of 0's

while(input != -1){
int isMatched=0,allValid=0,lowestValue=0, lineNumber=0;
//get address from user(hex)
printf("Enter another value: \n");
scanf("%llx", &input);
printf("Value in decimal: %lld\n", input);
if(input == -1)
	break;
//we add the input to a 2d character array so we can then output it to the user at the end w/ the number of hits/misses/numcycles/hit%
missHit[numInputsCounter][0] = input;
//if this is the first input, we will add M to the first line as this cache is cold
//we then increment the counter for the total number of inputs
//we will then apply bitmasks to obtain the tag and set 
//we need to apply bitmasks for hex input
int tagSize =0;
unsigned long long tagmask=0, temp=0, setMask=0;
tagSize = 64 - (blockOffsetBits + setIndexBits);
tagMask = input >> (setIndexBits + blockOffsetBits);
temp = input << tagSize;
setMask = temp >> (tagSize + blockOffsetBits);


//int tagMask = (input >> (m-blockOffsetBits)) & ((1l << blockOffsetBits)-1);
//int setMask = (input >> (m-blockOffsetBits-setIndexBits)) & ((1l << setIndexBits)-1);


//if there has been no inputs yet, the first entry is a miss and the cache is now not cold
if(numInputsCounter == 0){
missHit[numInputsCounter][1] = 'M';
myCache[setMask][0]->validBit = 1;
myCache[setMask][0]->tag = tagMask;
myCache[setMask][0]->LRU = numInputsCounter;
myCache[setMask][0]->LFUCounter += 1;
numCycles = numCycles + hitTime + missPen;
}
else{//if the cache is not cold, see what the replacement policy is, and then do the adding/cheching for hits/misses
if(strcmp(replacementPol, "LRU") == 0){//if the replacement policy is LRU, do this one
if(E==1){//if the number of lines equals 1, this simplifies things a bit
//if the set and tag match, then it is a bit, and no replacement is necessary
if((myCache[setMask][0]->tag == tagMask) && myCache[setMask][0]->validBit != 0){
missHit[numInputsCounter][1] = 'H';//if all conditions met, its a hit, do the stuff
numCycles = numCycles + hitTime;//we do not include miss penalty because it wa a hit
numHits++;
}
if(myCache[setMask][0]->tag != tagMask || myCache[setMask][0]->validBit == 0){
missHit[numInputsCounter][1] = 'M';//if the valid bits not set or tag doesnt mathc, its a miss
numCycles = numCycles + hitTime + missPen;
//if its a miss we load the data into the struct
myCache[setMask][0]->tag = tagMask;
myCache[setMask][0]->validBit = 1;
}
}
else{//if the number of lines per set != 1
//loop over the set by the number of lines it contains
for(int i=0;i<E;++i){
//if the cache's tag matches the inputs and the valid bit is set, its a hit
if(myCache[setMask][i]->tag == tagMask && myCache[setMask][i]->validBit == 1){
missHit[numInputsCounter][1] = 'H';
numCycles = numCycles + hitTime;
numHits++;
myCache[setMask][i]->LRU = numInputsCounter;//instead of a true or false value for least recently used
//we will just place the value of the number of the input it was, therefore the lowest number is the LRU
isMatched =1;
break;
}
}
if(isMatched == 0){
//loop over each line to see where the least recently used value was 
for(int i=0;i<E;++i){
//if the valid bit is not set in any of the lines, we will call it a miss and place that value into that line 
if(myCache[setMask][i]->validBit==0){
missHit[numInputsCounter][1] = 'M';
numCycles = numCycles + hitTime + missPen;
myCache[setMask][i]->validBit = 1;
myCache[setMask][i]->LRU = numInputsCounter;
myCache[setMask][i]->tag = tagMask;
allValid=1;
break;
}
}
}
//now we arrive at the actual eviction policy, for each line in the specified set, whichever LRU is lowest will be evicted, as by the policy
if(isMatched ==0 && allValid ==0){//if none of the tags matched and all of the valid bits were set, then we must evict a line
lowestValue=0, lineNumber=0;//this will keep track of the lowest value, and will evict the line with the lowest LRU value(as that was the least recently used value)
lowestValue = myCache[setMask][0]->LRU;
for(int i=0;i<E;++i){
if(myCache[setMask][i]->LRU < lowestValue){
//lowestValue = myCache[setMask][i]->LRU;
lineNumber = i;
}
}

//now that we have which line has the lowest value and what line it is on, we will count it as a miss and evict that line with the new data
missHit[numInputsCounter][1] = 'M';
numCycles = numCycles + hitTime + missPen;
myCache[setMask][lineNumber]->LRU = numInputsCounter;
myCache[setMask][lineNumber]->tag = tagMask;
}
}
}
else if(strcmp(replacementPol, "LFU") == 0){

	
if(E==1){//for LRU and LFU the policy doesn't change if there is only one line per set, so its the same if there is only one line per set
if((myCache[setMask][0]->tag == tagMask) && myCache[setMask][0]->validBit != 0){
missHit[numInputsCounter][1] = 'H';//if all conditions met, its a hit, do the stuff
numCycles = numCycles + hitTime;//we do not include miss penalty because it wa a hit
numHits++;
}
if(myCache[setMask][0]->tag != tagMask || myCache[setMask][0]->validBit == 0){
missHit[numInputsCounter][1] = 'M';//if the valid bits not set or tag doesnt mathc, its a miss
numCycles = numCycles + hitTime + missPen;
//if its a miss we load the data into the struct
myCache[setMask][0]->tag = tagMask;
myCache[setMask][0]->validBit = 1;
}
}

else{//now this is if there is more than one line per set, this is where things get interesting

for(int i=0;i<E;++i){
//if the cache's tag matches the inputs and the valid bit is set, its a hit
if(myCache[setMask][i]->tag == tagMask && myCache[setMask][i]->validBit == 1){
missHit[numInputsCounter][1] = 'H';
numCycles = numCycles + hitTime;
numHits++;
myCache[setMask][i]->LFUCounter += 1;

//myCache[setMask][i]->LRU = numInputsCounter;//instead of a true or false value for least recently used
//we will just place the value of the number of the input it was, therefore the lowest number is the LRU
isMatched =1;
break;
}
}

if(isMatched == 0){
//loop over each line to see where the least recently used value was 
for(int i=0;i<E;++i){
//if the valid bit is not set in any of the lines, we will call it a miss and place that value into that line 
if(myCache[setMask][i]->validBit==0){
missHit[numInputsCounter][1] = 'M';
numCycles = numCycles + hitTime + missPen;
myCache[setMask][i]->validBit = 1;
//myCache[setMask][i]->LRU = numInputsCounter;
myCache[setMask][i]->LFUCounter += 1;
myCache[setMask][i]->tag = tagMask;
allValid=1;
break;
}
}
}

if(isMatched ==0 && allValid ==0){//if none of the tags matched and all of the valid bits were set, then we must evict a line
lowestValue=0, lineNumber=0;//this will keep track of the lowest value, and will evict the line with the lowest LRU value(as that was the least recently used value)
lowestValue = myCache[setMask][0]->LFUCounter;
for(int i=0;i<E;++i){
if(myCache[setMask][i]->LFUCounter < lowestValue){
//lowestValue = myCache[setMask][i]->LRU;
lineNumber = i;
}
}

//now that we have which line has the lowest value and what line it is on, we will count it as a miss and evict that line with the new data
missHit[numInputsCounter][1] = 'M';
numCycles = numCycles + hitTime + missPen;
//myCache[setMask][lineNumber]->LRU = numInputsCounter;
myCache[setMask][lineNumber]->LFUCounter=1;
myCache[setMask][lineNumber]->tag = tagMask;
}

}
}
else{
printf("You entered an improper replacement policy, you buffoon\n");
exit(0);
}

}
numInputsCounter++;

}
//now we load these results into the struct
printf("\n");
for(int i=0;i<50;++i){
	if(missHit[i][1] != 0)
	printf("hit or misses: %x %c\n", missHit[i][0], missHit[i][1]);
}
int numMisses = numInputsCounter - numHits;
//rounded up the percentage 
int percentage = ceil(((float)numMisses/(float)numInputsCounter)*100);///numInputsCounter;
printf("miss percentage: %d, number of cycles: %d\n", percentage, numCycles);

for(int i=0; i<S;++i){
   for(int j=0; j<E;++j){
free(myCache[i][j]);
}
}

return 0;
}



