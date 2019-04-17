#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.c"
#include <time.h>

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


int main () {
	int n, steps, size, capacity, countEls;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));

	while (1) {
		printf("This is program for timing QUADtree.\nInput size of testing tree:\n");
		n = get_nInt(&size);
		printf("Input capacity of one Quadrant:\n");
		n = get_nInt(&capacity);
		printf("Input count of added and searched elements:\n");
		n = get_nInt(&countEls);
		printf("Input count of checks:\n");
		n = get_nInt(&steps);

		if (size == 0 || capacity == 0 || steps == 0) 
            continue;

        *root = create(size, capacity);
        printf("New tree was created\n");
        break;
	}

	clock_t first, last;
	srand(time(NULL));

	while(steps-- > 0) {
		int added = 0;
		for (int i = 0; i < countEls; i++) {
			int x, y;
			if (rand() % 2 == 0) //minus
				x = - (rand() % size/2);
			else 
				x = (rand() % size / 2) - 1;
			if (rand() % 2 == 0) //plus
				y = - (rand() % size/2);
			else 
				y = (rand() % size / 2) - 1;

			char *testStr = (char *)calloc(4, sizeof(char));
			strcpy(testStr, "test");

			if (add_el(*root, x, y, capacity, testStr) == 0) {
		    	added++;
		    }
		    else {
		    	free(testStr);
		    }
		}

		first = clock();

		int found = 0;
		for (int i = 0; i < countEls; i++) {
			int x, y;
			if (rand() % 2 == 0) //minus
				x = - (rand() % size/2);
			else 
				x = (rand() % size / 2) - 1;
			if (rand() % 2 == 0) //plus
				y = - (rand() % size/2);
			else 
				y = (rand() % size / 2) - 1;

			SearchRes result = search(*root, x, y, capacity);
			if(result.owner != NULL)
				found++;
		}

		last = clock();

		printf("Test: %d; Added: %d / %d; Time: %ld; Found elements: %d / %d\n\n", steps, added, countEls, last - first, found, countEls);
	}

	delTree(*root, size, capacity);
	free(root);
}