struct Item {
	int x;
	int y;
	char *info;
} typedef Item;

struct Quadrant {
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	Item **point;
	int busy;
	struct Quadrant *child[4];
	struct Quadrant *parent;
} typedef Quadrant;

struct SearchRes {
	Quadrant *owner;
	int place;
} typedef SearchRes;

SearchRes search(Quadrant *root, int x, int y, int capacity) {
	int it, s, sector;
	SearchRes res;
	while (1) {
		sector = -1;
		for (it = 0; it < capacity; it++) {
			if(root->point[it] != NULL && root->point[it]->x == x && root->point[it]->y == y) {
				printf("Founded!\n");
				res.owner = root;
				res.place = it;
				return res;
			} 
		}

		for (s = 0; s < 4; s++) {
			if(root->child[s] != NULL &&
			root->child[s]->xMin <= x &&
			root->child[s]->xMax >= x &&
			root->child[s]->yMin <= y &&
			root->child[s]->yMax >= y) {
				sector = s;
				break;
			}
		}

		if (sector == -1) {
			printf("Not found!\n");
			res.owner = NULL;
			res.place = -1;
			return res;
		}
		else {
			root = root->child[sector];
		}
	}
}

Quadrant *create(int size, int capacity) {
	Quadrant *root = (Quadrant *)calloc(1, sizeof(Quadrant));
	root->xMin = - size / 2;
	root->xMax = root->xMin + size - 1;
	root->yMin = root->xMin;
	root->yMax = root->yMax;

	Item **points = (Item **) calloc(capacity, sizeof(Item *));
	root->point = points;

	root->child[0] = NULL;
	root->child[1] = NULL;
	root->child[2] = NULL;
	root->child[3] = NULL;

	root->parent = NULL;

	return root;
}



Quadrant *search_place(Quadrant *root, int x, int y) {

}

int add_el (Quadrant *root, int x, int y, char *info) {

}