//tests.cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileProc.c"
#include <gtest/gtest.h>

int busyFieldsAdd(spaceAdd *topAdd, metaFile *meta) { //считает количество элементов в таблице сложения
    int busy = 0;
    for (int i = 0; i < meta->SIZE; i++) {
        if (topAdd->elem[i] != NULL)
            busy++;
    }
    return busy;
}

int countElemsClu(spaceClu *topClu, metaFile *meta) { //считает количество элементов в таблице сцепления
    int busy = 0;
    for (int i = 0; i < meta->SIZE; i++) {
        cluItem *curItm = topClu->next[i];
        while (curItm != NULL) {
            busy++;
            curItm = curItm->next;
        }
    }

    return busy;
}

int nAdd(spaceAdd *topAdd) {
    return topAdd->n;
}

int nClu(spaceClu *topClu) {
    return topClu->n;
}

char *getString(Item *itm, metaFile *meta) {
    char *str = (char *)calloc(itm->len, sizeof(char));
    fseek(meta->fp, itm->offset, SEEK_SET);
    fread(str, sizeof(char), itm->len, meta->fp);
    return str;
}

TEST(SimpleTest, STest1) {
    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test1text.db";
    meta.fileName = fName;
    meta.SIZE = 50;
    create(fName, &keyAdd, &keyClu, &meta, 1);

    char *t = "test string 1";
    char *t_fld = (char *)calloc(strlen(t), sizeof(char));
    strcpy(t_fld, t);

    insert(1, 1, t_fld, &keyAdd, &keyClu, &meta);
    ASSERT_EQ(busyFieldsAdd(&keyAdd, &meta), 1);
    ASSERT_EQ(countElemsClu(&keyClu, &meta), 1);
    ASSERT_EQ(nAdd(&keyAdd), 1);
    ASSERT_EQ(nClu(&keyClu), 1);

    viewItem *searchRes1;
    searchRes1 = searchAdd(1, &keyAdd, &meta);
    ASSERT_EQ(searchRes1->v1, 1);
    ASSERT_EQ(searchRes1->v2, 1);
    ASSERT_EQ(searchRes1->elem->key1, 1);
    ASSERT_EQ(searchRes1->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes1->elem, &meta), t_fld);

    viewItem *searchRes2;
    searchRes2 = searchClu(1, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(searchRes2->v1, 1);
    ASSERT_EQ(searchRes2->v2, 1);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes1->elem, &meta), t_fld);;

    int del = del1Key(1, 1, &keyClu, &keyAdd, &meta);

    ASSERT_EQ(nAdd(&keyAdd), 0);
    ASSERT_EQ(nClu(&keyClu), 0);

    fclose(meta.fp);
    remove(fName);
}

TEST(ModTest, MTest) {
    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test2ext.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    create(fName, &keyAdd, &keyClu, &meta, 1);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);
    insert(1, 1, t_fld1, &keyAdd, &keyClu, &meta);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld2, t2);
    insert(38, 101, t_fld2, &keyAdd, &keyClu, &meta); // добавляется во 2 таблицу туда же, куда и 1й

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld3, t3);
    insert(1, 1, t_fld3, &keyAdd, &keyClu, &meta);

    char *t4 = "test string 4 _______________________________________________________________________________________________________________________aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                lol ";
    char *t_fld4 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld4, t4);
    insert(1, 2, t_fld4, &keyAdd, &keyClu, &meta);


    viewItem *searchRes1;
    searchRes1 = searchAdd(1, &keyAdd, &meta);
    ASSERT_EQ(searchRes1->v1, 1);
    ASSERT_EQ(searchRes1->v2, 1);
    ASSERT_EQ(searchRes1->elem->key1, 1);
    ASSERT_EQ(searchRes1->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes1->elem, &meta), t_fld1);

    searchRes1 = searchRes1->next;
    ASSERT_EQ(searchRes1->v1, 2);
    ASSERT_EQ(searchRes1->v2, 2);
    ASSERT_EQ(searchRes1->elem->key1, 1);
    ASSERT_EQ(searchRes1->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes1->elem, &meta), t_fld3);

    searchRes1 = searchRes1->next;
    ASSERT_EQ(searchRes1->v1, 3);
    ASSERT_EQ(searchRes1->v2, 1);
    ASSERT_EQ(searchRes1->elem->key1, 1);
    ASSERT_EQ(searchRes1->elem->key2, 2);
    ASSERT_STREQ(getString(searchRes1->elem, &meta), t_fld4);

    viewItem *searchRes2;
    searchRes2 = searchClu(1, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(searchRes2->v1, 2);
    ASSERT_EQ(searchRes2->v2, 2);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld3);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 1);
    ASSERT_EQ(searchRes2->v2, 1);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld1);

    fclose(meta.fp);
    remove(fName);

}

TEST (ModTest, MTestAdd) {
    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    int del;
    int k;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test3text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    create(fName, &keyAdd, &keyClu, &meta, 1);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);
    insert(1, 1, t_fld1, &keyAdd, &keyClu, &meta);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld2, t2);
    insert(1, 101, t_fld2, &keyAdd, &keyClu, &meta); // добавляется во 2 таблицу туда же, куда и 1й

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld3, t3);
    insert(1, 1, t_fld1, &keyAdd, &keyClu, &meta);

    del = del1Key(1, 2, &keyClu, &keyAdd, &meta);

    char *t4 = "test string 4";
    char *t_fld4 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld4, t3);
    insert(1, 1, t_fld4, &keyAdd, &keyClu, &meta);

    viewItem *searchRes11 = searchAdd(1, &keyAdd, &meta);
    ASSERT_EQ(searchRes11->v1, 1);
    ASSERT_EQ(searchRes11->v2, 1);
    ASSERT_EQ(searchRes11->elem->key1, 1);
    ASSERT_EQ(searchRes11->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes11->elem, &meta), t_fld1);

    searchRes11 = searchRes11->next;
    ASSERT_EQ(searchRes11->v1, 2);
    ASSERT_EQ(searchRes11->v2, 3);
    ASSERT_EQ(searchRes11->elem->key1, 1);
    ASSERT_EQ(searchRes11->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes11->elem, &meta), t_fld4);

    searchRes11 = searchRes11->next;
    ASSERT_EQ(searchRes11->v1, 3);
    ASSERT_EQ(searchRes11->v2, 2);
    ASSERT_EQ(searchRes11->elem->key1, 1);
    ASSERT_EQ(searchRes11->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes11->elem, &meta), t_fld1);

    saveTable(&keyAdd, &keyClu, &meta, 0, NULL);
    fclose(meta.fp);
}

TEST (ModTest, MTestLoad) {
    int del;
    int k;

    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test3text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    load(fName, &keyAdd, &keyClu, &meta);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);

    viewItem *searchRes11 = searchAdd(1, &keyAdd, &meta);
    ASSERT_EQ(searchRes11->v1, 1);
    ASSERT_EQ(searchRes11->v2, 1);
    ASSERT_EQ(searchRes11->elem->key1, 1);
    ASSERT_EQ(searchRes11->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes11->elem, &meta), t_fld1);

    fclose(meta.fp);
    remove(fName);
}

TEST (DelTest, dTest1) {
    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    int del;
    int k;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test4text.db";
    meta.fileName = fName;
    meta.SIZE = 1200;
    create(fName, &keyAdd, &keyClu, &meta, 1);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);
    insert(1, 1, t_fld1, &keyAdd, &keyClu, &meta);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld2, t2);
    insert(38, 101, t_fld2, &keyAdd, &keyClu, &meta); // добавляется во 2 таблицу туда же, куда и 1й

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld3, t3);
    insert(40, 1, t_fld3, &keyAdd, &keyClu, &meta);

    char *t4 = "test string 4 _______________________________________________________________________________________________________________________aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                lol ";
    char *t_fld4 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld4, t4);
    insert(1, 2, t_fld4, &keyAdd, &keyClu, &meta);

    char *t5 = "test string 55 _______________________________________________________________________________________________________________________aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                lol ";
    char *t_fld5 = (char *)calloc(strlen(t5), sizeof(char));
    strcpy(t_fld5, t5);
    insert(1, 2001, t_fld5, &keyAdd, &keyClu, &meta);

    char *t6 = "tessssttt string 6$$$";
    char *t_fld6 = (char *)calloc(strlen(t6), sizeof(char));
    strcpy(t_fld6, t6);
    insert(40, 2, t_fld6, &keyAdd, &keyClu, &meta);

    del = del1Key(40, 0, &keyClu, &keyAdd, &meta);

    viewItem *searchRes1;
    searchRes1 = searchAdd(40, &keyAdd, &meta);
    if (searchRes1 == NULL)
    k = 0;
    else
    k = 1;
    ASSERT_EQ(k, 0);

    del = del1Key(1, 2, &keyClu, &keyAdd, &meta);
    viewItem *searchRes11 = searchAdd(1, &keyAdd, &meta);
    ASSERT_EQ(searchRes11->v1, 1);
    ASSERT_EQ(searchRes11->v2, 1);
    ASSERT_EQ(searchRes11->elem->key1, 1);
    ASSERT_EQ(searchRes11->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes11->elem, &meta), t_fld1);

    searchRes11 = searchRes11->next;
    ASSERT_EQ(searchRes11->v1, 2);
    ASSERT_EQ(searchRes11->v2, 1);
    ASSERT_EQ(searchRes11->elem->key1, 1);
    ASSERT_EQ(searchRes11->elem->key2, 2001);
    ASSERT_STREQ(getString(searchRes11->elem, &meta), t_fld5);

    searchRes11 = searchRes11->next;
    if (searchRes11 == NULL)
    k = 0;
    else
    k = 1;
    ASSERT_EQ(k, 0);

    ASSERT_EQ(nAdd(&keyAdd), 3);
    ASSERT_EQ(nClu(&keyClu), 3);

    fclose(meta.fp);
    remove(fName);
}

TEST (DelTest, dTest2) {
    int del;
    int k;

    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test51text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    create(fName, &keyAdd, &keyClu, &meta, 1);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);
    insert(1, 1, t_fld1, &keyAdd, &keyClu, &meta);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld2, t2);
    insert(38, 101, t_fld2, &keyAdd, &keyClu, &meta); // добавляется во 2 таблицу туда же, куда и 1й

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld3, t3);
    insert(4, 1, t_fld3, &keyAdd, &keyClu, &meta);

    char *t4 = "test string 4 _______________________________________________________________________________________________________________________aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                lol ";
    char *t_fld4 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld4, t4);
    insert(1, 2, t_fld4, &keyAdd, &keyClu, &meta);

    char *t5 = "tessssttt string 5$$$";
    char *t_fld5 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld5, t5);
    insert(40, 2, t_fld5, &keyAdd, &keyClu, &meta);

    del = del2Key(1, 2, &keyClu, &keyAdd, &meta);
    viewItem *searchRes2;
    searchRes2 = searchClu(1, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(searchRes2->v1, 1);
    ASSERT_EQ(searchRes2->v2, 1);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld1);
    if (searchRes2->next == NULL)
        k = 0;
    else
        k = 1;
    ASSERT_EQ(k, 0);

    saveTable(&keyAdd, &keyClu, &meta, 0, NULL);
    fclose(meta.fp);
}

TEST (DelTest, DelTestLoad) {
    int del;
    int k;

    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test51text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    load(fName, &keyAdd, &keyClu, &meta);

    del = del1Key(4, 0, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(del, 1);

    del = del2Key(2, 0, &keyClu, &keyAdd, &meta);
    viewItem *searchRes21;
    searchRes21 = searchClu(2, &keyClu, &keyAdd, &meta);
    if (searchRes21 == NULL)
        k = 0;
    else
        k = 1;
    ASSERT_EQ(k, 0);
    ASSERT_EQ(nClu(&keyClu), 2);

    fclose(meta.fp);
    remove(fName);
}

TEST (DelTest, DelVersionFrom2) {
    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;
    int del;
    
    keyAdd.n = 0;
    keyClu.n = 0;
    
    char *fName = "test6text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    create(fName, &keyAdd, &keyClu, &meta, 1);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);
    insert(1, 1, t_fld1, &keyAdd, &keyClu, &meta);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t2), sizeof(char));
    strcpy(t_fld2, t2);
    insert(1, 1, t_fld2, &keyAdd, &keyClu, &meta);

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t3), sizeof(char));
    strcpy(t_fld3, t3);
    insert(1, 1, t_fld3, &keyAdd, &keyClu, &meta);

    char *t4 = "test string 4";
    char *t_fld4 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld4, t4);
    insert(1, 1, t_fld4, &keyAdd, &keyClu, &meta);

    del = del2Key(1, 3, &keyClu, &keyAdd, &meta);

    viewItem *searchRes2 = searchClu(1, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(searchRes2->v1, 3);
    ASSERT_EQ(searchRes2->v2, 3);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld4);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 2);
    ASSERT_EQ(searchRes2->v2, 2);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld2);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 1);
    ASSERT_EQ(searchRes2->v2, 1);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld1);

    ASSERT_EQ(nClu(&keyClu), 3);

    saveTable(&keyAdd, &keyClu, &meta, 0, NULL);
    fclose(meta.fp);
}

TEST (DelTest, DelTestLoadStrong) {
    int del;
    int k;

    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test6text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    load(fName, &keyAdd, &keyClu, &meta);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t2), sizeof(char));
    strcpy(t_fld2, t2);

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t3), sizeof(char));
    strcpy(t_fld3, t3);

    char *t4 = "test string 4";
    char *t_fld4 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld4, t4);

    viewItem *searchRes2 = searchClu(1, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(searchRes2->v1, 3);
    ASSERT_EQ(searchRes2->v2, 3);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld4);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 2);
    ASSERT_EQ(searchRes2->v2, 2);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld2);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 1);
    ASSERT_EQ(searchRes2->v2, 1);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld1);

    saveTable(&keyAdd, &keyClu, &meta, 0, NULL);
    colGarb(&keyAdd, &keyClu, &meta);
    fclose(meta.fp);
}

TEST (DelTest, DelTestGarbageCollect) {
    int del;
    int k;

    metaFile meta;
    spaceAdd keyAdd;
    spaceClu keyClu;

    keyAdd.n = 0;
    keyClu.n = 0;

    char *fName = "test6text.db";
    meta.fileName = fName;
    meta.SIZE = 100;
    load(fName, &keyAdd, &keyClu, &meta);

    char *t1 = "test string 1";
    char *t_fld1 = (char *)calloc(strlen(t1), sizeof(char));
    strcpy(t_fld1, t1);

    char *t2 = "test string 2";
    char *t_fld2 = (char *)calloc(strlen(t2), sizeof(char));
    strcpy(t_fld2, t2);

    char *t3 = "test string 3";
    char *t_fld3 = (char *)calloc(strlen(t3), sizeof(char));
    strcpy(t_fld3, t3);

    char *t4 = "test string 4";
    char *t_fld4 = (char *)calloc(strlen(t4), sizeof(char));
    strcpy(t_fld4, t4);

    viewItem *searchRes2 = searchClu(1, &keyClu, &keyAdd, &meta);
    ASSERT_EQ(searchRes2->v1, 3);
    ASSERT_EQ(searchRes2->v2, 3);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld4);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 2);
    ASSERT_EQ(searchRes2->v2, 2);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld2);

    searchRes2 = searchRes2->next;
    ASSERT_EQ(searchRes2->v1, 1);
    ASSERT_EQ(searchRes2->v2, 1);
    ASSERT_EQ(searchRes2->elem->key1, 1);
    ASSERT_EQ(searchRes2->elem->key2, 1);
    ASSERT_STREQ(getString(searchRes2->elem, &meta), t_fld1);

    fclose(meta.fp);
    remove(fName);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}