typedef struct metaFile {
    int SIZE;
    FILE *fp;
    char *fileName;
} metaFile;

typedef struct Item {
    int key1;
    int key2;
    int offset;
    int len;//with \0
    int ind1;
    struct cluItem *ind2;
} Item;

typedef struct cluItem {
    Item *elem;
    struct cluItem *next;
    struct cluItem *prev;
} cluItem;

typedef struct spaceAdd {
    int n;
    int *freeCeil; //0 - free, 1 - busy, 2 - deleted
    Item **elem;
} spaceAdd;

typedef struct spaceClu {
    int n;
    cluItem **next;
} spaceClu;

typedef struct viewItem {
    Item *elem;
    int v1;
    int v2;
    struct viewItem *next;
} viewItem;


int keyToBytes(int key, int *bytes) {
    int N = 4;

    for (int i = 0; i < N; i++) {
        bytes[i] = key % 256;
        key /= 256;
    }

    return 0;
}

int hashClu(int *bytes, metaFile *meta) {
    int hash = 2139062143;
    int N = 4;
    for (int i = 0; i < N; i++) {
        hash = 37*hash + bytes[i];
    }

    return (hash > 0 ? hash : -hash) % meta->SIZE;

}

int delTable(spaceClu *, spaceAdd *, metaFile *);

void crash (spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    delTable(topClu, topAdd, meta);
    fclose(meta->fp);
    printf("File has damaged or your info is too big!\n");
    exit(0);
}

void fseek_d(FILE *fp, int offset, int mode, spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    int ret = fseek(fp, offset, mode);
    if (ret != 0) {
        crash(topAdd, topClu, meta);
    }
}

int load(char *fName, spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    FILE *fp = fopen(fName, "r+b");
    if(fp == NULL)
        return 1;

    meta->fp = fp;
    fread(&(meta->SIZE), sizeof(int), 1, meta->fp);

    Item **links = (Item **)calloc(meta->SIZE, sizeof(Item *));
    topAdd->elem = links;
    int *frCeil = (int *)calloc(meta->SIZE, sizeof(int));
    topAdd->freeCeil=frCeil;
    topClu->next = (cluItem **)calloc(meta->SIZE, sizeof(cluItem *));

    fread(&(topAdd->n), sizeof(int), 1, meta->fp);
    topClu->n = topAdd->n;

    //printf("Reading data from file.\nTable SIZE: %d\nElements in table: %d (%d)\n", meta->SIZE, topAdd->n, topClu->n);
    int elemPos;
    int elemStatus;
    int bytesClu[4];
    int ceilClu;
    int i;

    for (i = 0; i < meta->SIZE; i++) {
        fseek_d(meta->fp, 2*sizeof(int) + 2*i*(sizeof(int)), SEEK_SET, topAdd, topClu, meta);
        fread(&elemPos, sizeof(int), 1, meta->fp); //Optimize
        fread(&elemStatus, sizeof(int), 1, meta->fp);
        topAdd->freeCeil[i] = elemStatus;
    }

    int fCl;
    int curItm;
    for (i = 0; i < meta->SIZE; i++) {
        fseek_d(meta->fp, sizeof(int) * (2 + 2*meta->SIZE + i), SEEK_SET, topAdd, topClu, meta);
        fread(&fCl, sizeof(int), 1, meta->fp);

        while(fCl != 0) {
            cluItem *addElClu = (cluItem *) calloc(1, sizeof(cluItem));
            fseek_d(meta->fp, fCl, SEEK_SET, topAdd, topClu, meta);
            fread(&curItm, sizeof(int), 1, meta->fp);
            fseek_d(meta->fp, curItm, SEEK_SET, topAdd, topClu, meta);
            Item *newElem = (Item *) calloc(1, sizeof(Item));
            fread(newElem, sizeof(Item), 1, meta->fp);
            addElClu->elem = newElem;
            newElem->ind2 = addElClu;

            if (topClu->next[i] != NULL)
                topClu->next[i]->prev = addElClu;
            addElClu->next = topClu->next[i];
            addElClu->prev = NULL;
            topClu->next[i] = addElClu;

            fseek_d(meta->fp, fCl + sizeof(int), SEEK_SET, topAdd, topClu, meta);
            fread(&fCl, sizeof(int), 1, meta->fp);

            topAdd->elem[newElem->ind1] = newElem;
        }
    }

    return 0;
}

int create(char *fName, spaceAdd *topAdd, spaceClu *topClu, metaFile *meta, int tabCr) { //tabCr = 1 if we need to create local tables
    FILE *fp = fopen(fName, "w+b");
    if(fp == NULL)
        return 1;

    if (tabCr == 1) {
        Item **links = (Item **) calloc(meta->SIZE, sizeof(Item *));
        topAdd->elem = links;
        int *frCeil = (int *) calloc(meta->SIZE, sizeof(int));
        topAdd->freeCeil = frCeil;
        topClu->next = (cluItem **) calloc(meta->SIZE, sizeof(cluItem *));
    }

    fwrite(&meta->SIZE, sizeof(int), 1, fp);
    int zero = 0;
    fwrite(&zero, sizeof(int), 1, fp);
    int k;
    for(k = 0; k < meta->SIZE; k++) {
        fwrite(&zero, sizeof(int), 1, fp);
        fwrite(&zero, sizeof(int), 1, fp);
    }

    for(k = 0; k < meta->SIZE; k++)
        fwrite(&zero, sizeof(int), 1, fp);

    meta->fp = fp;
    return 0;
}

int hashAddStart(int key, metaFile *meta) {
    return key % meta->SIZE;
}

int hashAddNext(int busy, metaFile *meta) {
    return (busy + 37) % meta->SIZE;
}

int insert(int keyAdd, int keyClu, char *field, spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    if(topAdd->n == meta->SIZE) {
        return 1;
    }

    int bytesClu[4];
    keyToBytes(keyClu, bytesClu);
    int ceilClu = hashClu(bytesClu, meta);
    cluItem *addElClu = (cluItem *)calloc(1, sizeof(cluItem));

    Item *newEl = (Item *)calloc(1, sizeof(Item));

    if (topClu->next[ceilClu] != NULL)
        topClu->next[ceilClu]->prev = addElClu;
    addElClu->next = topClu->next[ceilClu];
    addElClu->prev = NULL;
    topClu->n++;
    topClu->next[ceilClu] = addElClu;

    int indAdd = hashAddStart(keyAdd, meta);
    while (1) {
        if (topAdd->freeCeil[indAdd] != 1) {
            topAdd->elem[indAdd] = newEl;
            topAdd->freeCeil[indAdd] = 1;
            topAdd->n++;
            break;
        }
        else {
            indAdd = hashAddNext(indAdd, meta);
        }
    }

    fseek_d(meta->fp, 0, SEEK_END, topAdd, topClu, meta);
    if(ftell(meta->fp) + strlen(field) > 2000000000) {
        crash(topAdd, topClu, meta);
    }
    newEl->key1 = keyAdd;
    newEl->key2 = keyClu;
    newEl->len = strlen(field) + 1;
    newEl->offset = ftell(meta->fp);
    newEl->ind1 = indAdd;
    newEl->ind2 = addElClu;
    addElClu->elem = newEl;

    fwrite(field, sizeof(char), newEl->len, meta->fp);

    return 0;
}


int addToView(viewItem **topView, Item *elem, int v1, int v2, metaFile *meta) {
    viewItem *newView = (viewItem *)calloc(1, sizeof(viewItem));
    Item *newElView = (Item *)calloc(1, sizeof(Item));
    newElView->len = elem->len;
    newElView->offset = elem->offset;
    newElView->key1 = elem->key1;
    newElView->key2 = elem->key2;
    newView->v1 = v1;
    newView->v2 = v2;
    newView->elem = newElView;
    (*topView)->next = newView;
    *topView = newView;

    return 0;
}

int clearView(viewItem *topView) {
    viewItem *curVw = topView;

    while(topView != NULL) {
        curVw = topView->next;
        free(topView->elem);
        free(topView);
        topView = curVw;
    }

    return 0;
}

int getVersionClu(cluItem *elemRes) {
    int v = 0;
    cluItem *curItm = elemRes;
    int keyClu = curItm->elem->key2;
    while(curItm != NULL) {
        if(curItm->elem->key2 == keyClu)
            v++;
        curItm = curItm->next;
    }

    return v;
}

int reverseV2(viewItem *topView, int countVer2) {
    int i = 0;
    while(topView != NULL) {
        topView->v2=countVer2-i;
        i++;
        topView = topView->next;
    }
}

int getVersionAdd(int keyAdd, int numAdd, spaceAdd *topAdd, metaFile *meta) {
    int keyPos = hashAddStart(keyAdd, meta);
    int v1 = 1;
    while(topAdd->freeCeil[keyPos] == 2 || topAdd->elem[keyPos]->ind1 != numAdd) {
        if (topAdd->freeCeil[keyPos] != 2 && topAdd->elem[keyPos]->key1 == keyAdd) {
            v1++;
        }

        keyPos = hashAddNext(keyPos, meta);
    }

    return v1;
}

viewItem *searchAdd(int keyAdd, spaceAdd *topAdd, metaFile *meta) {
    int keyPos = hashAddStart(keyAdd, meta);
    int n = 0;
    int v1 = 0;
    int v2;

    viewItem *headView = (viewItem *)calloc(1, sizeof(viewItem));
    headView->next = NULL;
    viewItem *pntr = headView;

    while (topAdd->freeCeil[keyPos] != 0 && n < meta->SIZE) {
        if(topAdd->freeCeil[keyPos] != 2 && topAdd->elem[keyPos]->key1 == keyAdd) {
            v1++;
            v2 = getVersionClu(topAdd->elem[keyPos]->ind2);
            addToView(&pntr, topAdd->elem[keyPos], v1, v2, meta);
        }
        keyPos = hashAddNext(keyPos, meta);
        n++;
    }

    pntr = headView;
    headView = headView->next;
    free(pntr);

    return headView;
}

viewItem *searchClu(int keyClu, spaceClu *topClu, spaceAdd *topAdd, metaFile *meta) {
    int bytesKey[4];
    keyToBytes(keyClu, bytesKey);
    int ceilClu = hashClu(bytesKey, meta);

    viewItem *headView = (viewItem *)calloc(1, sizeof(viewItem));
    headView->next = NULL;
    viewItem *pntr = headView;

    cluItem *curItm = topClu->next[ceilClu];
    int v1;
    int v2_all = 0;

    while (curItm != NULL) {
        if (curItm->elem->key2 == keyClu) {
            v2_all++;
            int v1 = getVersionAdd(curItm->elem->key1, curItm->elem->ind1, topAdd, meta);
            addToView(&pntr, curItm->elem, v1, 0, meta);

        }

        curItm = curItm->next;
    }

    pntr = headView;
    headView = headView->next;
    free(pntr);
    reverseV2(headView, v2_all);
    return headView;
}

int delClu(spaceClu *topClu, int ceil, cluItem *delItm) {
    if (delItm->prev == NULL) {
        topClu->next[ceil] = delItm->next;
    }
    else {
        delItm->prev->next = delItm->next;
    }

    if (delItm->next != NULL)
        delItm->next->prev = delItm->prev;
    free(delItm);

    return 0;
}

int del1Key(int key1, int v1, spaceClu *topClu, spaceAdd *topAdd, metaFile *meta) { //v1 = 0, если надо удалить все версии
    int keyPos = hashAddStart(key1, meta);
    int v1_all = 0;
    int bytesClu[4];
    int ceilClu;
    int deleted = 1;
    int n = 0;

    while(topAdd->freeCeil[keyPos] != 0 && n < meta->SIZE) {
        if(topAdd->freeCeil[keyPos] != 2 && topAdd->elem[keyPos]->key1 == key1) {
            v1_all++;
            if (v1 == v1_all || v1 == 0) {
                cluItem *ind2 = topAdd->elem[keyPos]->ind2;

                keyToBytes(topAdd->elem[keyPos]->key2, bytesClu);
                ceilClu = hashClu(bytesClu, meta);

                free(topAdd->elem[keyPos]);
                topAdd->elem[keyPos] = NULL;
                topAdd->freeCeil[keyPos] = 2;
                topAdd->n--;
                topClu->n--;
                delClu(topClu, ceilClu, ind2);

                deleted = 0;

                if (v1 > 0)
                    break;

            }
        }

        keyPos = hashAddNext(keyPos, meta);
        n++;
    }

    return deleted;
}

int del2Key(int key2, int v2, spaceClu *topClu, spaceAdd *topAdd, metaFile *meta) { //v2 = 0 если надо удалить все элементы
    int bytesClu[4];
    keyToBytes(key2, bytesClu);
    int ceilClu = hashClu(bytesClu, meta);
    int deleted = 1;

    cluItem *curItm = topClu->next[ceilClu];
    int v2_cur = 0;
    int v2_all = 0;

    if (v2 > 0) {
        while (curItm != NULL) {
            if (curItm->elem->key2 == key2) {
                v2_all++;
            }
            curItm = curItm->next;
        }
        curItm = topClu->next[ceilClu];
    }


    while(curItm != NULL) {
        if (curItm->elem->key2 == key2) {
            v2_cur++;
            if (v2_all - v2 + 1 == v2_cur || v2 == 0) {
                int ind1 = curItm->elem->ind1;
                free(curItm->elem);
                delClu(topClu, ceilClu, curItm);
                topAdd->elem[ind1] = NULL;
                topAdd->freeCeil[ind1] = 2;
                topAdd->n--;
                topClu->n--;
                deleted = 0;

                if (v2 > 0)
                    break;
            }

        }

        curItm = curItm->next;
    }

    return deleted;
}

int saveTable(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta, int saveInfo, metaFile *oldMeta) { //saveInfo == 1 if we need to save info [already not working]
    fseek_d(meta->fp, sizeof(int), SEEK_SET, topAdd, topClu, meta);
    fwrite(&(topAdd->n), sizeof(int), 1, meta->fp);
    int i;
    for (i = 0; i < meta->SIZE; i++) {
        fseek_d(meta->fp, 3*sizeof(int) + 2*i*(sizeof(int)), SEEK_SET, topAdd, topClu, meta);
        fwrite(&(topAdd->freeCeil[i]), sizeof(int), 1, meta->fp);

        int howClear;
        fseek_d(meta->fp, 2 * sizeof(int) + 2 * meta->SIZE * sizeof(int) + i * sizeof(int), SEEK_SET, topAdd, topClu, meta);
        fread(&howClear, sizeof(int), 1, meta->fp);
    }
    cluItem *curClu;
    char *reStr;
    int zero = 0;
    for (i = 0; i < meta->SIZE; i++) {
        fseek_d(meta->fp, 2 * sizeof(int) + 2 * meta->SIZE * sizeof(int) + i * sizeof(int), SEEK_SET, topAdd, topClu, meta);
        curClu = topClu->next[i];

        if(curClu == NULL) {
            fwrite(&zero, sizeof(int), 1, meta->fp);
        }
        else {
            int itemPlace, linkPlace, lastLink, pTab, oldTab;
            int fStep = 1;
            fseek_d(meta->fp, 2 * sizeof(int) + 2 * meta->SIZE * sizeof(int) + i * sizeof(int), SEEK_SET, topAdd, topClu, meta);
            fread(&pTab, sizeof(int), 1, meta->fp);

            while(curClu != NULL) {
                if(pTab == 0) {
                    fseek_d(meta->fp, 0, SEEK_END, topAdd, topClu, meta);
                }
                else {
                    fseek_d(meta->fp, pTab + sizeof(int), SEEK_SET, topAdd, topClu, meta);
                    oldTab = pTab;
                    fread(&pTab, sizeof(int), 1, meta->fp);
                    fseek_d(meta->fp, oldTab + sizeof(int), SEEK_SET, topAdd, topClu, meta);
                    fwrite(&zero, sizeof(int), 1, meta->fp);
                    if (fStep == 1) {
                        fseek_d(meta->fp, 2 * sizeof(int) + 2 * meta->SIZE * sizeof(int) + i * sizeof(int), SEEK_SET, topAdd, topClu, meta);
                        fwrite(&zero, sizeof(int), 1, meta->fp);
                        fStep = 0;
                    }

                    fseek_d(meta->fp, oldTab - sizeof(Item), SEEK_SET, topAdd, topClu, meta);
                }
                itemPlace = ftell(meta->fp);
                fwrite(curClu->elem, sizeof(Item), 1, meta->fp);
                linkPlace = ftell(meta->fp);
                fwrite(&itemPlace, sizeof(int), 1 , meta->fp);
                fseek_d(meta->fp, 2* sizeof(int) + meta->SIZE * 2 * sizeof(int) + i * sizeof(int), SEEK_SET, topAdd, topClu, meta);
                fread(&lastLink, sizeof(int), 1, meta->fp);
                fseek_d(meta->fp, 2* sizeof(int) + meta->SIZE * 2 * sizeof(int) + i * sizeof(int), SEEK_SET, topAdd, topClu, meta);
                fwrite(&linkPlace, sizeof(int), 1, meta->fp);
                fseek_d(meta->fp, linkPlace + sizeof(int), SEEK_SET, topAdd, topClu, meta);
                fwrite(&lastLink, sizeof(int), 1, meta->fp);

                fseek_d(meta->fp, 2 * sizeof(int) + 2*sizeof(int)*curClu->elem->ind1, SEEK_SET, topAdd, topClu, meta);
                fwrite(&itemPlace, sizeof(int), 1, meta->fp);

                curClu = curClu->next;
            }
        }
    }

    return 0;
}


int putInfo(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta, metaFile *newMeta) {
    fseek_d(newMeta->fp, 0, SEEK_END, topAdd, topClu, meta);
    int ofst;
    for(int i = 0; i < newMeta->SIZE; i++) {
        if(topAdd->freeCeil[i] == 1) {
            char *buf = (char *)calloc(topAdd->elem[i]->len, sizeof(char));
            ofst = topAdd->elem[i]->offset;
            topAdd->elem[i]->offset = ftell(newMeta->fp);
            fseek_d(meta->fp, ofst, SEEK_SET, topAdd, topClu, meta);
            fread(buf, sizeof(char), topAdd->elem[i]->len, meta->fp);
            fwrite(buf, sizeof(char), topAdd->elem[i]->len, newMeta->fp);
            free(buf);
        }
    }
    return 0;
}

int colGarb(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    char sgName[40];
    metaFile newMeta;
    strcpy(sgName, meta->fileName);
    strcat(sgName, "sg");
    newMeta.fileName = sgName;
    newMeta.SIZE = meta->SIZE;
    if(create(sgName, topAdd, topClu, &newMeta, 0) == 1)
        return 1;
    putInfo(topAdd, topClu, meta, &newMeta);
    saveTable(topAdd, topClu, &newMeta, 1, meta);
    fclose(meta->fp);
    remove(meta->fileName);
    rename(newMeta.fileName, meta->fileName);
    newMeta.fileName = meta->fileName;
    *meta = newMeta;

    return 0;
}

int delTable(spaceClu *topClu, spaceAdd *topAdd, metaFile *meta) {
    for (int curClu= 0; curClu < meta->SIZE; curClu++) {
        while(topClu->next[curClu] != NULL) {
            free(topClu->next[curClu]->elem);
            delClu(topClu, curClu, topClu->next[curClu]);
        }
    }

    free(topAdd->elem);
    free(topAdd->freeCeil);
    free(topClu->next);
    free(meta->fileName);

    return 0;
}