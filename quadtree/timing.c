#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.c"
//#include <time.h>

int get_Int(int *a, int nat) {
    int n;
    do {
        n = scanf("%d", a);
        if (n == 0) {
            printf("Incorrect input. Try again!\n");
            scanf("%*f^\n");
            scanf("%*s");
            continue;
        }

        if (*a > 100000000) {
            printf("Input number <= 100M\n");
            n = 0;
        }

        if(nat == 1 && *a < 0) {
        	printf("Input natural number\n");
            n = 0;
        }

    } while(n == 0);

    scanf("%*1[\n]");
    return n < 0 ? 0 : 1;
}

int get_nInt(int *a) {
    return get_Int(a, 1);
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

int main () {
	int n, steps, size, capacity;
	Quadrant **root;

	while (1) {
		printf("This is program for timing QUADtree.\nInput size of testing tree:\n");
		n = get_nInt(&size);
		printf("Input capacity of one Quadrant:\n");
		n = get_nInt(&capacity);
		printf("Input count of checks:\n");
		n = get_nInt(&steps);

		if (size == 0 || capacity == 0 || steps == 0) 
            continue;

        *root = create(size, capacity);
        printf("New tree was created\n");
        break;
	}

	/*clock_t first, last;
	srand(time(NULL));*/

	while(steps-- > 0) {
		int added = 0;
		for (int i = 0; i < size; i++) {
			int x, y;
			if (rand() % 2 == 0) { //minus
				x = - (rand() % size/2);
			}
			else {
				x = (rand() % size / 2) - 1;
			}

			if (rand() % 2 == 0) { //minus
				y = - (rand() % size/2);
			}
			else {
				y = (rand() % size / 2) - 1;
			}

			//char test[] = "test";
			char *testStr = (char *)calloc(5, sizeof(char));
			strcpy(testStr, "test");
			printf("[%d, %d]", x, y);

			//if (add_el(*root, x, y, capacity, testStr) == 0) {
		    //	added++;
		    //}
		}

		//first = clock();

		printf("Test: %d; Added: %d; Time: \n", steps, added);
	}
}