#define SIZE 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "graph.c"
#include "fileWork.c"

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
	if(adding(gTab, name, x, y) == 0) {
		printf("Added Successfully\n");
		return 0;
	}
	else {
		printf("Element with these name or coordinates is exist\n");
		return 1;
	}
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

	printf("Connected successfully\n");
	return 0;

}

int D_Del(GraphClutch *gTab) {
	int res, delName;
	printf("Input name (int) of deliting node\n");
	res = get_nInt(&delName);
	if (res == 0) {
		printf("Error\n");
		return 1;
	}

	int del = delete(gTab, delName);
	if (del != 0) {
		printf("No element with these name\n");
		return 1;
	}
	
	printf("Deleted Successfully\n");
	return 0;
}

int D_Save(GraphClutch *gTab) {
	char *fName = NULL;
	printf("Input name of saveing file\n");
	fName = getStr();
	/*
		save struct:
		name
		x
		y
		n1
		n2
		..
		nn
		-1//end of node
		......
	*/
	if (writeGraph(gTab, fName) == 1) {
		printf("File error!\n");
		return 1;
	}

	printf("Graph was recorded Successfully\n");
	return 0;
}

int D_Load(GraphClutch *gTab) {
	char *fName = NULL;
	printf("Input name of file\n");
	fName = getStr();
	if (readGraph(gTab, fName) != 0) {
		printf("Error of Graph reading\n");
		//clear
		return 1;
	}
	return 0;
}

int D_Rand(GraphClutch *gTab) {
	int gSize;
	printf("Input size of Graph (1 ÷ 10K)\n");
	if(get_nInt(&gSize) == 0 || gSize < 1 || gSize > 10000) {
		printf("Error. Try Again\n");
		return 1;
	}

	int name;
	float x, y;
	int mxVal = gSize*1000;
	srand ( time(NULL) );

	int *names = (int *)calloc(gSize, sizeof(int));
	for (int i = 0; i < gSize; i++) {
		name = rand() % mxVal;
		x = (float)(rand() % mxVal - mxVal)/(float)(rand() % mxVal/1000 - mxVal/1000);
		y = (float)(rand() % mxVal - mxVal)/(float)(rand() % mxVal/1000 - mxVal/1000);
		if (adding(gTab, name, x, y) != 0) {
			i--;
			continue;
		}
		names[i] = name;
	}
	int nFrom, nTo;
	for (int i = 0; i < gSize*(rand() % gSize + 1); i++) {
		nFrom = names[rand() % gSize];
		nTo = names[rand() % gSize];
		edge(gTab, nFrom, nTo);
	}

	free(names);
}

int D_Clear(GraphClutch *gTab) {
	int cl = clear(gTab);
	return 0;
}

int Print_Matrix(int sz, mCols *matrix) {
	printf("SIZE: %d\nID, Names\n", sz);
	for (int t = 0; t < sz; t++) {
		printf("%d %d\n", matrix->names[t][0], matrix->names[t][1]);
	}

	printf("\nDistances\n");
	for (int i = 0; i < sz+1; i++) {
		printf("%d\n", i);
		for (int j = 0; j < sz; j++) {
			for (int k = 0; k < sz; k++) {
				matrix->dist[i][j][k].inf == 0 ? printf("%f\t", matrix->dist[i][j][k].dist) : printf("∞\t\t");
			}
			printf("\n");
		}
		printf("\n");
	}

	printf("Predecessors\n");
	for (int i = 0; i < sz+1; i++) {
		printf("%d\n", i);
		for (int j = 0; j < sz; j++) {
			for (int k = 0; k < sz; k++) {
				matrix->pred[i][j][k] != -1 ? printf("%d\t", matrix->pred[i][j][k]) : printf("Ø\t");
			}
			printf("\n");
		}
		printf("\n");
	}
}

int D_FW(GraphClutch *gTab) {
	mCols *matrix = fMatrix(gTab);

	Print_Matrix(gTab->n, matrix);
}

int main() {
	char *msgs[] = {"0. Quit", "1. Add node to Graph", "2. Add edge to Graph", 
					"3. Delete node from Graph",
					"4. Find 3 shortest ways between all nodes (Floyd–Warshall algorithm)",
					"5. Print Graph", "6. Save Graph in file"};


    GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

	while (1) {
		printf("This is Graph program. Choose one option\n1. Create Graph by hand\n2. Generate graph\n3. Load Graph from file\n");
		int mode;
		if (get_nInt(&mode) == 0) {
			printf("Error. Try again\n");
			continue;
		}
		else if(mode == 1) {
			printf("Empty Graph was created\n");
			break;
		}
		else if(mode == 2) {
			printf("Random graph\n");
			D_Rand(&gTab);
			break;
		}
		else if(mode == 3) {
			printf("Load graph\n");
			D_Load(&gTab);
			break;
		}
		else {
			printf("Input correct value\n");
			continue;
		}

	}

    int mItem;
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
    		D_Add(&gTab);
    	}
    	else if(mItem == 2) {
    		D_Add_Edge(&gTab);
    	}
    	else if(mItem == 3) {
    		D_Del(&gTab);
    	}
    	else if(mItem == 4) {
    		D_FW(&gTab);
    	}
    	else if(mItem == 5) {
    		D_Show(&gTab);
    	}
    	else if(mItem == 6) {
    		D_Save(&gTab);
    	}
    }

    D_Clear(&gTab);
    printf("End of program\n");
}