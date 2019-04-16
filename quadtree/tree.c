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

struct bItem {
	Item *elem;
	struct bItem *parent;
	struct bItem *left;
	struct bItem *right;
} typedef bItem;

struct SearchRes {
	Quadrant *owner;
	int place;
} typedef SearchRes;

int born (Quadrant *parent, int capacity) {
	for (int i = 0; i < 4; i++) {
		parent->child[i] = (Quadrant *)calloc(1, sizeof(Quadrant));

		Item **points = (Item **) calloc(capacity, sizeof(Item *));
		parent->child[i]->point = points;

		parent->child[i]->child[0] = NULL;
		parent->child[i]->child[1] = NULL;
		parent->child[i]->child[2] = NULL;
		parent->child[i]->child[3] = NULL;

		parent->child[i]->parent = parent;
		parent->child[i]->busy = 0;
	}

	parent->child[1]->xMin = parent->xMin;
	parent->child[1]->xMax = parent->xMin + (parent->xMax - parent->xMin + 1) / 2 - 1;
	parent->child[0]->xMin = parent->child[1]->xMax + 1;
	parent->child[0]->xMax = parent->xMax;
	parent->child[2]->xMin = parent->xMin;
	parent->child[2]->xMax = parent->child[1]->xMax;
	parent->child[3]->xMin = parent->child[2]->xMax + 1;
	parent->child[3]->xMax = parent->xMax;

	parent->child[0]->yMin = parent->yMin;
	parent->child[0]->yMax = parent->yMin + (parent->yMax - parent->yMin + 1) / 2 - 1;
	parent->child[1]->yMin = parent->yMin;
	parent->child[1]->yMax = parent->child[0]->yMax;
	parent->child[2]->yMin = parent->child[0]->yMax + 1;
	parent->child[2]->yMax = parent->yMax;
	parent->child[3]->yMin = parent->child[0]->yMax + 1;
	parent->child[3]->yMax = parent->yMax;

	printf("\
			Parent: x %d - %d; y %d - %d\n\
			Q0: x %d - %d; y %d - %d\n\
			Q1: x %d - %d; y %d - %d\n\
			Q2: x %d - %d; y %d - %d\n\
			Q3: x %d - %d; y %d - %d\n",parent->xMin, parent->xMax,
										parent->yMin, parent->yMax,
										parent->child[0]->xMin, parent->child[0]->xMax,
										parent->child[0]->yMin, parent->child[1]->yMax,
										parent->child[1]->xMin, parent->child[1]->xMax,
										parent->child[1]->yMin, parent->child[1]->yMax,
										parent->child[2]->xMin, parent->child[2]->xMax,
										parent->child[2]->yMin, parent->child[2]->yMax,
										parent->child[3]->xMin, parent->child[3]->xMax,
										parent->child[3]->yMin, parent->child[3]->yMax);

	return 0;
}

SearchRes search(Quadrant *root, int x, int y, int capacity) {
	int it, s, sector;
	SearchRes res;
	while (1) {
		sector = -1;
		for (it = 0; it < capacity; it++) {
			if(root->point[it] != NULL && root->point[it]->x == x && root->point[it]->y == y) {
				printf("found\n");
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
			res.owner = NULL;
			res.place = -1;
			return res;
		}
		else {
			root = root->child[sector];
		}
	}
}

Quadrant *search_place(Quadrant *root, int x, int y, int capacity) {
	int s;
	while (1) {
		if(root == NULL) {
			break;
		}

		if (root->busy < capacity) {
			return root;
		}

		if(root->child[0] == NULL) {
			born(root, capacity);
		}

		for (s = 0; s < 4; s++) {
			if(root->child[s]->xMin <= x &&
			root->child[s]->xMax >= x &&
			root->child[s]->yMin <= y &&
			root->child[s]->yMax >= y) {
				break;
			}
		}
		printf("s: %d\n", s);
		root = root->child[s];
	}
	

	return NULL;
}

Quadrant *create(int size, int capacity) {
	Quadrant *root = (Quadrant *)calloc(1, sizeof(Quadrant));
	root->xMin = - size / 2;
	root->xMax = root->xMin + size - 1;
	root->yMin = root->xMin;
	root->yMax = root->xMax;

	Item **points = (Item **) calloc(capacity, sizeof(Item *));
	root->point = points;

	root->child[0] = NULL;
	root->child[1] = NULL;
	root->child[2] = NULL;
	root->child[3] = NULL;

	root->parent = NULL;
	root->busy = 0;

	return root;
}

int add_el (Quadrant *root, int x, int y, int capacity, char *info) {
	if(search(root, x, y, capacity).owner == NULL) {
		Quadrant *tofill = search_place(root, x, y, capacity);
		tofill->busy++;
		for (int i = 0; i < capacity; i++) {
			if(tofill->point[i] == NULL) {
				tofill->point[i] = (Item *)calloc(1, sizeof(Item));
				tofill->point[i]->x = x;
				tofill->point[i]->y = y;
				tofill->point[i]->info = info;
				return 0;
			}
		}
	}
	else {
		return 1;
	}
}

int check_brothers(Quadrant *parent, int capacity) { //0 - not free; 1 - free
	for(int i = 0; i < 4; i++) {
		for(int c = 0; c < capacity; c++) {
			if (parent->child[i]->point[c] != NULL) 
				return 0;
		}
	}
	return 1;
}

SearchRes find_successor(Quadrant *ancentor, int capacity) {
	SearchRes successor;
	int i;

	successor.owner = ancentor;
	while (1) {
		for (i = 0; i < 4; i++) {
			if(successor.owner->child[i]->busy != 0) {
				break;
			}
		}

		printf("III: %d\n", i);
		if (successor.owner->child[i]->child[0] == NULL) {
			for (int k = 0; k < capacity; k++) {
				if(successor.owner->child[i]->point[k] != NULL) {
					successor.owner = successor.owner->child[i];
					successor.place = k;
					return successor;
				}
			}
		}
		else {
			successor.owner = successor.owner->child[i];
		}
	}
}

int delete_el(Quadrant *root, int size, int capacity, int x, int y) {
	SearchRes victim = search(root, x, y, capacity);
	if(victim.owner == NULL) {
		return 1;
	}

	if (victim.owner->child[0] == NULL) {
		Quadrant *parent = victim.owner->parent;
		free(victim.owner->point[victim.place]->info);
		free(victim.owner->point[victim.place]);
		victim.owner->point[victim.place] = NULL;
		victim.owner->busy--;
		if(parent != NULL && check_brothers(parent, capacity) == 1) {
			for(int k = 0; k < 4; k++) {
				free(parent->child[k]);
				parent->child[k] = NULL;
			}
		}
	}
	else {
		free(victim.owner->point[victim.place]->info);
		free(victim.owner->point[victim.place]);
		SearchRes successor = find_successor(victim.owner, capacity);
		printf("Successor: %s\n", successor.owner->point[successor.place]->info);
		victim.owner->point[victim.place] = successor.owner->point[successor.place];
		successor.owner->point[successor.place] = NULL;
		successor.owner->busy--;
		if(check_brothers(successor.owner->parent, capacity) == 1) {
			for(int k = 0; k < 4; k++) {
				free(successor.owner->parent->child[k]);
				successor.owner->parent->child[k] = NULL;
			}
		}
	}
}

int bAdd(Item *added, bItem **bRoot, int capacity) {
	bItem *newL = (bItem *)calloc(1, sizeof(bItem));
	newL->elem = added;

	if (*bRoot == NULL) {
		*bRoot = newL;
		return 0;
	}

	bItem *cur = *bRoot;
	bItem *parent = NULL;

	while (cur != NULL) {
		parent = cur;
		if(added->x < cur->elem->x)
			cur = cur->left;
		else
			cur = cur->right;
	}

	if (added->x < parent->elem->x)
		parent->left = newL;
	else
		parent->right = newL;
	newL->parent = parent;

	return 0;
}

int stepToShowTree(Quadrant *current, bItem **bRoot, int capacity) {
	for(int i = 0; i < capacity; i++) {
		if(current->point[i] != NULL) {
			bAdd(current->point[i], bRoot, capacity);
		}
	}

	if(current->child[0] == NULL) 
		return 0;
	else {
		stepToShowTree(current->child[0], bRoot, capacity);
		stepToShowTree(current->child[1], bRoot, capacity);
		stepToShowTree(current->child[2], bRoot, capacity);
		stepToShowTree(current->child[3], bRoot, capacity);
	}
}

int delBTree(bItem *bRoot) {
	if (bRoot == NULL) 
		return 0;
	delBTree(bRoot->left);
	delBTree(bRoot->right);
	free(bRoot);
}