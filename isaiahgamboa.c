//Create a process creation hierarchy as an array of length MAX_PROCESSES which references process control blocks (PCBs), indexed 0 to MAX_PROCESSES-1.
//Each PCB is a structure consisting of two fields:
//parent: a PCB index corresponding to the process’ creator
//children: a pointer to a linked list, where each node contains the PCB index of one child process and a link to the next child in the linked list

//The necessary functions are simplified as follows:
//create_child() represents the create function, which prompts for the parent process p. The function creates a new child process q of process p by performing the following tasks:
//allocate memory for an unused PCB[q]
//record the parent's index, p, in PCB[q]
//initialize the list of children of PCB[q] as empty (NULL)
//create a new link containing the child's index q and append the link to the children field of PCB[p]
//destroy_descendants() represents the destroy function, which prompts for the parent process p. The function recursively destroys all descendent processes (child, grandchild, etc.) of process p by performing the following tasks: for each element q on the linked list of children of p:
//destroy_desecndants(q)	(recursively destroy all descendants of q)
//free memory utilized by PCB[q] and set it to NULL
//Free memory utilized by the node with id q and set it to NULL


#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 256 //wanted to use 256 for bigger cases

typedef struct ChildNode {
    int child;
    struct ChildNode *next;
} 
ChildNode;

typedef struct PCB {
    int parent;          // -1 if no parent 
    ChildNode *children; // linked list of child
} 
PCB; //call PCB

// need an array of PCB pointers
static PCB *pcbtable[MAX_PROCESSES] = {0};


static int free_index(void) { //need to find a free index in the pcbtable
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (!pcbtable[i]) 
    		return i;
    return -1;
}

static void free_child(ChildNode *h) { //need to have a linked list for child
    while (h) { 
    	ChildNode *t = h->next; 
    	free(h); h = t; 
    }
}
//reminder to self: make a prompt for outputs that is not a part of the selector prompt
// need a process list prompt
static void process_list(void) {
    printf("\nProcess list:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (pcbtable[i]) {
        	printf("Process id: %d\n", i);
        if (pcbtable[i]->parent == -1) 
        	printf("\tNo parent process\n");
        else                          
       		printf("Parent process: %d\n", pcbtable[i]->parent);
        if (!pcbtable[i]->children)     
        	printf("\tNo child processes\n");
        else {
            for (ChildNode *c = pcbtable[i]->children; c; c = c->next)
                printf("\tChild process: %d\n", c->child);
        }
    }
}
// want to knock out destroy descendants first
static void destroy_descendants(int p) { 
    if (!pcbtable[p]) //case handling
    	return;
    ChildNode *c = pcbtable[p]->children; //allows p to continue 
    while (c) {
        int q = c->child; //destroys and rsets q free
        //finally got to set q free
        if (q >= 0 && q < MAX_PROCESSES && pcbtable[q]) {
            destroy_descendants(q);
            free_child(pcbtable[q]->children);
            free(pcbtable[q]); //free can eliminate q
            pcbtable[q] = NULL;
        }
        ChildNode *tmp = c;
        c = c->next;
        free(tmp);
    }
    pcbtable[p]->children = NULL;
}
//had free time so i can just terminate the pcbtable
static void free_all(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (pcbtable[i]) {
        free_child(pcbtable[i]->children);
        free(pcbtable[i]);
        pcbtable[i] = NULL;
    }
}

static void initialize(void) {
    free_all();  //empty table
    pcbtable[0] = (PCB *)malloc(sizeof(PCB)); //found 
    if (!pcbtable[0]) 
    	exit(1);
    //need to have id parent = 0, no child would exist
    pcbtable[0]->parent = -1;
    pcbtable[0]->children = NULL;
    process_list();
}

//need create child class
static void create_child(void) {
    int p;
    printf("Enter the parent process id: \n");
    if (scanf("%d", &p) != 1) //little bit of error handling example bad input or invalid parent
    	return;
    if (p < 0 || p >= MAX_PROCESSES || !pcbtable[p]) 
    	return;
//in case if the table is full
    int q = free_index();
    if (q == -1) 
    	return;
//make sure to use q as child pcb index
    pcbtable[q] = (PCB *)malloc(sizeof(PCB));
    if (!pcbtable[q]) 
    	exit(1);
    pcbtable[q]->parent = p;
    pcbtable[q]->children = NULL;
//q and append the link to the children field of PCB[p]
    ChildNode *node = (ChildNode *)malloc(sizeof(ChildNode));
    if (!node) 
    	exit(1);
    node->child = q; 
    node->next = NULL;
//need to have the first child
    // make sure the child follows thru at the end
    if (!pcbtable[p]->children) 
    	pcbtable[p]->children = node;
    else {
        ChildNode *t = pcbtable[p]->children;
        while (t->next) t = t->next;
        t->next = node;
    }

    process_list();
}
//need to have selection 3 have an operational prompt
static void destroy_descendants_table(void) {
    int p;
    printf("\nEnter the parent process whose descendants are to be destroyed: ");
    if (scanf("%d", &p) != 1) 
    	return;
    if (p < 0 || p >= MAX_PROCESSES || !pcbtable[p]) 
    	return;

    destroy_descendants(p);
    process_list();
}

//option 4 to just free everything
static void quit_program(void) {
    free_all(); //had all the work done before hand
}

//1) Initialize process hierarchy
//2) Create a new child process 
//3) Destroy all descendants of a parent process 
//4) Quit program and free memory

int main(void) {
    int choice;
    do {
        printf("\n\nProcess creation and destruction\n");
        printf("--------------------------------\n");
        printf("1) Initialize process hierarchy\n");
        printf("2) Create a new child process\n");
        printf("3) Destroy all descendants of a process\n");
        printf("4) Quit program and free memory\n");
        printf("\n Enter selection: ");

        if (scanf("%d", &choice) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        switch (choice) {
            case 1: initialize(); 
            	break;
            case 2: create_child();                 
            	break;
            case 3: destroy_descendants_table();   
            	break;
            case 4:
                printf("Quitting program... ");
                quit_program();
                break;
        }
    } while (choice != 4);

    return 0;
}
