#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Ethan Uliano
// Lab: 1
//note: I am deeply sad that we have left roger in the dust this class, I miss him very much, I will honor him by naming this game file after him, RIP Roger

#define CREATURES_LENGTH 10
//This is the Room struct, it defines the structure of a room, pretty much the basis of the games architecture, the creatures move in and out of rooms
struct Room{
//change the neighbors values to pointers because it will make linking the rooms together very much easier
int state;
int north;
int south;
int east;
int west;
int creatures[10];
int counter;
};
//This is the global respect value, we all know what this means, im pretty sure its initialized @ 40, change that if im wrong
int respect = 40;
//This function takes in all of the user input and creates the rooms, I believe I am going to change the structs neighbor ints to pointers as then it will 
//pre-link all of the rooms together, make this addition after starting the game loop
void makeRooms(struct Room *rooms, int state, int north, int south, int west, int east, int counter);
//This function allows animals to be added to the creatures array in each room
//void addCreatures(int type, int location, struct Room *rooms);
//This will be the look command, it will be invoked whenever the player wants to look
void gameLoop(int numRooms, struct Room *rooms[numRooms], int pcLoc);
//This is the help command, it will present the user with all of the commands they can use(it is an instructional/informational method)
void help();
//This function will allow the user to see what is in their current room(the PC's current room)
void look(struct Room *rooms, int pcLoc);
//This is all of the actions to move to a neighbor
int north(struct Room *rooms, int pcLoc);
int south(struct Room *rooms, int pcLoc);
int east(struct Room *rooms, int pcLoc);
int west(struct Room *rooms, int pcLoc);
//the following commands will move a creature to a specific direction
void forceNorth(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc);
void forceWest(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc);
void forceEast(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc);
void forceSouth(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc);

//These are the add creatures and remove creatures to a room function
void addCreature(int type, struct Room *rooms);
void removeCreature(int type, struct Room *rooms);
//These functions allow the respect to be altered, they are specific to NPC's and Animal's respectively
void smile(int creature);
void grumble(int creature);
void lickFace(int creature);
void growl(int creature);
//These functions will occur when a creature is forced to clean/dirty a room(the respect is effected 3x the normal amount
void largeSmile(int creature);
void largeGrumble(int creature);
void largeLickFace(int creature);
void largeGrowl(int creature);
//These will be the clean and dirty functions, they will pretty much be the main way to win/lose the game
void clean(int numRooms, struct Room *rooms[numRooms], int pcLoc, int isForced, int type);
void dirty(int numRooms, struct Room *rooms[numRooms], int pcLoc, int isForced, int type);
//These functions will force a creature to clean/dirty a room
void forcedClean(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc);
void forcedDirty(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc);
//This fucntion comes up with a random neighbor for the animals/NPC's that have to change rooms due to a state change
int randomNeighbor(struct Room *rooms);
//these will remove all of the creatures of a specific type from the room, i.e. if a room becomes dirty, they will be removed from the current room and placed//into a neighboring room they can inhabit, if the neighboring room is not of an inhabitable state, the state of that room is changed from to half-dirty
//Another clause is that if the room has no neighbors, or all of the neighboring rooms are full, the creature will drill a hole in the wall and leave the game//This will rarely come up, but make sure to implement something to deal with that
//void removeAllAnimals(struct Room *rooms);
//void removeAllNPC(struct Room *rooms);


int main(void){

int numRooms = 0, numCreatures = 0, pcLoc, state, north, east, south, west, type, location;

typedef struct Room Room;

printf("Please enter how many rooms you're gonna make: \n");
scanf("%i", &numRooms);
//must initialize a pointer to an array of structs before we can allocate memory on the heap for it, it is initialized here
struct Room *rooms[numRooms];
//allocate memory on heap for array of structs, maybe add some validation of memory, not sure at this point
for(int i=0;i<numRooms;++i){
	//check to see if you need to multpiply by maxnumRooms, or take this one out of the for loop and just allocate memory for the numROoms*sizeof(rooms)
	rooms[i] = malloc(sizeof(Room));
	printf("Enter the room info\n");
	scanf("%d %d %d %d %d", &state, &north, &east, &west, &south);
	makeRooms(rooms[i], state, north, east, west, south, 0);
}
printf("Please enter how many creatures you're gonna make\n");
scanf("%d", &numCreatures);

for(int i=0;i<numCreatures;++i){
	printf("Enter the creature info:\n");
	scanf("%d %d", &type, &location);
	if(type == 0){
	       pcLoc = location;		
	}
	addCreature(type,  rooms[location]);
}
//We will now begin working on the game loop, now that the rooms are all set up(pray to fuck that we didn't miss an edge case of the rooms)
gameLoop(numRooms, rooms, pcLoc);

//free the memory of rooms
for(int i=0;i<numRooms;++i){
	free(rooms[i]);
}
return 0;
}

void gameLoop(int numRooms, struct Room *rooms[numRooms],int pcLoc){
//i would start with the look function as it seems to be the easiest to implement first
//this function will have a loop that will stop once the player enters the word 'exit'
int compValue = 1;
char input[80], *firstHalf = NULL, *secondHalf = NULL;
char separator[4] =  ":";

while(compValue != 0){
printf(">>>\n");
fgets(input, 80, stdin);
//this code will split the string that was entered if it contains : and will then make a creature do what the user specified
firstHalf = strtok(input, separator);
//printf("First half of input: %s\n", firstHalf);
//go through second half of input
secondHalf = strtok(0, firstHalf);
//printf("Second half of input: %s\n", secondHalf);


if(firstHalf == NULL || secondHalf == NULL){
if(strcmp(input, "help\n") == 0){
	help();
}

if(strcmp(input, "look\n") == 0){
        look(rooms[pcLoc], pcLoc);
}
//since the PC will potentially be moving to a different room, we need to create a removeCreatures function, as we can use the addAnimals function to add the PC to the room entered
if(strcmp(input, "north\n") == 0){
        if(rooms[pcLoc]->north == -1){
		printf("This room has no neighbors to the north\n");
	}
	else{
		removeCreature(0,rooms[pcLoc]);
		pcLoc = north(rooms[pcLoc], pcLoc);
		addCreature(0, rooms[pcLoc]);
	}}

if(strcmp(input, "east\n") == 0){
	if(rooms[pcLoc]->east == -1){
		printf("This room has no neighbors to the east\n");
	}
	else{
		removeCreature(0, rooms[pcLoc]);
		pcLoc = east(rooms[pcLoc], pcLoc);
		addCreature(0, rooms[pcLoc]);

	}
}
if(strcmp(input, "south\n") == 0){
	if(rooms[pcLoc]->south == -1){
		printf("This room has no neighbors to the south\n");
	}
	else{
		removeCreature(0, rooms[pcLoc]);
		pcLoc = south(rooms[pcLoc], pcLoc);
		addCreature(0, rooms[pcLoc]);

	}}
if(strcmp(input, "west\n") == 0){
	if(rooms[pcLoc]->west == -1){
		printf("This room has no neighbors to the west\n");
	}
	else{
		removeCreature(0, rooms[pcLoc]);
		pcLoc = west(rooms[pcLoc], pcLoc);
		addCreature(0, rooms[pcLoc]);

	}}
if(strcmp(input, "clean\n") == 0){
	clean(numRooms, rooms, pcLoc, 0, 0);
}
if(strcmp(input, "dirty\n") == 0){
	dirty(numRooms, rooms, pcLoc, 0, 0);
}
compValue = strcmp(input, "exit\n");
}
else{
if(strcmp(secondHalf, "clean\n") == 0){
printf("The creature %s will clean the room: %s\n", firstHalf, secondHalf);
int integer = atoi(firstHalf);
forcedClean(numRooms, rooms, integer, pcLoc);
}
if(strcmp(secondHalf, "dirty\n") == 0){
int number = atoi(firstHalf);
printf("The creature %s will dirty the room: %s\n", firstHalf, secondHalf);
forcedDirty(numRooms, rooms, number, pcLoc);
}	
if(strcmp(secondHalf, "north\n") == 0){
int number = atoi(firstHalf);
if(rooms[pcLoc]->north == -1)
	printf("There is no neighbor to the North, try a different command\n");
else
	forceNorth(numRooms, rooms, number, pcLoc);
}

if(strcmp(secondHalf, "east\n") == 0){
int number = atoi(firstHalf);

if(rooms[pcLoc]->east == -1)
	printf("There is no neighbor to the East, try a different command\n");
else
	forceEast(numRooms, rooms, number, pcLoc);

}

if(strcmp(secondHalf, "west\n") == 0){
int number = atoi(firstHalf);

if(rooms[pcLoc]->west == -1)
	printf("There is no neighbor to the West, try a different command\n");
else
	forceWest(numRooms, rooms, number, pcLoc);


}
if(strcmp(secondHalf, "south\n") == 0){
int number = atoi(firstHalf);

if(rooms[pcLoc]->south == -1)
	printf("There is no neighbor to the South, try a different command\n");
else
	forceSouth(numRooms, rooms, number, pcLoc);

}
if(strcmp(secondHalf, "help\n") == 0){
help();
}
if(strcmp(secondHalf, "look\n") == 0){
look(rooms[pcLoc], pcLoc);
}
if(strcmp(secondHalf, "exit\n") == 0){
printf("Have a good day\n");
exit(0);
}

}
}
}
//These are the command functions that allow the PC to move to a neighbor(as long as they exist)
int north(struct Room *rooms, int pcLoc){
	pcLoc = rooms->north;
	return pcLoc;
}
int east(struct Room *rooms, int pcLoc){
	pcLoc = rooms->east;
	return pcLoc;
}
int west(struct Room *rooms, int pcLoc){
	pcLoc = rooms->west;
	return pcLoc;
}
int south(struct Room *rooms, int pcLoc){
	pcLoc = rooms->south;
	return pcLoc;
}

//These will be when a creature is forced to move to a different room, 
void forceNorth(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc){
addCreature(rooms[pcLoc]->creatures[creature], rooms[rooms[pcLoc]->north]);
removeCreature(rooms[pcLoc]->creatures[creature], rooms[pcLoc]);
}

void forceEast(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc){
addCreature(rooms[pcLoc]->creatures[creature], rooms[rooms[pcLoc]->east]);
removeCreature(rooms[pcLoc]->creatures[creature], rooms[pcLoc]);
}

void forceWest(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc){
addCreature(rooms[pcLoc]->creatures[creature], rooms[rooms[pcLoc]->west]);
removeCreature(rooms[pcLoc]->creatures[creature], rooms[pcLoc]);
}

void forceSouth(int numRooms, struct Room *rooms[numRooms], int creature, int pcLoc){
addCreature(rooms[pcLoc]->creatures[creature], rooms[rooms[pcLoc]->south]);
removeCreature(rooms[pcLoc]->creatures[creature], rooms[pcLoc]);
}



//This is where we left off, format the output using if statments ya know how it is bra
void look(struct Room *rooms, int pcLoc){
//printf("This is inside the look fucntion\n");
printf("Room %d\n", pcLoc);
printf("Respect: %d\n", respect);
if(rooms->state == 0)
	printf("State: clean\n");
if(rooms->state == 1)
	printf("State: Half-dirty\n");
if(rooms->state == 2)
	printf("State: Dirty\n");
if(rooms->north != -1)
	printf("Neighbor %d to the North\n", rooms->north);
if(rooms->east != -1)
	printf("Neighbor %d to the East\n", rooms->east);
if(rooms->south != -1)
	printf("Neighbor %d to the South\n", rooms->south);
if(rooms->west != -1)
	printf("Neighbor %d to the West\n", rooms->west);
printf("The current room contains: \n");
for(int i=0;i<rooms->counter;++i){
if(rooms->creatures[i] == 0)
	printf("PC\n");// %d\n", rooms->creatures[i]);
//I changed this to only print out the place in the array, i think this will work(original was printf("Animal %d\n", rooms->creatures[i]);
if(rooms->creatures[i] == 1)
	printf("Animal: %d\n", i);
if(rooms->creatures[i] == 2)
	printf("NPC: %d\n", i);


}
}
/*****we left off here the remove function is not working properly *******/
//call this method whenever you need to remove a creature from a room
void removeCreature(int type, struct Room *rooms){
int location;
//Not sure if i need this if statement for validation, implement later if stucc gets messed up
for(int i=0;i<CREATURES_LENGTH;++i){
if(rooms->creatures[i] == type){
	//printf("This is the location of the PC(in this case) in the array: %d\n", rooms->creatures[i]);
	for(int j=i;j<CREATURES_LENGTH-1;j++){
		rooms->creatures[j] = rooms->creatures[j+1];
	}
	rooms->counter += -1;
	break;
}
}
}
void removeSpecificCreature(int placeInArray, struct Room *rooms){
//The user will enter a creature they want to remove, their numbers will start w 1, so we subtract 1 to get the right number, it will then remove the creature
for(int i=placeInArray;i<CREATURES_LENGTH-1;++i){
rooms->creatures[i] = rooms->creatures[i+1];
}
rooms->counter += -1;

}
//This function will remove all of the Animals from the room that is passed in
//We ended off here add if specific type of creature is in the room they have to leave to a random neighbor
void clean(int numRooms, struct Room *rooms[numRooms], int pcLoc, int isForced, int type){
	int neighbor=0, npcCounter=0;
	//this line of code calculates how many NPC's are in the room incase they are forced out for good
	for(int i=0; i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] ==1)
		npcCounter++;
	}
	if(rooms[pcLoc]->state == 0){
	printf("The room is already clean you fool\n");
	}	
	else if(rooms[pcLoc]->state == 1){
	rooms[pcLoc]->state = 0;
	printf("The room's state has been changed to clean\n");
	}
	else if(rooms[pcLoc]->state == 2){
	rooms[pcLoc]->state = 1;
	printf("The room's state has been changed to half-dirty\n");
	}

	//2 are NPC's, they like it dirty, so for every NPC in this room, they will grumble(respect -1)
	for(int i=0;i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] == 2){
	grumble(rooms[pcLoc]->creatures[i]);
	}
	}

	//1 is an animal, they like it clean, so for every animal in this room, they will lick your face(respect +1)
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	if(rooms[pcLoc]->creatures[i] == 1){
	lickFace(rooms[pcLoc]->creatures[i]);
	}
	}
	//we now have to add another part of the clean function, this will remove any creatures who cannot inhabit the room if it is clean(i.e. NPC's)
	if(rooms[pcLoc]->state == 0){
	//we see if the current pc's room has any neighbors, if it doesnt, we remove all the NPC's from the room
	neighbor = randomNeighbor(rooms[pcLoc]);
       	
	if(neighbor == -1){
        printf("You have forced these poor creatures out of the game for good, theyre very angry now\n");
	//removeCreature(1, rooms[pcLoc]);
	for(int i=0;i<npcCounter;++i){
	//change this for the largeGrumble function
	largeGrumble(2);
	removeCreature(2, rooms[pcLoc]);
	}
	}
	//if the room has neighbors, the NPC's will bebplaced randomly to the neighbors
	else{
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	//get a random neighbor
	//THIS WAS A NEW ADDITION IF IT DOESN'T WORK TAKE IT OUT
	if(rooms[pcLoc]->creatures[i] == 2){
	neighbor = randomNeighbor(rooms[pcLoc]);
	//add all of the creatures to the neighboring rooms
	addCreature(2, rooms[neighbor]);
	//remove all of the creatures from the cuurent room
	removeCreature(2, rooms[pcLoc]);
	}
	}
	}
	}	
	


}
//were adding another part to this function, that being if it is a forced action or not(we will pass an int that will be 0 if not forced and 1 if forced
void dirty(int numRooms, struct Room *rooms[numRooms], int pcLoc, int isForced, int type){
	int neighbor=0, animalCounter=0;
	//this line of code calsulates how many animals are in the room incase they are forced out and then we 3x the score if they are forced out
	for(int i=0;i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] == 1)
		animalCounter++;
	}
	//This is just changing the current rooms state to whatever it is plus a dirty action 	
	if(rooms[pcLoc]->state == 2){
	printf("The room is already dirty\n");
	}
	else if(rooms[pcLoc]->state == 1){
	rooms[pcLoc]->state = 2;
	printf("The room's state has been changed to dirty\n");
	}
	else if(rooms[pcLoc]->state == 0){
	rooms[pcLoc]->state = 1;
	printf("The room's state has been changed to half-dirty\n");
	}

	//2 are NPC's, they like it dirty, so for every NPC in this room, they will smile(respect +1)
	for(int i=0;i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] == 2){
	smile(rooms[pcLoc]->creatures[i]);
	}
	}

	//1 is an animal, they like it clean, so for every animal in this room, they will growl(respect -1)
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	if(rooms[pcLoc]->creatures[i] == 1){
	growl(rooms[pcLoc]->creatures[i]);
	}
	}
	
	//we now have to add another part of the clean function, this will remove any creatures who cannot inhabit the room if it is dirty(i.e. Animals)
	if(rooms[pcLoc]->state == 2){
	//see if the rooms has any neighbors(if randomNeighbor returns -1 there are no neighbors and the creatures have to be kicked out of the game
	neighbor = randomNeighbor(rooms[pcLoc]);
	
	if(neighbor == -1){
	printf("You have forced these poor creatures out of the game for good, theyre very angry now\n");
	//all of them are already removed, so you need to figure out the points loss if this occurs
	//removeCreature(1, rooms[pcLoc]);
	for(int i=0;i<animalCounter;++i){
	//Change this to the large growl function
	//respect = respect - 3;
	//printf("You really made the Animals angry...They all have to leave now. Respect: %d\n", respect);
	largeGrowl(1);
	removeCreature(1, rooms[pcLoc]);
	}
	}
	//if there exists any neighbors, the animals will be placed in rooms randomly to those neighbors
	else{
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	//gets a random neighbor
	if(rooms[pcLoc]->creatures[i] == 1){
	neighbor = randomNeighbor(rooms[pcLoc]);
	//add all the animals to the nieghboring rooms
	addCreature(1, rooms[neighbor]);
	//remove all of the animals from the current room;
	removeCreature(1, rooms[pcLoc]);
	}
	}
	}
	}
	
	//if the action is forced do the following code
}
void forcedDirty(int numRooms, struct Room *rooms[numRooms], int arraySlot, int pcLoc){
int neighbor=0, animalCounter=0;
	for(int i=0;i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] == 1)
		animalCounter++;
	}
	//This is just changing the current rooms state to whatever it is plus a dirty action 	
	if(rooms[pcLoc]->state == 2){
	printf("The room is already dirty\n");
	}
	else if(rooms[pcLoc]->state == 1){
	rooms[pcLoc]->state = 2;
	printf("The room's state has been changed to dirty\n");
	}
	else if(rooms[pcLoc]->state == 0){
	rooms[pcLoc]->state = 1;
	printf("The room's state has been changed to half-dirty\n");
	}
	//do large action based on creature that was forced to dirty room
	if(rooms[pcLoc]->creatures[arraySlot] == 1){
	largeGrowl(arraySlot);
	//if there are any animals other than the one forced to clean the room, they will react normally	
	for(int i=0;i<rooms[pcLoc]->counter && i != arraySlot;++i){
	if(rooms[pcLoc]->creatures[i] == 1)
	growl(rooms[pcLoc]->creatures[i]);
	}

	//
	for(int i=0;i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] == 2){
	smile(rooms[pcLoc]->creatures[i]);
	}
	}
	
	}
	//if an NPC is forced to clean a room, they will react with a largeGrumble
	else if(rooms[pcLoc]->creatures[arraySlot] == 2){
	largeSmile(arraySlot);
	
	for(int i =0; i<rooms[pcLoc]->counter && i != arraySlot; ++i){
	if(rooms[pcLoc]->creatures[arraySlot] == 2)
	smile(rooms[pcLoc]->creatures[arraySlot]);
	}

        for(int i=0;i<rooms[pcLoc]->counter; ++i){
	if(rooms[pcLoc]->creatures[i] == 1){
	growl(rooms[pcLoc]->creatures[i]);
	}
	}
	
	}
	//if the pc is forced to clean a room, nothing occurs in respect value
	else
		printf("The PC has cleaned the room\n");	


	
	//we now have to add another part of the clean function, this will remove any creatures who cannot inhabit the room if it is dirty(i.e. Animals)
	if(rooms[pcLoc]->state == 2){
	//see if the rooms has any neighbors(if randomNeighbor returns -1 there are no neighbors and the creatures have to be kicked out of the game
	neighbor = randomNeighbor(rooms[pcLoc]);
	
	if(neighbor == -1){
	printf("You have forced these poor creatures out of the game for good, theyre very angry now\n");
	//all of them are already removed, so you need to figure out the points loss if this occurs
	//removeCreature(1, rooms[pcLoc]);
	for(int i=0;i<animalCounter;++i){
	largeGrowl(1);
	removeCreature(1, rooms[pcLoc]);
	}
	}
	//if there exists any neighbors, the animals will be placed in rooms randomly to those neighbors
	else{
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	//gets a random neighbor
	if(rooms[pcLoc]->creatures[i] == 1){
	neighbor = randomNeighbor(rooms[pcLoc]);
	//add all the animals to the nieghboring rooms
	addCreature(1, rooms[neighbor]);
	//remove all of the animals from the current room;
	removeCreature(1, rooms[pcLoc]);
	}
	}
	}
	}

}
void forcedClean(int numRooms, struct Room *rooms[numRooms], int arraySlot, int pcLoc){
	int neighbor=0, npcCounter=0;
	for(int i=0; i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] ==2)
		npcCounter++;
	}

	//no matter what we need to clean the room(this stays consistent from clean to forcedClean)
	if(rooms[pcLoc]->state == 0){
	printf("The room is already clean you fool\n");
	}	
	else if(rooms[pcLoc]->state == 1){
	rooms[pcLoc]->state = 0;
	printf("The room's state has been changed to clean\n");
	}
	else if(rooms[pcLoc]->state == 2){
	rooms[pcLoc]->state = 1;
	printf("The room's state has been changed to half-dirty\n");
	}
	//if an animal is forced to clean a room, then they will react will a largeLickFace
	if(rooms[pcLoc]->creatures[arraySlot] == 1){
	largeLickFace(arraySlot);
	//for every animal BESIDES the animal that was forced to clean will lickFace
	for(int i =0;i<rooms[pcLoc]->counter && i != arraySlot;++i){
	if(rooms[pcLoc]->creatures[i] == 1)
	lickFace(rooms[pcLoc]->creatures[i]);
	}

	//for every NPC in the room, they will grumble at the action
	for(int i=0;i<rooms[pcLoc]->counter;++i){
	if(rooms[pcLoc]->creatures[i] == 2){
	grumble(rooms[pcLoc]->creatures[i]);
	}
	}

	}
	//if an NPC is forced to clean a room, they will react with a largeGrumble
	else if(rooms[pcLoc]->creatures[arraySlot] == 2){
	largeGrumble(arraySlot);
	//for every NPC in the room BESIDES the NPC that was forced to clean will react with a grumble
	for(int i=0;i<rooms[pcLoc]->counter && i != arraySlot;++i){
	if(rooms[pcLoc]->creatures[i] == 2)
	grumble(rooms[pcLoc]->creatures[i]);
	}


	//for every animal in the room, they like the action
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	if(rooms[pcLoc]->creatures[i] == 1){
	lickFace(rooms[pcLoc]->creatures[i]);
	}
	}

	}
	//if the pc is forced to clean a room, nothing occurs in respect value
	else
		printf("The PC has cleaned the room\n");	
	//if the state of the room is clean, then all of the NPC's need to leave
	if(rooms[pcLoc]->state == 0){
	//we see if the current pc's room has any neighbors, if it doesnt, we remove all the NPC's from the room
	neighbor = randomNeighbor(rooms[pcLoc]);
       	
	if(neighbor == -1){
        printf("You have forced these poor creatures out of the game for good, theyre very angry now\n");
	//removeCreature(1, rooms[pcLoc]);
	for(int i=0;i<npcCounter;++i){
	//change this for the largeGrumble function
	largeGrumble(2);
	removeCreature(2, rooms[pcLoc]);
	}
	}
	//if the room has neighbors, the NPC's will bebplaced randomly to the neighbors
	else{
	for(int i=0;i<rooms[pcLoc]->counter; ++i){
	//get a random neighbor
	//THIS WAS A NEW ADDITION IF IT DOESN'T WORK TAKE IT OUT
	if(rooms[pcLoc]->creatures[i] == 2){
	neighbor = randomNeighbor(rooms[pcLoc]);
	//add all of the creatures to the neighboring rooms
	addCreature(2, rooms[neighbor]);
	//remove all of the creatures from the cuurent room
	removeCreature(2, rooms[pcLoc]);
	}
	}
	}
	}	

}
void smile(int creature){
respect++;
printf("An NPC smiles, your respect is now %d\n",  respect);
if(respect >= 80){
printf("Youve won the game congrats!!!\n");
exit(0);
}
}
void lickFace(int creature){
respect++;
printf("An animal licks your face a lot, your respect is now %d\n", respect);
if(respect >= 80){
printf("Youve won the game congrats!!!\n");
exit(0);
}

}
void grumble(int creature){
respect--;
printf("An NPC grumbles, your respect is now %d\n", respect);
if(respect <= 0){
printf("Youve lost the game, how sad\n");
exit(0);
}

}
void growl(int creature){
respect--;
printf("An animal growls, your respect is now %d\n", respect);
if(respect <= 0){
printf("Youve lost the game, how sad\n");
exit(0);
}

}
void largeGrowl(int creature){
respect = respect -3;
printf("An animal was not happy with that, they growl loudly. Respect: %d\n", respect);
if(respect <= 0){
printf("You've lost the game\n");
exit(0);
}
}
void largeLickFace(int creature){
respect = respect +3;
printf("An animal was very happy with that, they lick all over your face. Respect: %d\n", respect);
if(respect >= 80){
printf("You've won the game congrats\n");
exit(0);
}
}
void largeSmile(int creature){
respect = respect +3;
printf("An NPC was very happy with that, they smile wide. Respect: %d\n", respect);
if(respect >= 80){
printf("You've won the game congrats\n");
exit(0);
}
}
void largeGrumble(int creature){
respect = respect -3;
printf("An NPC was not happy with that, they grumble loudly. Respect: %d\n", respect);
if(respect <= 0){
printf("You've lost the game\n");
exit(0);
}
}

int randomNeighbor(struct Room *rooms){
int neighbor=0, counter=0;
	if(rooms->north != -1)
		counter++;
	if(rooms->south != -1)
		counter++;
	if(rooms->east != -1)
		counter++;
	if(rooms->west != -1)
		counter++;
	if(counter == 0){
	//neighbors
	return -1;
	}
	else if(counter == 1){
	//this will force all of the animals to whatever the single neighbor is(make sure to add clause if rooms's creatures array is full
	if(rooms->north != -1)
		return rooms->north;
	else if(rooms->south != -1)
		return rooms->south;
	else if(rooms->east != -1)
		return rooms->east;
	else if(rooms->west != -1)
		return rooms->west;
	}
	else{
		if(rooms->south != -1)
			return rooms->south;
		else if(rooms->west != -1)
			return rooms->west;
		else if(rooms->north != -1)
			return rooms->north;
		else if(rooms->east != -1) 
			return rooms->east; 	
	}
return -1;	
}

//call this method whenever you need to add a creature to a room
void addCreature(int type, struct Room *room){
//The following code will change the state of the room a creature is moving into if it is uninhabitable for them(i.e. an Animal is moving into a dirty room
if(type == 1 && room->state == 2){
room->state = 1;
}	
else if(type == 2 && room->state == 0){
room->state =1;
}

//The following code just adds the creature to the room, and increments the rooms counter variable	
if(!room)
	return;
else{
	room->creatures[room->counter] = type;
	//room->creatures[1][room->counter] = id;
	room->counter += 1;
}
}


void help(){
printf("This is the help command, it will explain all of the rules of the game to you as well as all of the commands you can use the goal of the game is to(obviously win) get your respect to 80, and avoid losing(respect goes to 0) You will enter data to create your world, and then will travel the world as the PC, forcing creatures to clean and dirty rooms, pretty messed up but you chose to play this game so anyways, here is a list of commands that you can use to force these poor creatures to play this cruel game(also the commands are case-sensitive)\n");
printf("help: you can enter this command to display all of the commands you can use\nlook: you can enter this commmand to see various different things about the current room you are in, i.e. it shows you the rooms neighbors and the different kinds of animals inside it\nnorth/south...: you can enter a direction to travel and change rooms, if you enter a direction that doesn't exist, you wont be able to move there(obviously)\nclean: This command will clean a given room, it will raise your respect if there are more Animals in a given room than NPC's\ndirty: the same as the clean command but reversed\nexit: This command will allow you to leave this god-forsaken land\ncreatureType:command: this command will allow you to force a creature to do the any of the commands stated above, such as leave towards the east, or clean a given room, ex. (1:clean) would make creature 1 to clean the room, this command will have 3x the effect of the normal clean, in this case\n");

}

//this method should only be called in the beginning of the game to create the rooms and then left be
void makeRooms(struct Room *room, int state, int north, int east, int south, int west, int counter){
if(!room)
	return;
else{
	room->state = state;
	room->north = north;
	room->east = east;
	room->south = south;
	room->west = west;
	room->counter = 0;
}
}

