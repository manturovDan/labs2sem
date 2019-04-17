//tests.cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.c"
#include <gtest/gtest.h>

int countElements(Quadrant *root, int size, int capacity, int *count) {
	for (int i = 0; i < capacity; i++) {
		if (root->point[i] != NULL)
			(*count)++;
	}

	if (root->child[0] == NULL)
		return 0;
	countElements(root->child[0], size, capacity, count);
	countElements(root->child[1], size, capacity, count);
	countElements(root->child[2], size, capacity, count);
	countElements(root->child[3], size, capacity, count);
}

int D_Show(Quadrant *root, int size, int capacity, int deep) {
	for (int t = 0; t < deep; t++)
		printf("\t");
	printf("[Addr: %p; Busy: %d] ", root, root->busy);
	
	for (int i = 0; i < capacity; i++) {
		if(root->point[i] == NULL)
			printf("(%d NULL) ", i);
		else 
			printf("(%d [%d; %d] : %s) ", i, root->point[i]->x, root->point[i]->y, root->point[i]->info);
	}
	printf("\n");

	if (root->child[0] != NULL) {
		D_Show(root->child[0], size, capacity, deep+1);
		D_Show(root->child[1], size, capacity, deep+1);
		D_Show(root->child[2], size, capacity, deep+1);
		D_Show(root->child[3], size, capacity, deep+1);
	}
	else {
		return 0;
	}
}

TEST(CountTests, ct1) {
	int n, steps, size, capacity, countEls;
	size = 15;
	capacity = 3;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);

	for (int i = 0; i < 5; i++) {
		int x = i-2;
		int y = i-2;

		char *testStr = (char *)calloc(4, sizeof(char));
		strcpy(testStr, "test");

		add_el(*root, x, y, capacity, testStr);
	}

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	//D_Show(*root, size, capacity, 0);
	ASSERT_EQ(cnt, 5);
}

TEST(CountTests, ct2) {
	int n, steps, size, capacity, countEls;
	size = 100000;
	capacity = 1500;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);

	for (int i = 0; i < 10000; i++) {
		int x = i-2;
		int y = i-2;

		char *testStr = (char *)calloc(4, sizeof(char));
		strcpy(testStr, "test");

		if (add_el(*root, x, y, capacity, testStr) == -1) {
			printf("No mem");
			break;
		}
	}

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	//D_Show(*root, size, capacity, 0);
	ASSERT_EQ(cnt, 10000);
}

TEST(CountTests, ct3) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 5;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = -0;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 700;
	y = -40000;
	add_el(*root, x, y, capacity, testStr);

	x = 15;
	y = 17;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 10001;
	add_el(*root, x, y, capacity, testStr);

	x = 3123;
	y = 32133;
	add_el(*root, x, y, capacity, testStr);

	x = 931;
	y = 31;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 21000;
	add_el(*root, x, y, capacity, testStr);

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	//D_Show(*root, size, capacity, 0);
	ASSERT_EQ(cnt, 10);
}

TEST(CountTests, ct4) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 1;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = 0;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 700;
	y = -40000;
	add_el(*root, x, y, capacity, testStr);

	x = 15;
	y = 17;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 10001;
	add_el(*root, x, y, capacity, testStr);

	x = 3123;
	y = 32133;
	add_el(*root, x, y, capacity, testStr);

	x = 931;
	y = 31;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 21000;
	add_el(*root, x, y, capacity, testStr);

	x = -1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = -955;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = 90;
	add_el(*root, x, y, capacity, testStr);

	x = 1255;
	y = -640;
	add_el(*root, x, y, capacity, testStr);

	x = -700;
	y = 40000;
	add_el(*root, x, y, capacity, testStr);

	x = -15;
	y = -17;
	add_el(*root, x, y, capacity, testStr);

	x = 11000;
	y = 0;
	add_el(*root, x, y, capacity, testStr);

	x = 8000;
	y = 10701;
	add_el(*root, x, y, capacity, testStr);

	x = 12212;
	y = 32133;
	add_el(*root, x, y, capacity, testStr);

	x = 931;
	y = -31;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -21009;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -21009;
	add_el(*root, x, y, capacity, testStr);


	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	//D_Show(*root, size, capacity, 0);
	ASSERT_EQ(cnt, 21);
}

TEST(CountTests, ct5) {
	int n, steps, size, capacity, countEls, x, y;
	size = 10000;
	capacity = 5;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	//D_Show(*root, size, capacity, 0);
	ASSERT_EQ(cnt, 1);
}

TEST(CountTests, ct6) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 5;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	//D_Show(*root, size, capacity, 0);
	ASSERT_EQ(cnt, 0);
}

TEST(SearchTests, st1) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 3;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	char *testStr2 = (char *)calloc(6, sizeof(char));
	strcpy(testStr2, "test2");

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr2);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = -0;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 700;
	y = -40000;
	add_el(*root, x, y, capacity, testStr);

	x = 15;
	y = 17;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 10001;
	add_el(*root, x, y, capacity, testStr);

	x = 3123;
	y = 32133;
	add_el(*root, x, y, capacity, testStr);

	x = 931;
	y = 31;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 21000;
	add_el(*root, x, y, capacity, testStr);

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);

	SearchRes result = search(*root, 1000, 21000, capacity);
	ASSERT_STREQ(result.owner->point[result.place]->info, "test");

	SearchRes result1 = search(*root, 1000, 210000, capacity);
	int comp;
	if (result1.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 0);

	SearchRes result2 = search(*root, 1000, -1000, capacity);
	ASSERT_STREQ(result2.owner->point[result2.place]->info, "test2");
}

TEST(SearchTests, st2) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 50;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	SearchRes result1 = search(*root, 10000, 210000, capacity);
	int comp;
	if (result1.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 0);

	SearchRes result2 = search(*root, -1000, 210000, capacity);
	
	if (result2.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 0);
}

TEST(DeleteTests, dt1) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 50;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);

	delete_el(*root, size, capacity, 0, 0);
	delete_el(*root, size, capacity, 10, 0);
	delete_el(*root, size, capacity, 0, -5);

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 0);
}

TEST(DeleteTests, dt2) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 1;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = 0;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 700;
	y = -40000;
	add_el(*root, x, y, capacity, testStr);

	x = 15;
	y = 17;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 1000;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 10001;
	add_el(*root, x, y, capacity, testStr);

	x = 3123;
	y = 32133;
	add_el(*root, x, y, capacity, testStr);

	x = 931;
	y = 31;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = 21000;
	add_el(*root, x, y, capacity, testStr);

	x = -1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = -955;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = 90;
	add_el(*root, x, y, capacity, testStr);

	x = 1255;
	y = -640;
	add_el(*root, x, y, capacity, testStr);

	x = -700;
	y = 40000;
	add_el(*root, x, y, capacity, testStr);

	x = -15;
	y = -17;
	add_el(*root, x, y, capacity, testStr);

	x = 11000;
	y = 0;
	add_el(*root, x, y, capacity, testStr);

	x = 8000;
	y = 10701;
	add_el(*root, x, y, capacity, testStr);

	x = 12212;
	y = 32133;
	add_el(*root, x, y, capacity, testStr);

	x = 931;
	y = -31;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -21009;
	add_el(*root, x, y, capacity, testStr);

	x = 1000;
	y = -21009;
	add_el(*root, x, y, capacity, testStr);

	delete_el(*root, size, capacity, 0, 0);
	delete_el(*root, size, capacity, 10, 0);
	delete_el(*root, size, capacity, 0, -5);

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 20);

	cnt = 0;
	delete_el(*root, size, capacity, 1000, -21009);
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 19);

	cnt = 0;
	delete_el(*root, size, capacity, 1000, -21009);
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 19);

	cnt = 0;
	delete_el(*root, size, capacity, 931, -31);
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 18);

	SearchRes result2 = search(*root, -1000, 210000, capacity);
	int comp;
	if (result2.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 0);

	SearchRes result = search(*root, 1255, -640, capacity);
	if (result.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 1);

}

TEST(DeleteTests, dt3) {
	int n, steps, size, capacity, countEls, x, y;
	size = 100000;
	capacity = 1;
	Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));
	*root = create(size, capacity);
	char *testStr = (char *)calloc(5, sizeof(char));
	strcpy(testStr, "test");

	x = 1000;
	y = -1000;
	add_el(*root, x, y, capacity, testStr);

	x = 55;
	y = -40;
	add_el(*root, x, y, capacity, testStr);

	x = 0;
	y = 0;
	add_el(*root, x, y, capacity, testStr);

	delete_el(*root, size, capacity, 0, 0);
	delete_el(*root, size, capacity, 10, 0);
	delete_el(*root, size, capacity, 0, -5);

	int cnt = 0;
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 2);

	delete_el(*root, size, capacity, 0, 0);
	delete_el(*root, size, capacity, 1000, -1000);
	delete_el(*root, size, capacity, 55, -40);

	cnt = 0;
	countElements(*root, size, capacity, &cnt);
	ASSERT_EQ(cnt, 0);

	SearchRes result2 = search(*root, -1000, 210000, capacity);
	int comp;
	if (result2.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 0);

	SearchRes result = search(*root, 1000, -1000, capacity);
	if (result.owner == NULL)
		comp = 0;
	else
		comp = 1;
	ASSERT_EQ(comp, 0);

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}