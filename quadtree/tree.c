struct Item {
	int x;
	int y;
	char *info;
} typedef Item;

struct Quadrant {
	int coordMin;
	int coordMax;
	Item **point;
	int busy;
	struct Quadrant *child[4];
} typedef Quadrant;

Quadrant *create(int size, int capacity) {
	Quadrant *root = (Quadrant *)calloc(1, sizeof(Quadrant));
	root->coordMin = - size / 2;
	root->coordMax = size / 2 + (size % 2) - 1;

	Item **points = (Item **) calloc(capacity, sizeof(Item *));
	root->point = points;

	return root;
}