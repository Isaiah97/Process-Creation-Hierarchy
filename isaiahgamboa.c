#include <stdio.h>
#include <stdlib.h>

//need an array
#define MAX_PROCESSES 256

typedef struct ChildNode {
	int child;
	struct ChildNode *next;
}
ChildNode;

typedef struct PCB {
	int parent;
	ChildNode *child;
}
PCB;

static PCB *pcbtable[MAX_PROCESSES] = {0};

static int free_index(void) {
	int i;

	for (i = 0; i < MAX_PROCESSES; ++i)
		if (!pcbtable[i])
			return i;
		return -1;
}

void Initialization(){


printf("\nProcess List: \n");
printf("Process id: ");

}

void Creation(){


}

void Destruction(){


}

void Exit(){


}


// code the main table with 4 options
// Initailize
// Creation
// Destroy
// Quit
// I also need a selector for each category
int main(){
	int choice;

	do{
	printf("\nProcess creation and destruction\n");
	printf("--------------------------------\n");
	printf(" 1) Initilize process hierarchy\n");
	printf(" 2) Create a new child process\n");
	printf(" 3) Destroy all descendants of a parent process\n");
	printf(" 4) Quit program and free memory\n");
	printf("Enter selection: ");
        scanf("%d", &choice);

	// having cases would be best bet
	switch (choice) {  
	
		case 1:
			Initialization();
			break;
		case 2:
			Creation();
			break;
		case 3:
			Destruction();
			break;
		case 4:
			printf("Program terminated\n");
			break;
		}
	
	}while (choice != 4);

	return 0;
}
