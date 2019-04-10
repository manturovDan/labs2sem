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
	Quadrant *child[4];
} typedef Quadrant;