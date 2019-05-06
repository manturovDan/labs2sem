#define SIZE 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.c"

int get_Num(int *a, int nat) { //nat 1 if need natural number (>= 0)
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

int get_Int(int *a) {
	return get_Num(a, 0);
}

int get_nInt(int *a) {
    return get_Num(a, 1);
}

int get_Float(float *a, int bez) { // bez 1 if we need >= number
	int n;
    do {
        n = scanf("%f", a);

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

        if(bez == 1 && *a < 0) {
            printf("Input natural number\n");
            n = 0;
        }

    } while(n == 0);

    scanf("%*1[\n]");
    return n < 0 ? 0 : 1;
}

int D_Add(GraphClutch *gTab) {
	int res, name;
	float x, y;
	printf("Input name (int) of Graph node\n");
	res = get_nInt(&name);
	printf("Input x-coord (float) of new node\n");
	res *= get_Float(&x, 0);
	printf("Input y-coord (float) of new node\n");
	res *= get_Float(&y, 0);
	if (res == 0) {
		printf("\nError.Try again\n");
		return 1;
	}

	printf("Adding Node\n");
	return adding(gTab, name, x, y);
}

int D_Show(GraphClutch *gTab) {
	int emp = 0;
	for (int i = 0; i < SIZE; i++) {
		CluItem *cont = gTab->grTab[i];
		if(cont != NULL) {
			printf("(%d)\n", i);
			while(cont != NULL) {
				printf("%d (%f; %f)", cont->fNode->name, cont->fNode->x, cont->fNode->y);
				Neighbour *nbr = cont->fNode->nbr;
				while (nbr != NULL) {
					printf(" --%f-->(%d) ", nbr->dist, nbr->name);
					nbr = nbr->next;
				} 
				printf("\n");
				cont = cont->next;
			}
			emp = 1;
		}
	}

	if (emp == 0) {
		printf("Graph is empty\n");
		return 1;
	}

	return 0;
}

int D_Add_Edge(GraphClutch *gTab) {
	int res, from, to;
	printf("Input name of starting node\n");
	res = get_nInt(&from);
	printf("Input name of ending node\n");
	res *= get_nInt(&to);
	if (res == 0) {
		printf("Error\n");
		return 1;
	}
	int eres = edge(gTab, from, to);

	if (eres == 1) {
		printf("The way is already exist\n");
		return 1;
	}
	else if (eres == 2) {
		printf("Error! There aren\'t these nodes in Graph\n");
		return 1;
	}

	return 0;

}

int main() {
	char *msgs[] = {"0. Quit", "1. Add node to Graph", "2. Add edge to Graph", 
					"3. Delete node from Graph",
					"4. Find 3 shortest ways between all nodes (Floyd–Warshall algorithm)",
					"5. Print Graph", "6. Save Graph in file"};


	while (1) {
		printf("This is Graph program. Choose one option\n1. Create Graph by hand\n2. Generate graph\n3. Load Graph from file\n");
		int mode;
		if (get_nInt(&mode) == 0) {
			printf("Error. Try again\n");
			continue;
		}
		else if(mode == 1) {
			printf("Create your graph\n");
			break;
		}
		else if(mode == 2) {
			printf("Random graph\n");
			break;
		}
		else if(mode == 3) {
			printf("Load graph\n");
			break;
		}
		else {
			printf("Input correct value\n");
			continue;
		}

	}
    
    GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int mItem;
    int res;
    while (1) {
    	for (int i = 0; i < sizeof(msgs) / sizeof(msgs[0]); i++) {
    		printf("%s\n", msgs[i]);
    	}

    	if (get_nInt(&mItem) == 0) {
    		printf("\nError.Try again\n");
    		continue;
    	}

    	if (mItem == 0)
    		break;
    	else if (mItem == 1) {
    		res = D_Add(&gTab);
    		if (res == 2) {
    			printf("Element is exist\n");
    		}
    		else if(res == 0) {
    			printf("Successfully added\n");
    		}
    	}
    	else if(mItem == 2) {
    		D_Add_Edge(&gTab);
    	}
    	else if(mItem == 5) {
    		D_Show(&gTab);
    	}
    }

    printf("End of program. Make clearing\n");
}