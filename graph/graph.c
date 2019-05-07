struct Neighbour {
	int name;
	float dist;
	struct Neighbour *next;
} typedef Neighbour;

struct Node {
	int name;
	float x;
	float y;
	Neighbour *nbr;
} typedef Node;

struct CluItem {
	Node *fNode;
	struct CluItem *next;
} typedef CluItem;

struct GraphClutch {
	int n;
	CluItem *grTab[SIZE];
} typedef GraphClutch;

struct dval {
	int inf; //1 or 0
	float dist; //if int == 0
} typedef dval;

struct mCols {
	dval ***dist;
	int ***pred;
	int **names;
} typedef mCols;

struct Way {
	int from;
	int to;
	dval dist;
	Neighbour *road;
	struct Way *next;
} typedef Way;


int hash(int name) {
	return name % SIZE;
}

CluItem *find(GraphClutch *gTab, int name) {
	int tabPlace = hash(name);

	CluItem *cont = gTab->grTab[tabPlace];
	if(cont == NULL) {
		return NULL;
	}

	while(cont != NULL) {
		if(cont->fNode->name == name) {
			return cont;
		}
		cont = cont->next;
	}

	return NULL;
}

CluItem *findCoords(GraphClutch *gTab, float x, float y) {
	CluItem *cont;
	for (int i = 0; i < SIZE; i++) {
		cont = gTab->grTab[i];
		while (cont != NULL) {
			if(cont->fNode->x == x && cont->fNode->y == y)
				return cont;
			cont = cont->next;
		}
	}

	return NULL;
}

int adding(GraphClutch *gTab, int name, float x, float y) { //returns: 2 - is already exist
	if (find(gTab, name) != NULL || findCoords(gTab, x, y) != NULL) {
		return 2;
	}

	int tabPlace = hash(name);

	CluItem *newCont = (CluItem *)calloc(1, sizeof(CluItem));
	if (gTab->grTab[tabPlace] != NULL)
		newCont->next = gTab->grTab[tabPlace];
	newCont->fNode = (Node *)calloc(1, sizeof(Node));
	newCont->fNode->name = name;
	newCont->fNode->x = x;
	newCont->fNode->y = y;
	newCont->fNode->nbr = NULL;
	gTab->grTab[tabPlace] = newCont;
	gTab->n++;

	return 0;	
}

float calcDist(float x1, float y1, float x2, float y2) {
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int edge(GraphClutch *gTab, int from, int to) {
	CluItem *nfrom = find(gTab, from);
	CluItem *nto = find(gTab, to);
	if(nfrom == NULL || nto == NULL) {
		return 2; //no nodes
	}

	Neighbour *nbr = nfrom->fNode->nbr;
	while (nbr != NULL) {
		if(nbr->name == to) {
			return 1; //way exists
		}
		nbr = nbr->next;
	} 

	Neighbour *newCon = (Neighbour *)calloc(1, sizeof(Neighbour));
	if(nfrom->fNode->nbr != NULL) 
		newCon->next = nfrom->fNode->nbr;
	newCon->name = nto->fNode->name;
	newCon->dist = calcDist(nfrom->fNode->x, nfrom->fNode->y, nto->fNode->x, nto->fNode->y);
	nfrom->fNode->nbr = newCon;
	return 0;
}

int delete(GraphClutch *gTab, int name) {
	int tabPlace = hash(name);

	CluItem *cont = gTab->grTab[tabPlace];
	if(cont == NULL) {
		return 1;
	}

	CluItem *last = NULL;
	int delItm = 0;
	while(cont != NULL) {
		if(cont->fNode->name == name) {
			delItm = 1;
			break;
		}
		last = cont;
		cont = cont->next;
	}

	//del delItm
	if(cont == NULL)
		return 2;

	if (last == NULL) { //first elem in clueue list
		if(cont->next != NULL)
			gTab->grTab[tabPlace] = cont->next;
		else 
			gTab->grTab[tabPlace] = NULL;
	}
	else if(cont->next != NULL)
		last->next = cont->next;
	
	Neighbour *nbr = cont->fNode->nbr;
	free(cont->fNode);
	free(cont);

	Neighbour *deln;
	while (nbr != NULL) {
		deln = nbr;
		nbr = nbr->next;
		free(deln);
	}

	gTab->n--;

	for (int i = 0; i < SIZE; i++) {
		CluItem *cont = gTab->grTab[i];
		Neighbour *nbr;
		Neighbour *lnbr = NULL;
		while (cont != NULL) {
			nbr = cont->fNode->nbr;
			while(nbr != NULL) {
				if(nbr->name == name) {
					break;
				}
				lnbr = nbr;
				nbr = nbr->next;
			}

			if(nbr != NULL) {
				if(last == NULL) {
					if (nbr->next != NULL)
						cont->fNode->nbr = nbr->next;
					else
						cont->fNode->nbr = NULL;
				}
				else if(nbr->next != NULL)
					lnbr->next = nbr->next;

				free(nbr);
			}

			cont = cont->next;
		}
	}

	return 0;
}

int clear(GraphClutch *gTab) {
	CluItem *cont;
	CluItem *last;
	for (int i = 0; i < SIZE; i++) {
		cont = gTab->grTab[i];
		last = NULL;
		while(cont != NULL) {
			last = cont;
			Neighbour *nbr = cont->fNode->nbr;
			Neighbour *deln;
			while(nbr != NULL) {
				deln = nbr;
				free(deln);
				nbr = nbr->next;
			}

			free(last->fNode);
			free(last);
			cont = cont->next;
		}
	}

	return 0;
}

int idByName(mCols *matrix, int name, int sz) {
	//TODO QSort, Binsearch
	for (int i = 0; i < sz; i++) {
		if(matrix->names[i][1] == name) {
			return i;
		}
	}
	return -1;
}

mCols *createMatrix(GraphClutch *gTab) {
	mCols *repo = (mCols *)calloc(1, sizeof(repo));
	
	repo->dist = (dval ***)calloc(gTab->n+1, sizeof(dval **));
	for (int i = 0; i < gTab->n+1; i++) {
		repo->dist[i] = (dval **)calloc(gTab->n, sizeof(dval *));
		for (int j = 0; j < gTab->n; j++) {
			repo->dist[i][j] = (dval *)calloc(gTab->n, sizeof(dval));
			for (int k = 0; k < gTab->n; k++)
				repo->dist[i][j][k].inf = 1;
		}
	}

	repo->pred = (int ***)calloc(gTab->n+1, sizeof(int **));
	for (int i = 0; i < gTab->n+1; i++) {
		repo->pred[i] = (int **)calloc(gTab->n, sizeof(int *));
		for (int j = 0; j < gTab->n; j++) {
			repo->pred[i][j] = (int *)calloc(gTab->n, sizeof(int));
			for (int k = 0; k < gTab->n; k++)
				repo->pred[i][j][k] = -1;
		}
	}

	repo->names = (int **)calloc(gTab->n, sizeof(int *));
	int n = 0;
	for (int t = 0; t < SIZE; t++) {
		CluItem *cont = gTab->grTab[t];
		while (cont != NULL) {
			repo->names[n] = (int *)calloc(2, sizeof(int));
			repo->names[n][0] = n;
			repo->names[n][1] = cont->fNode->name;
			cont = cont->next;
			n++;
		}
	}
	return repo;
}

int startMatrix(GraphClutch *gTab, mCols *matrix) {
	for (int s = 0; s < gTab->n; s++) {
		CluItem *cNode = find(gTab, matrix->names[s][1]);
		Neighbour *nbr = cNode->fNode->nbr;
		int id;
		while (nbr != NULL) {
			id = idByName(matrix, nbr->name, gTab->n);
			matrix->dist[0][s][id].inf = 0;
			matrix->dist[0][s][id].dist = nbr->dist;
			matrix->pred[0][s][id] = s;
			
			nbr = nbr->next;
		} 
		for (int k = 0; k <= gTab->n; k++) {
			matrix->dist[k][s][s].inf = 0;
			matrix->dist[k][s][s].dist = 0;
		}
	}

	return 0;
}

int FloydWarshall(int sz, mCols *matrix) {
	for (int k = 1; k <= sz; k++) {
		for (int i = 0; i < sz; i++) {
			for (int j = 0; j < sz; j++) {
				if (matrix->dist[k-1][i][k-1].inf != 1 && matrix->dist[k-1][k-1][j].inf != 1 && (matrix->dist[k-1][i][j].inf == 1 || matrix->dist[k-1][i][j].dist > matrix->dist[k-1][i][k-1].dist + matrix->dist[k-1][k-1][j].dist)) {
					matrix->dist[k][i][j].inf = 0;
					matrix->dist[k][i][j].dist = matrix->dist[k-1][i][k-1].dist + matrix->dist[k-1][k-1][j].dist;
					matrix->pred[k][i][j] = matrix->pred[k-1][k-1][j];
				}
				else {
					matrix->dist[k][i][j] = matrix->dist[k-1][i][j];
					matrix->pred[k][i][j] = matrix->pred[k-1][i][j];
				}
			}
		}
	}

	return 0;
}

Way *shortest(int sz, mCols *matrix) {
	Way *tail;
	Way *head;
	tail = NULL;
	head = NULL;
	for(int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (i == j)
				continue;
			int deep = sz;
			int shr = 0;
			float dist = -1;

			while (deep >= 0 && shr < 3) {
				if (matrix->dist[deep][i][j].inf == 1 == 0 && (matrix->dist[deep][i][j].dist < dist || dist == -1)){
					dist = matrix->dist[deep][i][j].dist;
					Way *itm = (Way*)calloc(1, sizeof(Way));
					itm->from = i;
					itm->to = j;
					itm->dist.inf = 0;
					itm->dist.dist = dist;
					if (tail == NULL)
						tail = itm;
					else
						head->next = itm;
					head = itm;
					itm->next = NULL;
					shr--;
				}
				
				deep--;
			}
		}
	}

	return tail;
}

mCols *fMatrix(GraphClutch *gTab) {
	mCols *mtrx = createMatrix(gTab);
	startMatrix(gTab, mtrx);
	FloydWarshall(gTab->n, mtrx);
	return mtrx;
}