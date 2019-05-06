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

int adding(GraphClutch *gTab, int name, float x, float y) { //returns: 2 - is already exist
	if (find(gTab, name) != NULL) {
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

	if(delItm == 0)
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
	free(cont);

	Neighbour *deln;
	while (nbr != NULL) {
		deln = nbr;
		nbr = nbr->next;
		free(deln);
	}

	gTab->n--;

}