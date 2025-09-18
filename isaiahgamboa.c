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

#define MAX_PROCESSES 256 //wanted to use 256

typedef struct ChildNode {
    int child;
    struct ChildNode *next;
} 
ChildNode;

typedef struct PCB {
    int parent;          // -1 if no parent 
    ChildNode *children; // linked list of child indices 
} 
PCB; //call PCB

// need an array of PCB pointers
static PCB *pcbtable[MAX_PROCESSES] = {0};


static int find_free_index(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (!pcbtable[i]) 
    		return i;
    return -1;
}

static void free_child_list(ChildNode *h) {
    while (h) { 
    	ChildNode *t = h->next; 
    	free(h); h = t; 
    }
}


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

/* destroy all descendants of p (NOT p). Also removes links from p */
static void destroy_descendants(int p) {
    if (!pcbtable[p]) return;
    ChildNode *c = pcbtable[p]->children;
    while (c) {
        int q = c->child;
        if (q >= 0 && q < MAX_PROCESSES && pcbtable[q]) {
            destroy_descendants(q);
            free_child_list(pcbtable[q]->children);
            free(pcbtable[q]);
            pcbtable[q] = NULL;
        }
        ChildNode *tmp = c;
        c = c->next;
        free(tmp);
    }
    pcbtable[p]->children = NULL;
}

static void free_all(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (pcbtable[i]) {
        free_child_list(pcbtable[i]->children);
        free(pcbtable[i]);
        pcbtable[i] = NULL;
    }
}

/* -------- menu actions with EXACT rubric text -------- */
static void initialize(void) {
    free_all();
    pcbtable[0] = (PCB *)malloc(sizeof(PCB));
    if (!pcbtable[0]) 
    	exit(1);
    pcbtable[0]->parent = -1;
    pcbtable[0]->children = NULL;
    process_list();
}

static void create_child(void) {
    int p;
    printf("\n\nEnter the parent process id: \n");
    if (scanf("%d", &p) != 1) 
    	return;
    if (p < 0 || p >= MAX_PROCESSES || !pcbtable[p]) 
    	return;

    int q = find_free_index();
    if (q == -1) 
    	return;

    pcbtable[q] = (PCB *)malloc(sizeof(PCB));
    if (!pcbtable[q]) 
    	exit(1);
    pcbtable[q]->parent = p;
    pcbtable[q]->children = NULL;

    ChildNode *node = (ChildNode *)malloc(sizeof(ChildNode));
    if (!node) 
    	exit(1);
    node->child = q; 
    node->next = NULL;

    if (!pcbtable[p]->children) 
    	pcbtable[p]->children = node;
    else {
        ChildNode *t = pcbtable[p]->children;
        while (t->next) t = t->next;
        t->next = node;
    }

    process_list();
}

static void destroy_descendants(void) {
    int p;
    printf("\nEnter the parent process whose descendants are to be destroyed: ");
    if (scanf("%d", &p) != 1) 
    	return;
    if (p < 0 || p >= MAX_PROCESSES || !pcbtable[p]) 
    	return;

    destroy_descendants(p);
    process_list();
}

static void quit_program(void) {
    free_all();
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
            case 3: destroy_descendants();   
            	break;
            case 4:
                printf("Quitting program... ");
                quit_program();
                break;
        }
    } while (choice != 4);

    return 0;
}
