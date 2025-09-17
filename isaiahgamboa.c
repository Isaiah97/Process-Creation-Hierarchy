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

static int index(void) {
	int i;

	for (i = 0; i < MAX_PROCESSES; ++i)
		if (!pcbtable[i])
			return i;
		return -1;
}

static void free_child(ChildNode *head) {
	while (head) { 
		ChildNode *tmp = h->next;
		free(head);
		head = tmp;
	}
}

/* Append child index q to parent p's children list (at tail to preserve order) */
static void append_child_link(int p, int q) {
    ChildNode *node = (ChildNode *)malloc(sizeof(ChildNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed while adding child link.\n");
        exit(1);
    }
    node->child = q;
    node->next = NULL;

    if (ptable[p]->children == NULL) {
        ptable[p]->children = node;
    } else {
        ChildNode *cur = ptable[p]->children;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
}

/* Find the lowest unused PCB index (slot is NULL). Return -1 if none. */
static int find_free_index(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (ptable[i] == NULL) return i;
    }
    return -1;
}

/* Print the process list in the style shown in the sample output */
static void print_process_list(void) {
    printf("Process list:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (ptable[i] != NULL) {
            printf("Process id: %d\n", i);
            if (ptable[i]->parent == -1) {
                printf("No parent process\n");
            } else {
                printf("Parent process: %d\n", ptable[i]->parent);
            }
            if (ptable[i]->children == NULL) {
                printf("No child processes\n");
            } else {
                ChildNode *cur = ptable[i]->children;
                while (cur) {
                    printf("Child process: %d\n", cur->child);
                    cur = cur->next;
                }
            }
        }
    }
}

/* Recursively destroy ALL descendants of p (child, grandchild, ...), but NOT p itself. */
static void destroy_descendants(int p) {
    if (ptable[p] == NULL) return;

    /* Walk the children list; for each q: recursively destroy q's descendants, then free PCB[q] and its list node. */
    ChildNode *cur = ptable[p]->children;
    while (cur) {
        int q = cur->child;

        /* Recurse into q first (if q exists) */
        if (q >= 0 && q < MAX_PROCESSES && ptable[q] != NULL) {
            destroy_descendants(q);              /* destroy q's descendants */
            /* Now free PCB[q] itself */
            free_children_list(ptable[q]->children);
            free(ptable[q]);
            ptable[q] = NULL;
        }

        /* Advance while freeing the link node from p's list */
        ChildNode *to_free = cur;
        cur = cur->next;
        free(to_free);
    }

    /* After destroying all children, set p's children to NULL (no children left). */
    ptable[p]->children = NULL;
}

/* Clean up everything (used for Quit and re-initialization safety) */
static void free_all(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (ptable[i] != NULL) {
            free_children_list(ptable[i]->children);
            free(ptable[i]);
            ptable[i] = NULL;
        }
    }
}

/* ------------ Menu actions ------------ */
static void initialize_process_hierarchy(void) {
    /* Reset everything and create process 0 with no parent and no children */
    free_all();

    PCB *root = (PCB *)malloc(sizeof(PCB));
    if (!root) {
        fprintf(stderr, "Memory allocation failed during initialization.\n");
        exit(1);
    }
    root->parent = -1;   /* no parent */
    root->children = NULL;
    ptable[0] = root;

    print_process_list();
}

static void create_child(void) {
    int parent;
    printf("Enter the parent process id: ");
    if (scanf("%d", &parent) != 1) return;

    if (parent < 0 || parent >= MAX_PROCESSES || ptable[parent] == NULL) {
        /* Parent does not exist; silently ignore or print nothing per sample. */
        /* You can add an error line here if your grader allows extra output. */
        return;
    }

    /* Find an unused PCB index q */
    int q = find_free_index();
    if (q == -1) {
        /* Table full; nothing in sample, so just return */
        return;
    }

    /* Allocate PCB[q] */
    PCB *child = (PCB *)malloc(sizeof(PCB));
    if (!child) {
        fprintf(stderr, "Memory allocation failed while creating child.\n");
        exit(1);
    }
    child->parent = parent;
    child->children = NULL;
    ptable[q] = child;

    /* Append child's index to parent's children list */
    append_child_link(parent, q);

    print_process_list();
}

static void destroy_descendants_prompt(void) {
    int parent;
    printf("Enter the parent process whose descendants are to be destroyed: ");
    if (scanf("%d", &parent) != 1) return;

    if (parent < 0 || parent >= MAX_PROCESSES || ptable[parent] == NULL) {
        /* Parent doesn't exist; follow sample style: no extra error prints required */
        return;
    }

    destroy_descendants(parent);
    print_process_list();
}

static void quit_program(void) {
    /* Free memory and exit after loop prints message */
    free_all();
}

/* ------------ Main ------------ */
int main(void) {
    int choice;

    do {
        printf("Process creation and destruction\n");
        printf("--------------------------------\n");
        printf("1) Initialize process hierarchy\n");
        printf("2) Create a new child process\n");
        printf("3) Destroy all descendants of a process\n");
        printf("4) Quit program and free memory\n");
        printf("Enter selection: ");

        if (scanf("%d", &choice) != 1) {
            /* If bad input, consume and continue */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            continue;
        }

        switch (choice) {
            case 1:
                initialize_process_hierarchy();
                break;
            case 2:
                create_child();
                break;
            case 3:
                destroy_descendants_prompt();
                break;
            case 4:
                printf("Quitting program... ");
                quit_program();
                printf("here is the rules\n");
                break;
            default:
                /* Keep menu numbers untouched; ignore invalids */
                break;
        }
    } while (choice != 4);

    return 0;
}
//void intitalization(){


//printf("\nProcess List: \n");
//printf("Process id: ");

//}

//void create_child(){


//}

//void destroy_descendants(){


//}

//void Exit(){


//}


// code the main table with 4 options
// Initailize
// Creation
// Destroy
// Quit
// I also need a selector for each category
//int main(){
	//int choice;
//
	//do{
	//printf("\nProcess creation and destruction\n");
	//printf("--------------------------------\n");
	//printf(" 1) Initilize process hierarchy\n");
	//printf(" 2) Create a new child process\n");
	//printf(" 3) Destroy all descendants of a parent process\n");
	//printf(" 4) Quit program and free memory\n");
	//printf("Enter selection: ");
     //   scanf("%d", &choice);

	// having cases would be best bet
	//switch (choice) {  
	
		//case 1:
			//intitalization();
			//break;
		//case 2:
		//	create_child();
		//	break;
		//case 3:
		//	destroy_descendants();
		//	break;
		//case 4:
		//	printf("\nQuitting program ...");
		//	break;
		//}
	
	//}while (choice != 4);

	//return 0;
//}
