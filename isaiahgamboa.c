#include <stdio.h>


void Initialization(){



}

void Creation(){


}

void Destruction(){


}

void Exit(){


}



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
			print("Program terminated\n")
		}
	}while (choice != 4);

	return 0;
}
