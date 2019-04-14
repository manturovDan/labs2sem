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
	struct Quadrant *parent;
} typedef Quadrant;

Quadrant *create(int size, int capacity) {
	Quadrant *root = (Quadrant *)calloc(1, sizeof(Quadrant));
	root->coordMin = - size / 2;
	root->coordMax = root->coordMin + size - 1;

	Item **points = (Item **) calloc(capacity, sizeof(Item *));
	root->point = points;

	root->child[0] = NULL;
	root->child[1] = NULL;
	root->child[2] = NULL;
	root->child[3] = NULL;

	root->parent = NULL;

	return root;
}

int add_el (Quadrant *root, int x, int y, char *info) {
	
}