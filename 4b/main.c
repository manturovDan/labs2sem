#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileProc.c"

int get_nInt(int *a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n == 0) {
            printf("Incorrect input. Try again!\n");
            scanf("%*f^\n");
            scanf("%*s");
            continue;
        }

        if (*a < 0 || *a > 100000000) {
            printf("Input natural number <= 100M\n");
            n = 0;
        }
    } while(n == 0);

    scanf("%*1[\n]");
    return n < 0 ? 0 : 1;
}

char *getStr() {
    char buf[81];
    char *ptr = (char *)calloc(1, sizeof(char));
    if (ptr == NULL) {
        printf("No memory");
        exit(0);
    }
    short int n = 0;
    int len = 0;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
        }
        else if (n == 0) {
            scanf("%*1[\n]");
        }
        else {
            len += strlen(buf);
            ptr = (char*)realloc(ptr, len+1);
            if (ptr == NULL) {
                printf("No memory");
                exit(0);
            }
            strcat(ptr, buf);
        }

    } while (n > 0);

    return ptr;
}

int showTable(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    int i;
    printf("Clutch space:\n");
    for (i = 0; i < meta->SIZE; i++) {
        cluItem *nxMem;
        nxMem = topClu->next[i];

        if(nxMem != NULL) {
            char *infoStr = (char *)calloc(nxMem->elem->len, sizeof(char));
            fseek(meta->fp, nxMem->elem->offset, SEEK_SET);
            fread(infoStr, nxMem->elem->len, sizeof(char), meta->fp);

            printf("#%d-->((%p < %p > %p) { addr: %p, key1: %d, key2: %d, info: %s, len: %d, ind1: %d, ind2: %p })",
                   i, nxMem->prev, nxMem, nxMem->next, nxMem->elem, nxMem->elem->key1,
                   nxMem->elem->key2, infoStr, nxMem->elem->len, nxMem->elem->ind1, nxMem->elem->ind2);
            free(infoStr);
        }

        while(nxMem != NULL) {
            if(nxMem->next == NULL)
                break;
            nxMem = nxMem->next;

            char *infoStr = (char *)calloc(nxMem->elem->len, sizeof(char));
            fseek(meta->fp, nxMem->elem->offset, SEEK_SET);
            fread(infoStr, nxMem->elem->len, sizeof(char), meta->fp);

            printf("\t<-> ((%p < %p > %p) { addr: %p, key1: %d, key2: %d, info: %s, ind1: %d, ind2: %p })", nxMem->prev,
                   nxMem, nxMem->next, nxMem->elem, nxMem->elem->key1,
                   nxMem->elem->key2, infoStr, nxMem->elem->ind1, nxMem->elem->ind2);
            free(infoStr);
        }
        if(nxMem != NULL)
            printf("\n");

    }

    printf("\nAdd space:\n");
    for(i = 0; i < meta->SIZE; i++) {
        if (topAdd->elem[i] != NULL || topAdd->freeCeil[i] != 0)
            printf("#%d [%d] (%p)\n", i, topAdd->freeCeil[i], topAdd->elem[i]);
    }

    printf("\n1: %d elements\n2: %d elements\n", topAdd->n, topClu->n);
}

int showView(viewItem *topView, metaFile *meta) {
    viewItem *topViewDub = topView;
    if(topView == NULL) {
        printf("Not found!\n");
        return 1;
    }

    printf("Founded data:\n\n{\n");
    while(topView != NULL) {
        char *infoStr = (char *)calloc(topView->elem->len, sizeof(char));
        fseek(meta->fp, topView->elem->offset, SEEK_SET);
        fread(infoStr, topView->elem->len, sizeof(char), meta->fp);

        printf("\t{\n\t\tv1: %d,\n\t\tv2: %d,\n\t\t"
               "Item:\n\t\t{\n\t\t\tkey1: %d,\n\t\t\tkey2: %d,\n\t\t\t"
               "info: %s\n\t\t}\n\t},\n", topView->v1, topView->v2, topView->elem->key1, topView->elem->key2, infoStr);
        topView = topView->next;
        free(infoStr);
    }
    printf("\n}\n");

    clearView(topViewDub);
    return 0;
}

int D_Add(spaceAdd *, spaceClu *, metaFile *),
        D_Find(spaceAdd *, spaceClu *, metaFile *),
        D_Delete(spaceAdd *, spaceClu *, metaFile *),
        D_Show(spaceAdd *, spaceClu *, metaFile *),
        showTable(spaceAdd *, spaceClu *, metaFile *),
        D_Garbage(spaceAdd *, spaceClu *, metaFile *),
        D_Save(spaceAdd *, spaceClu *, metaFile *);

int (*fptr[])(spaceAdd *, spaceClu *, metaFile *) = { NULL, D_Add, D_Find, D_Delete, D_Show, D_Save, D_Garbage };

int D_Load(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    char *fName = NULL;
    printf("Enter DB file name:\n");
    fName = getStr();
    if (fName == NULL) {
        printf("The end!\n");
        exit(0);
    }

    meta->fileName = fName;

    if (load(fName, topAdd, topClu, meta) == 1) {
        printf("File doesn't exists, creating new one!\nEnter SIZE of both tables:\n");
        get_nInt(&(meta->SIZE));
        if(meta->SIZE == 0)
            return 1;

        if (create(fName, topAdd, topClu, meta, 1) == 1)
            return 1;

        printf("New file created\n");
    }

    return 0;
}

int D_Add(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    int key1, key2;
    char *t_field = NULL;
    printf("Enter first key (Add space): \n");
    get_nInt(&key1);
    printf("Enter second key (Clutch space): \n");
    get_nInt(&key2);
    printf("Enter string of info: \n");
    t_field = getStr();
    if (insert(key1, key2, t_field, topAdd, topClu, meta) == 1) {
        printf("Adding item error!\n");
        return 1;
    }
    free(t_field);
    printf("Successful adding!\n");
    return 0;
}

int D_Find(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    int choice = 2;
    printf("Print 1 to find by key from 1st space\nprint 2 to find by key from 2nd space:\n");
    get_nInt(&choice);
    viewItem *searchRes;
    if (choice == 1) {
        int key1;
        printf("Input key from 1st space:\n");
        get_nInt(&key1);

        searchRes = searchAdd(key1, topAdd, meta);
    }
    else if (choice == 2) {
        int key2;
        printf("Input key from 2nd space:\n");
        get_nInt(&key2);

        searchRes = searchClu(key2, topClu, topAdd, meta);

    }
    else {
        printf("Incorrect input!\n");
        return 1;
    }

    showView(searchRes, meta);

    return 0;
}

int D_Delete(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    int delMode; //1 - delete oll versions, 2 - delete one version
    int whichSpace;
    int version = 0;
    int key;
    int del;
    printf("Input 1 to delete all versions of element or 2 to delete "
           "one version of element:\n");
    get_nInt(&delMode);

    if (delMode == 2) {
        printf("Input version of element:\n");
        get_nInt(&version);
    }
    else if(delMode != 1) {
        printf("Incorrect input!\n");
        return 1;
    }

    printf("Input 1 to delete by key from 1st space,\n"
           "Input 2 to delete by key from 2nd space:\n");
    get_nInt(&whichSpace);
    if(whichSpace > 2 || whichSpace < 1) {
        printf("Incorrect input!\n");
        return 1;
    }

    printf("Input key of element from %d space:\n", whichSpace);
    get_nInt(&key);

    if (whichSpace == 1) {
        del = del1Key(key, version, topClu, topAdd, meta);
    }
    else if (whichSpace == 2) {
        del = del2Key(key, version, topClu, topAdd, meta);
    }
    else {
        printf("Incorrect input!\n");
        return 1;
    }

    if(del == 1) {
        printf("Not found!\n");
        return 1;
    }

    printf("Successfully deleted!\n");
    return 0;
}

int D_Show(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    showTable(topAdd, topClu, meta);
}

int D_Save(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    saveTable(topAdd, topClu, meta, 0, NULL);

    return 0;
}

int D_Garbage(spaceAdd *topAdd, spaceClu *topClu, metaFile *meta) {
    D_Save(topAdd, topClu, meta);
    if (colGarb(topAdd, topClu, meta) == 0) {
        printf("Successfully cleared!\n");
    }
    else {
        printf("Error!\n");
    }
    return 0;
}


const char *msgs[] = {"Menu:\n0. Save and quit", "1. Add element to table",
                      "2. Find element", "3. Delete element",
                      "4. Show table", "5. Save data in file",
                      "6. Launch garbage collection"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int dialog(const char *msgs[], int N) {
    char *errmsg = "";
    int rc;
    int i, n;

    do {
        puts(errmsg);
        errmsg = "You are wrong. Repeat, please!";

        for (i = 0; i < N; i++) {
            puts(msgs[i]);
        }
        puts("Choose the operation");

        n = get_nInt(&rc);

        if (n == 0)
            rc = 0;

    } while(rc < 0 || rc >= N);

    return rc;
}

int main () {
    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;
    int rc;

    keyAdd.n = 0;
    keyClu.n = 0;

    while (1) {
        if (D_Load(&keyAdd, &keyClu, &meta) == 0) {
            printf("Data loaded successfully!\n");
            break;
        }

        printf("Error in data loading.\nEnter any word to try again or exit by ^D\n");
        char *ex = getStr();
        if(ex == NULL) {
            printf("The end!\n");
            free(ex);
            exit(0);
        }

    }

    while (rc = dialog(msgs, NMsgs)) {
        fptr[rc](&keyAdd, &keyClu, &meta);
    }

    D_Save(&keyAdd, &keyClu, &meta);
    delTable(&keyClu, &keyAdd, &meta);
    fclose(meta.fp);
    return 0;
}