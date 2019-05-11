#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "graph.c"
#include "fileWork.c"
#include <gtest/gtest.h>

int realCount(GraphClutch *gTab) {
	int n = 0;
	for(int i = 0; i < SIZE; i++) {
		CluItem *cont = gTab->grTab[i];
		while (cont != NULL) {
			n++;
			cont = cont->next;
		}
	}
	return n;
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

TEST(Uniqueness, ut1) { //Check that we cant add two element with same parametrs (only function returning)
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int add1 = adding(&gTab, 1, 0, 0);
	ASSERT_EQ(add1, 0);

	int add2 = adding(&gTab, 1, 0, 0);
	ASSERT_NE(add2, 0);

	int add3 = adding(&gTab, 90, 0, 0);
	ASSERT_NE(add3, 0);

	int add4 = adding(&gTab, 90, 1, 0);
	ASSERT_EQ(add4, 0);

	ASSERT_EQ(gTab.n, 2);
	ASSERT_EQ(realCount(&gTab), 2);
}

TEST(SearchTest, st1) { //Test for searching function by name
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int add1 = adding(&gTab, 1, 0.5, 123);
	int add2 = adding(&gTab, 5354, 1.423, 12);
	int add3 = adding(&gTab, 543, 23.12, 432.432);
	int add4 = adding(&gTab, 25, 243.42, 432);
	int add5 = adding(&gTab, 432, 765.5, 0);
	int add6 = adding(&gTab, 432, .434, 4324.0535);
	int add7 = adding(&gTab, 9423, 765.65, 0);
	int add8 = adding(&gTab, 4390, 0, 0);
	int add9 = adding(&gTab, 133, 65, 40);
	int add10 = adding(&gTab, 870, 3451, 0);
	int add11 = adding(&gTab, 4320, 543.5, -423);
	int add12 = adding(&gTab, 213, 643.643, 234);
	int add13 = adding(&gTab, 9438, 533.3511, 0);
	int add14 = adding(&gTab, 74, 5432, 4322);
	int add15 = adding(&gTab, 0, 423, -.432);
	int add16 = adding(&gTab, 11, 432, 32);
	int add17 = adding(&gTab, 122, 2432, 22);
	int add18 = adding(&gTab, 224, 142, 2234);
	int add19 = adding(&gTab, 228, 16, 10);
	int add20 = adding(&gTab, 42, 1, 0);
	int add21 = adding(&gTab, 910, 2321, 0.1111);

	ASSERT_EQ(gTab.n, 20);
	ASSERT_EQ(realCount(&gTab), 20);

	CluItem *f1 = find(&gTab, 1000);
	int n = 0;
	if (f1 == NULL)
		n = 1;
	ASSERT_EQ(n, 1);

	CluItem *f11 = find(&gTab, 0);
	n = 0;
	if (f11 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f11->fNode->name, 0);
	ASSERT_FLOAT_EQ(f11->fNode->x, 423);
	ASSERT_FLOAT_EQ(f11->fNode->y, -.432);

	CluItem *f2 = find(&gTab, 0);
	n = 0;
	if (f2 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f2->fNode->name, 0);
	ASSERT_FLOAT_EQ(f2->fNode->x, 423);
	ASSERT_FLOAT_EQ(f2->fNode->y, -.432);

	CluItem *f3 = find(&gTab, 4320);
	n = 0;
	if (f3 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f3->fNode->name, 4320);
	ASSERT_FLOAT_EQ(f3->fNode->x, 543.5);
	ASSERT_FLOAT_EQ(f3->fNode->y, -423);
	n = 0;
	if (f3->fNode->nbr == NULL)
		n = 1;
	ASSERT_EQ(n, 1);

	CluItem *f4 = find(&gTab, 910);
	n = 0;
	if (f4 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f4->fNode->name, 910);
	ASSERT_FLOAT_EQ(f4->fNode->x, 2321);
	ASSERT_FLOAT_EQ(f4->fNode->y, 0.1111);
	n = 0;
	if (f4->fNode->nbr == NULL)
		n = 1;
	ASSERT_EQ(n, 1);


}

TEST (SearchTest, st2) { //Search by coords
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int add1 = adding(&gTab, 1, 0.5, 123);
	int add2 = adding(&gTab, 5354, 1.423, 12);
	int add3 = adding(&gTab, 543, 23.12, 432.432);
	int add4 = adding(&gTab, 25, 243.42, 432);
	int add5 = adding(&gTab, 432, 765.5, 0);
	int add6 = adding(&gTab, 4320, .434, 4324.0535);
	int add7 = adding(&gTab, 9423, 765.65, 0);
	int add8 = adding(&gTab, 4390, 0, 0);
	int add9 = adding(&gTab, 133, 65, 40);
	int add10 = adding(&gTab, 870, 3451, 0);

	ASSERT_EQ(gTab.n, 10);
	ASSERT_EQ(realCount(&gTab), 10);

	CluItem *f1 = findCoords(&gTab, 1000, 2000);
	int n = 0;
	if (f1 == NULL)
		n = 1;
	ASSERT_EQ(n, 1);

	CluItem *f2 = findCoords(&gTab, 3451, 0);
	n = 0;
	if (f2 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f2->fNode->name, 870);
	ASSERT_FLOAT_EQ(f2->fNode->x, 3451);
	ASSERT_FLOAT_EQ(f2->fNode->y, 0);
	n = 0;
	if (f2->fNode->nbr == NULL)
		n = 1;
	ASSERT_EQ(n, 1);

	CluItem *f22 = findCoords(&gTab, 0.5, 123);
	n = 0;
	if (f22 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f22->fNode->name, 1);
	ASSERT_FLOAT_EQ(f22->fNode->x, 0.5);
	ASSERT_FLOAT_EQ(f22->fNode->y, 123);
	n = 0;
	if (f2->fNode->nbr == NULL)
		n = 1;
	ASSERT_EQ(n, 1);

	CluItem *f3 = findCoords(&gTab, 23.12, 432.432);
	n = 0;
	if (f3 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f3->fNode->name, 543);
	ASSERT_FLOAT_EQ(f3->fNode->x, 23.12);
	ASSERT_FLOAT_EQ(f3->fNode->y, 432.432);
	n = 0;
	if (f3->fNode->nbr == NULL)
		n = 1;
	ASSERT_EQ(n, 1);

	CluItem *f4 = findCoords(&gTab, 765.65, 0);
	n = 0;
	if (f4 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f4->fNode->name, 9423);
	ASSERT_FLOAT_EQ(f4->fNode->x, 765.65);
	ASSERT_FLOAT_EQ(f4->fNode->y, 0);
	n = 0;
	if (f4->fNode->nbr == NULL)
		n = 1;
	ASSERT_EQ(n, 1);
}

TEST(EdgeTest, et1) {
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int eres1 = edge(&gTab, 1, 5);
    int add1 = adding(&gTab, 1, 0.5, 123);
    int eres2 = edge(&gTab, 1, 1);
    int eres3 = edge(&gTab, 1, 1);
	int add2 = adding(&gTab, 5354, 1.423, 12);
	int add3 = adding(&gTab, 543, 23.12, 432.432);
	int add4 = adding(&gTab, 25, 243.42, 432);
	int add5 = adding(&gTab, 432, 765.5, 0);
	int eres4 = edge(&gTab, 1, 5354);
	int eres5 = edge(&gTab, 1, 25);
	int eres6 = edge(&gTab, 25, 1);
	int add6 = adding(&gTab, 4320, .434, 4324.0535);
	int add7 = adding(&gTab, 9423, 765.65, 0);
	int add8 = adding(&gTab, 4390, 0, 0);
	int add9 = adding(&gTab, 133, 65, 40);
	int add10 = adding(&gTab, 870, 3451, 0);
	int eres7 = edge(&gTab, 870, 133);

	ASSERT_NE(eres1, 0);
	ASSERT_EQ(eres2, 0);
	ASSERT_NE(eres3, 0);
	ASSERT_EQ(eres4, 0);
	ASSERT_EQ(eres5, 0);
	ASSERT_EQ(eres7, 0);
	ASSERT_EQ(eres7, 0);

	int n;
	CluItem *f1 = find(&gTab, 25);
	n = 0;
	if (f1 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f1->fNode->name, 25);
	n = 0;
	if (f1->fNode->nbr == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	Neighbour *nbr = f1->fNode->nbr;
	while (nbr != NULL) {
		ASSERT_EQ(nbr->name, 1);
		nbr = nbr->next;
	}

	CluItem *f2 = find(&gTab, 1);
	n = 0;
	if (f2 == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	ASSERT_EQ(f2->fNode->name, 1);
	n = 0;
	if (f2->fNode->nbr == NULL)
		n = 1;
	ASSERT_NE(n, 1);
	nbr = f2->fNode->nbr;
	ASSERT_EQ(nbr->name, 25);
	ASSERT_FLOAT_EQ(nbr->dist, 393.053592);
	nbr = nbr->next;

	ASSERT_EQ(nbr->name, 5354);
	ASSERT_FLOAT_EQ(nbr->dist, 111.003837);
	nbr = nbr->next;

	ASSERT_EQ(nbr->name, 1);
	ASSERT_FLOAT_EQ(nbr->dist, 0);
	nbr = nbr->next;

	n = 0;
	if (nbr == NULL) 
		n = 1;
	ASSERT_EQ(n, 1);
}

TEST(FileTest, writing) {
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int eres1 = edge(&gTab, 1, 5);
    int add1 = adding(&gTab, 1, 0.5, 123);
    int eres2 = edge(&gTab, 1, 1);
    int eres3 = edge(&gTab, 1, 1);
	int add2 = adding(&gTab, 5354, 1.423, 12);
	int add3 = adding(&gTab, 543, 23.12, 432.432);
	int add4 = adding(&gTab, 25, 243.42, 432);
	int add5 = adding(&gTab, 432, 765.5, 0);
	int eres4 = edge(&gTab, 1, 5354);
	int eres5 = edge(&gTab, 1, 25);
	int eres6 = edge(&gTab, 25, 1);
	int add6 = adding(&gTab, 4320, .434, 4324.0535);
	int add7 = adding(&gTab, 9423, 765.65, 0);
	int add8 = adding(&gTab, 4390, 0, 0);
	int add9 = adding(&gTab, 133, 65, 40);
	int add10 = adding(&gTab, 870, 3451, 0);
	int eres7 = edge(&gTab, 870, 133);

	char *fName = (char *)calloc(20, sizeof(char));
	strcpy(fName, "tests/t1.txt");
	int write = writeGraph(&gTab, fName);
	ASSERT_NE(write, 1);

	char *fName2 = (char *)calloc(20, sizeof(char));
	strcpy(fName2, "tests/t1.txt");
	FILE *read = fopen(fName2, "rt");
	int n = 1;
	if (read <= 0)
		n = 0;
	ASSERT_NE(n, 0);

	char *fNameRef = (char *)calloc(20, sizeof(char));
	strcpy(fNameRef, "tests/t1.txt");
	FILE *readR = fopen(fNameRef, "rt");
	n = 1;
	if (readR <= 0)
		n = 0;
	ASSERT_NE(n, 0);

	char rowT[50];
	char rowR[50];
	while (1) {
		if(fscanf(read, "%s", rowT) <= 0 || fscanf(readR, "%s", rowR) <= 0)
			break;
		ASSERT_STREQ(rowT, rowR);
	}

}

TEST(FileTest, reading) {
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int eres1 = edge(&gTab, 1, 5);
    int add1 = adding(&gTab, 1, 0.5, 123);
    int eres2 = edge(&gTab, 1, 1);
    int eres3 = edge(&gTab, 1, 1);
	int add2 = adding(&gTab, 5354, 1.423, 12);
	int add3 = adding(&gTab, 543, 23.12, 432.432);
	int add4 = adding(&gTab, 25, 243.42, 432);
	int add5 = adding(&gTab, 432, 765.5, 0);
	int eres4 = edge(&gTab, 1, 5354);
	int eres5 = edge(&gTab, 1, 25);
	int eres6 = edge(&gTab, 25, 1);
	int add6 = adding(&gTab, 4320, .434, 4324.0535);
	int add7 = adding(&gTab, 9423, 765.65, 0);
	int add8 = adding(&gTab, 4390, 0, 0);
	int add9 = adding(&gTab, 133, 65, 40);
	int add10 = adding(&gTab, 870, 3451, 0);
	int eres7 = edge(&gTab, 870, 133);

	char *fName = (char *)calloc(20, sizeof(char));
	strcpy(fName, "tests/t2.txt");
	int write = writeGraph(&gTab, fName);
	ASSERT_NE(write, 1);

	GraphClutch nTab;
    nTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	nTab.grTab[i] = NULL;
    }

	char *fName2 = (char *)calloc(20, sizeof(char));
	strcpy(fName2, "tests/t2.txt");
	int read = readGraph(&nTab, fName2);
	ASSERT_EQ(read, 0);

	for (int i = 0; i < SIZE; i ++) {
		CluItem *cont1 = gTab.grTab[i];
		CluItem *cont2 = nTab.grTab[i];
		while(cont1 != NULL || cont2 != NULL) {
			ASSERT_EQ(cont1->fNode->name, cont2->fNode->name);
			ASSERT_FLOAT_EQ(cont1->fNode->x, cont2->fNode->x);
			ASSERT_FLOAT_EQ(cont1->fNode->y, cont2->fNode->y);
			Neighbour *nbr1 = cont1->fNode->nbr;
			Neighbour *nbr2 = cont2->fNode->nbr;
			int sumName1 = 0;
			int sumName2 = 0;
			float sumDist1 = 0;
			float sumDist2 = 0;
			while(nbr1 != NULL || nbr2 != NULL) {
				sumName1 += nbr1->name;
				sumName2 += nbr2->name;
				sumDist1 += nbr1->dist;
				sumDist2 += nbr2->dist;
				nbr1 = nbr1->next;
				nbr2 = nbr2->next;
			}

			ASSERT_EQ(sumName1, sumName2);
			ASSERT_FLOAT_EQ(sumDist1, sumDist2);

			cont1 = cont1->next;
			cont2 = cont2->next;
		}
	}
}

TEST (DeleteTest, dtest1) {
	GraphClutch gTab;
    gTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	gTab.grTab[i] = NULL;
    }

    int adv1 = adding(&gTab, 100, 64.57, -5341);
    int aeres1 = edge(&gTab, 1, 5);
    int aadd1 = adding(&gTab, 1, 0.5, 123);
    int aeres2 = edge(&gTab, 1, 1);
    int aeres3 = edge(&gTab, 1, 1);
    int adv2 = adding(&gTab, 100500, 186.57, 155);
	int aadd2 = adding(&gTab, 5354, 1.423, 12);
	int aadd3 = adding(&gTab, 543, 23.12, 432.432);
	int ade1 = edge(&gTab,1, 100);
	int ade2 = edge(&gTab,100, 1);
	int ade3 = edge(&gTab,100500, 100);
	int ade4 = edge(&gTab,100500, 1);
	int aadd4 = adding(&gTab, 25, 243.42, 432);
	int aadd5 = adding(&gTab, 432, 765.5, 0);
	int aeres4 = edge(&gTab, 1, 5354);
	int aeres5 = edge(&gTab, 1, 25);
	int aeres6 = edge(&gTab, 25, 1);
	int aadd6 = adding(&gTab, 4320, .434, 4324.0535);
	int aadd7 = adding(&gTab, 9423, 765.65, 0);
	int adv3 = adding(&gTab, 101, 86.57, 55);
	int aadd8 = adding(&gTab, 4390, 0, 0);
	int aadd9 = adding(&gTab, 133, 65, 40);
	int aadd10 = adding(&gTab, 870, 3451, 0);
	int aeres7 = edge(&gTab, 870, 133);
	int ade5 = edge(&gTab,1, 101);
	int ade6 = edge(&gTab,870, 100500);
	int aeres8 = edge(&gTab, 0, 18887);
	int ade8 = edge(&gTab,25, 101);
	int ade9 = edge(&gTab,100500, 25);
	int ade10 = edge(&gTab,432, 101);

	deleteEl(&gTab, 100);
	deleteEl(&gTab, 100500);
	deleteEl(&gTab, 101);


	ASSERT_NE(aeres8, 0);

	GraphClutch nTab;
    nTab.n = 0;
    for (int i =0; i < SIZE; i++) {
    	nTab.grTab[i] = NULL;
    }

    int eres1 = edge(&nTab, 1, 5);
    int add1 = adding(&nTab, 1, 0.5, 123);
    int eres2 = edge(&nTab, 1, 1);
    int eres3 = edge(&nTab, 1, 1);
	int add2 = adding(&nTab, 5354, 1.423, 12);
	int add3 = adding(&nTab, 543, 23.12, 432.432);
	int add4 = adding(&nTab, 25, 243.42, 432);
	int add5 = adding(&nTab, 432, 765.5, 0);
	int eres4 = edge(&nTab, 1, 5354);
	int eres5 = edge(&nTab, 1, 25);
	int eres6 = edge(&nTab, 25, 1);
	int add6 = adding(&nTab, 4320, .434, 4324.0535);
	int add7 = adding(&nTab, 9423, 765.65, 0);
	int add8 = adding(&nTab, 4390, 0, 0);
	int add9 = adding(&nTab, 133, 65, 40);
	int add10 = adding(&nTab, 870, 3451, 0);
	int eres7 = edge(&nTab, 870, 133);

	for (int i = 0; i < SIZE; i ++) {
		CluItem *cont1 = gTab.grTab[i];
		CluItem *cont2 = nTab.grTab[i];
		while(cont1 != NULL || cont2 != NULL) {
			ASSERT_EQ(cont1->fNode->name, cont2->fNode->name);
			ASSERT_FLOAT_EQ(cont1->fNode->x, cont2->fNode->x);
			ASSERT_FLOAT_EQ(cont1->fNode->y, cont2->fNode->y);
			Neighbour *nbr1 = cont1->fNode->nbr;
			Neighbour *nbr2 = cont2->fNode->nbr;
			int sumName1 = 0;
			int sumName2 = 0;
			float sumDist1 = 0;
			float sumDist2 = 0;
			while(nbr1 != NULL || nbr2 != NULL) {
				sumName1 += nbr1->name;
				sumName2 += nbr2->name;
				sumDist1 += nbr1->dist;
				sumDist2 += nbr2->dist;
				nbr1 = nbr1->next;
				nbr2 = nbr2->next;
			}

			ASSERT_EQ(sumName1, sumName2);
			ASSERT_FLOAT_EQ(sumDist1, sumDist2);

			cont1 = cont1->next;
			cont2 = cont2->next;
		}
	}
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}