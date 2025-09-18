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
    int parent;          /* -1 if no parent */
    ChildNode *children; /* linked list of child indices */
} 
PCB;

/* Global: array of PCB pointers (NULL = free slot) */
static PCB *ptable[MAX_PROCESSES] = {0};

/* -------- utilities kept minimal -------- */
static int find_free_index(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (!ptable[i]) 
    		return i;
    return -1;
}

static void free_child_list(ChildNode *h) {
    while (h) { 
    	ChildNode *t = h->next; free(h); h = t; 
    }
}

static void print_process_list(void) {
    printf("\nProcess list:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (ptable[i]) {
        	printf("Process id: %d\n", i);
        if (ptable[i]->parent == -1) 
        	printf("No parent process\n");
        else                          
       		printf("Parent process: %d\n", ptable[i]->parent);
        if (!ptable[i]->children)     
        	printf("	No child processes\n");
        else {
            for (ChildNode *c = ptable[i]->children; c; c = c->next)
                printf("	Child process: %d\n", c->child);
        }
    }
}

/* destroy all descendants of p (NOT p). Also removes links from p */
static void destroy_descendants(int p) {
    if (!ptable[p]) return;
    ChildNode *c = ptable[p]->children;
    while (c) {
        int q = c->child;
        if (q >= 0 && q < MAX_PROCESSES && ptable[q]) {
            destroy_descendants(q);
            free_child_list(ptable[q]->children);
            free(ptable[q]);
            ptable[q] = NULL;
        }
        ChildNode *tmp = c;
        c = c->next;
        free(tmp);
    }
    ptable[p]->children = NULL;
}

static void free_all(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) 
    	if (ptable[i]) {
        free_child_list(ptable[i]->children);
        free(ptable[i]);
        ptable[i] = NULL;
    }
}

/* -------- menu actions with EXACT rubric text -------- */
static void initialize_process_hierarchy(void) {
    free_all();
    ptable[0] = (PCB *)malloc(sizeof(PCB));
    if (!ptable[0]) 
    	exit(1);
    ptable[0]->parent = -1;
    ptable[0]->children = NULL;
    print_process_list();
}

static void create_child(void) {
    int p;
    printf("\nEnter the parent process id: \n");
    if (scanf("%d", &p) != 1) 
    	return;
    if (p < 0 || p >= MAX_PROCESSES || !ptable[p]) 
    	return;

    int q = find_free_index();
    if (q == -1) 
    	return;

    ptable[q] = (PCB *)malloc(sizeof(PCB));
    if (!ptable[q]) 
    	exit(1);
    ptable[q]->parent = p;
    ptable[q]->children = NULL;

    ChildNode *node = (ChildNode *)malloc(sizeof(ChildNode));
    if (!node) 
    	exit(1);
    node->child = q; 
    node->next = NULL;

    if (!ptable[p]->children) 
    	ptable[p]->children = node;
    else {
        ChildNode *t = ptable[p]->children;
        while (t->next) t = t->next;
        t->next = node;
    }

    print_process_list();
}

static void destroy_descendants_prompt(void) {
    int p;
    printf("\nEnter the parent process whose descendants are to be destroyed: ");
    if (scanf("%d", &p) != 1) 
    	return;
    if (p < 0 || p >= MAX_PROCESSES || !ptable[p]) 
    	return;

    destroy_descendants(p);
    print_process_list();
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
        printf("Process creation and destruction\n");
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
            case 1: initialize_process_hierarchy(); 
            	break;
            case 2: create_child();                 
            	break;
            case 3: destroy_descendants_prompt();   
            	break;
            case 4:
                printf("Quitting program... ");
                quit_program();
                break;
        }
    } while (choice != 4);

    return 0;
}
