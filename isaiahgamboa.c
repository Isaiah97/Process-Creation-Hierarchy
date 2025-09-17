#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 64

typedef struct ChildNode {
    int child;
    struct ChildNode *next;
} ChildNode;

typedef struct PCB {
    int parent;          /* -1 for no parent */
    ChildNode *children; /* linked list of child indices */
} PCB;

/* Global table of PCB pointers; NULL = free slot */
static PCB *ptable[MAX_PROCESSES] = {0};

/* --------- Small utilities --------- */
static int find_free_index(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) if (!ptable[i]) return i;
    return -1;
}

static void free_child_list(ChildNode *h) {
    while (h) { ChildNode *t = h->next; free(h); h = t; }
}

static void print_process_list(void) {
    printf("Process list:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) if (ptable[i]) {
        printf("Process id: %d\n", i);
        if (ptable[i]->parent == -1) printf("No parent process\n");
        else                          printf("Parent process: %d\n", ptable[i]->parent);
        if (!ptable[i]->children)     printf("No child processes\n");
        else {
            for (ChildNode *c = ptable[i]->children; c; c = c->next)
                printf("Child process: %d\n", c->child);
        }
    }
}

/* Recursively destroy descendants of p (NOT p itself), and remove links from p */
static void destroy_descendants(int p) {
    if (!ptable[p]) return;
    ChildNode *c = ptable[p]->children;
    while (c) {
        int q = c->child;
        if (q >= 0 && q < MAX_PROCESSES && ptable[q]) {
            destroy_descendants(q);               /* destroy q's subtree first */
            free_child_list(ptable[q]->children); /* free q's child list */
            free(ptable[q]);                      /* free PCB[q] */
            ptable[q] = NULL;
        }
        ChildNode *tmp = c;                       /* free the link node in p */
        c = c->next;
        free(tmp);
    }
    ptable[p]->children = NULL;
}

/* Free everything (used by Initialize and Quit) */
static void free_all(void) {
    for (int i = 0; i < MAX_PROCESSES; ++i) if (ptable[i]) {
        free_child_list(ptable[i]->children);
        free(ptable[i]);
        ptable[i] = NULL;
    }
}

/* --------- Menu actions (simple) --------- */
static void initialize_process_hierarchy(void) {
    free_all();
    ptable[0] = (PCB *)malloc(sizeof(PCB));
    ptable[0]->parent = -1;
    ptable[0]->children = NULL;
    print_process_list();
}

static void create_child(void) {
    int p;
    printf("Enter the parent process id: ");
    if (scanf("%d", &p) != 1) return;
    if (p < 0 || p >= MAX_PROCESSES || !ptable[p]) return;

    int q = find_free_index();
    if (q == -1) return;

    ptable[q] = (PCB *)malloc(sizeof(PCB));
    ptable[q]->parent = p;
    ptable[q]->children = NULL;

    /* append q to p's children (at tail to match sample ordering) */
    ChildNode *node = (ChildNode *)malloc(sizeof(ChildNode));
    node->child = q; node->next = NULL;
    if (!ptable[p]->children) ptable[p]->children = node;
    else {
        ChildNode *t = ptable[p]->children;
        while (t->next) t = t->next;
        t->next = node;
    }

    print_process_list();
}

static void destroy_descendants_prompt(void) {
    int p;
    printf("Enter the parent process whose descendants are to be destroyed: ");
    if (scanf("%d", &p) != 1) return;
    if (p < 0 || p >= MAX_PROCESSES || !ptable[p]) return;

    destroy_descendants(p);
    print_process_list();
}

static void quit_program(void) {
    free_all();
}

/* --------- Main --------- */
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
            /* flush bad input */
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        switch (choice) {
            case 1: initialize_process_hierarchy(); break;
            case 2: create_child();                 break;
            case 3: destroy_descendants_prompt();   break;
            case 4:
                printf("Quitting program... ");
                quit_program();
                printf("here is the rules\n");
                break;
            default: /* ignore invalid per rubric */ break;
        }
    } while (choice != 4);

    return 0;
}
