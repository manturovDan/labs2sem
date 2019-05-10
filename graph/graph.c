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

struct Road {
	int start;
	int end;
	float dist;
	struct Road *last; //last in way
	struct Road *nextg; //next in list
} typedef Road;

struct mItm {
	float **dist;
	Road **track;
	int *names;
} typedef mItm;

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

int idByName(mItm *matrix, int name, int sz) {
	//TODO QSort, Binsearch
	for (int i = 0; i < sz; i++) {
		if(matrix->names[i] == name) {
			return i;
		}
	}
	return -1;
}

mItm *createMatrix(GraphClutch *gTab) {
	mItm *repo = (mItm *)calloc(1, sizeof(mItm));
	repo->dist = (float **)calloc(gTab->n, sizeof(float *));
	for (int i = 0; i < gTab->n; i++) {
		repo->dist[i] = (float *)calloc(gTab->n, sizeof(float));
		for (int j = 0; j < gTab->n; j++) {
			if(i == j) 
				repo->dist[i][j] = 0;
			else
				repo->dist[i][j] = -1;
		}
	}

	repo->names = (int *)calloc(gTab->n, sizeof(int));
	int n = 0;
	for (int t = 0; t < SIZE; t++) {
		CluItem *cont = gTab->grTab[t];
		while(cont != NULL) {
			repo->names[n] = cont->fNode->name;
			n++;
			cont = cont->next;
		}
	}

	repo->track = (Road **)calloc(1, sizeof(Road *));

	return repo;
}

int clearMatrix(int sz, mItm *matrix) {
	for (int i = 0; i < sz; i++) {
		free(matrix->dist[i]);
	}
	free(matrix->dist);
	free(matrix->names);
	Road *step = *matrix->track;
	while (step != NULL) { 
		Road *del = step;
		step = step->nextg;
		free(del);
	}

	free(matrix->track);
	free(step);
	free(matrix);
	return 0;
}

int clearRoad(int sz, Road ****way) {
	for(int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			free(way[i][j]);
		}
		free(way[i]);
	}
	free(way);
}

int startMatrix(GraphClutch *gTab, mItm *matrix) {
	for (int s = 0; s < gTab->n; s++) {
		CluItem *cNode = find(gTab, matrix->names[s]);
		Neighbour *nbr = cNode->fNode->nbr;
		int id;
		while (nbr != NULL) {
			id = idByName(matrix, nbr->name, gTab->n);
			matrix->dist[s][id] = nbr->dist;
			nbr = nbr->next;
		}

		matrix->dist[s][s] = 0;
	}

	return 0;
}

int checkInTrack(Road *last, int node) {
	while (last != NULL) {
		if (last->end == node) 
			return 1;
		last = last->last;
	}

	return 0;
}

int deep(Road *last, mItm *matrix, GraphClutch *gTab) {
	for (int j = 0; j < gTab->n; j++) {
		if (matrix->dist[last->end][j] > 0 && checkInTrack(last, j) == 0) {
			Road *way = (Road *)calloc(1, sizeof(Road));
			if (way == NULL) {
				clearMatrix(gTab->n, matrix);
				printf("Memory error!\n");
				exit(0);
			}
			way->last = last;
			way->start = last->start;
			way->end = j;
			way->dist = last->dist + matrix->dist[last->end][j];
			if (matrix->track != NULL) 
				way->nextg = *matrix->track;
			*matrix->track = way;
			deep(way, matrix, gTab);
		}
	}

	return 0;
}

int runSearch(mItm *matrix, GraphClutch *gTab) {
	for (int i = 0; i < gTab->n; i++) {
		Road *first = (Road *)calloc(1, sizeof(Road));
		first->start = i;
		first->end = i;
		first->dist = 0;
		first->last = NULL;
		first->nextg = *matrix->track;
		deep(first, matrix, gTab);
		free(first);
	}
	return 0;
}

Road ****optMx(int sz) {
	Road ****opt = (Road ****)calloc(sz, sizeof(Road ***));
	for (int i = 0; i < sz; i++) {
		opt[i] = (Road ***)calloc(sz, sizeof(Road **));
		for (int j = 0; j < sz; j++) {
			opt[i][j] = (Road **)calloc(3, sizeof(Road *));
			for (int k = 0; k < 3; k++) {
				opt[i][j][k] = NULL;
			}
		}
	}

	return opt;
}

int findMax3(Road **ways) {
	int mval = 0;
	int mx = -1;
	for (int i = 0; i < 3; i++) {
		if(ways[i]->dist > mval) {
			mval = ways[i]->dist;
			mx = i;
		}
	}

	return mx;
}

int getOptimal(Road ****roads, mItm *matrix, GraphClutch *gTab) {
	Road *step = *matrix->track;
	while (step != NULL) {
		int toRec = -1;
		for(int k = 0; k < 3; k++) {
			if(roads[step->start][step->end][k] == NULL) {\
				toRec = k;
				break;
			}
		}

		if(toRec == -1) {
			int mx = findMax3(roads[step->start][step->end]);
			if (roads[step->start][step->end][mx]->dist > step->dist)
				toRec = mx;
		}

		if (toRec != -1) {
			roads[step->start][step->end][toRec] = step;
		}

		step = step->nextg;
	}

	for (int i = 0; i < gTab->n; i++) {
		for(int j = 0; j < gTab->n; j++) {
			if(roads[i][j][0] == NULL || roads[i][j][1] == NULL){
				continue;
			} 

			if (roads[i][j][2] != NULL) {
				int mx = findMax3(roads[i][j]);
				Road *move = roads[i][j][2];
				roads[i][j][2] = roads[i][j][mx];
				roads[i][j][mx] = move;
			}

			if(roads[i][j][0] > roads[i][j][1]) {
				Road *move = roads[i][j][1];
				roads[i][j][1] = roads[i][j][0];
				roads[i][j][0] = move;
			}

			
		}
	}
	return 0;
}

mItm *fMatrix(GraphClutch *gTab) {
	mItm *mtrx = createMatrix(gTab);
	startMatrix(gTab, mtrx);
	runSearch(mtrx, gTab);	
	return mtrx;
}