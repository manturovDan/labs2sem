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

TEST(SearchTest, st1) { //Test for searching function
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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}