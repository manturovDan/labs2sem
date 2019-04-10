//agree that key compares in spaces by min<=x<=max
//SIZE of side of quadrant, keys are from -SIZE div 2 to SIZE div(top top)2 - 1 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.c"

int get_nInt(int *a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n == 0) {
            printf("Incorrect input. Try again!\n");
            scanf("%*f^\n");
            scanf("%*s");
            continue;
        }

        if (*a < 0 || *a > 100000000) {
            printf("Input natural number <= 100M\n");
            n = 0;
        }
    } while(n == 0);

    scanf("%*1[\n]");
    return n < 0 ? 0 : 1;
}

char *getStr() {
    char buf[81];
    char *ptr = (char *)calloc(1, sizeof(char));
    if (ptr == NULL) {
        printf("No memory");
        exit(0);
    }
    short int n = 0;
    int len = 0;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
        }
        else if (n == 0) {
            scanf("%*1[\n]");
        }
        else {
            len += strlen(buf);
            ptr = (char*)realloc(ptr, len+1);
            if (ptr == NULL) {
                printf("No memory");
                exit(0);
            }
            strcat(ptr, buf);
        }

    } while (n > 0);

    return ptr;
}

const char *msgs[] = {"Menu:\n0. Quit", "1. Add element to tree",
                        "2. Find element", "3. Delete element", 
                        "4. Show tree"};

int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int D_Create(),
    D_Add(),
    D_Find(),
    D_Delete(),
    D_Show();

int (*fptr[])() = { NULL, D_Add, D_Find, D_Delete, D_Show };

int D_Add() {
    printf("test\n");
}

int D_Find() {
    printf("test\n");
}

int D_Delete() {
    printf("test\n");
}

int D_Show() {
    printf("test\n");
}

int dialog(const char *msgs[], int N) {
    char *errmsg = "";
    int rc;
    int i, n;

    do {
        puts(errmsg);
        errmsg = "You are wrong. Repeat please!\n";

        for (i = 0; i < N; i++) {
            puts(msgs[i]);
        }
        puts("Choose the operation:");

        n = get_nInt(&rc);

        if (n == 0) {
            rc = 0;
        }

    } while (rc < 0 || rc >= N);

    return rc;
}

int main () {
    int rc, n;

    while (1) {
        printf("This is QUADtree program.\n\
Insert 1 to create new tree or insert 0 to load free from file:\n");
        
        n = get_nInt(&rc);
        if (rc == 1) {
            printf("Input SIZE of QUADRANT side.\n\
Keys are from -SIZE div 2 to SIZE div(top top)2 - 1\n");
            int size, capacity;
            get_nInt(&size);
            printf("Input capacity of one quadrant:\n");
            get_nInt(&capacity);

            if (size == 0 || capacity == 0) 
                continue;

            Quadrant *root = create(size, capacity);

            break;

        }
        else {
            break;
        }
    }

    while (rc = dialog(msgs, NMsgs)) {
        fptr[rc]();
    }
}