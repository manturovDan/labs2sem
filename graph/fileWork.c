int writeGraph(GraphClutch *gTab, char *fName) {
	FILE *fp;
	fp = fopen(fName, "wt");

	if (fp == 0)
		return 1;

	for (int i = 0; i < SIZE; i++) {
		CluItem *cont = gTab->grTab[i];
		while (cont != NULL) {
			fprintf(fp, "%d\n%f\n%f\n", cont->fNode->name, cont->fNode->x, cont->fNode->y);
			Neighbour *nbr = cont->fNode->nbr;
			while (nbr != NULL) {
				fprintf(fp, "%d\n", nbr->name);
				nbr = nbr->next;
			}
			fprintf(fp, "-1\n");
			cont = cont->next;
		}
	}

	fclose(fp);
	return 0;
}

int readGraph(GraphClutch *gTab, char *fName) {
	FILE *fp;
	if ((fp = fopen(fName, "rt")) == 0) {
		return 1;
	}

	int name, way, st;
	float x, y;
	while (1) {
		if (fscanf(fp, "%d", &name) <= 0 ||
		fscanf(fp, "%f", &x) <= 0 ||
		fscanf(fp, "%f", &y) <= 0 ||
		fscanf(fp, "%d", &way) <= 0) {
			break;
		}
		if (adding(gTab, name, x, y) != 0)
			return 1;

		while(way != -1)
			if (fscanf(fp, "%d", &way) <= 0)
				break;
	}


	fseek(fp, 0, SEEK_SET);

	while (1) {
		if (fscanf(fp, "%d", &name) <= 0 ||
		fscanf(fp, "%f", &x) <= 0 ||
		fscanf(fp, "%f", &y) <= 0 ||
		fscanf(fp, "%d", &way) <= 0) {
			break;
		}

		while(way != -1) {
			if (edge(gTab, name, way) != 0)
				return 1;
			if (fscanf(fp, "%d", &way) <= 0)
				return 1;
		}
	}

	fclose(fp);
	return 0;
}