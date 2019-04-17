//agree that key compares in spaces by min<=x<=max
//SIZE of side of quadrant, keys are from -SIZE div 2 to SIZE div(top top)2 - 1 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.c"

int get_Int(int *a, int nat) {
    int n;
    do {
        n = scanf("%d", a);
        if (n == 0) {
            printf("Incorrect input. Try again!\n");
            scanf("%*f^\n");
            scanf("%*s");
            continue;
        }

        if (*a > 100000000) {
            printf("Input number <= 100M\n");
            n = 0;
        }

        if(nat == 1 && *a < 0) {
        	printf("Input natural number\n");
            n = 0;
        }

    } while(n == 0);

    scanf("%*1[\n]");
    return n < 0 ? 0 : 1;
}

int get_nInt(int *a) {
    return get_Int(a, 1);
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

const char *msgs[] = {"Menu:\n0. Quit", "1. Add element to tree",
                        "2. Find element", "3. Delete element", 
                        "4. Show tree", "5. Save in file",
                    	"6. Show items by X-key", "7. Show elements from range"};

int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int D_Create(),
    D_Add(Quadrant *, int, int),
    D_Find(),
    D_Delete(Quadrant *, int, int),
    D_Show();

int (*fptr[])() = { NULL, D_Add, D_Find, D_Delete, D_Show };

int D_Add(Quadrant *root, int size, int capacity) {
    int n, x, y;
    char *info = NULL;
    printf("Keys are integers (%d ÷ %d)\nInput X-key:\n", -size/2, - size / 2 + size - 1);
    get_Int(&x, 0);
    if(x < - size / 2 || x > - size / 2 + size - 1)
    {
    	printf("Error! Input key in correct interval!\n");
    	return 1;
    }
    printf("Input Y-key:\n");
    get_Int(&y, 0);
    if(y < - size / 2 || y > - size / 2 + size - 1)
    {
    	printf("Error! Input key in correct interval!\n");
    	return 1;
    }
    printf("Input information:\n");
    info = getStr();

    if (add_el(root, x, y, capacity, info) == 1) {
    	printf("Element has been added!\n");
    }

}

int D_Find() {
    int n, x, y;
    printf("Keys are integers (%d ÷ %d)\nInput X-key of searching element:\n", -size/2, - size / 2 + size - 1);
    get_Int(&x, 0);
    if(x < - size / 2 || x > - size / 2 + size - 1)
    {
    	printf("Error! Input key in correct interval!\n");
    	return 1;
    }
    printf("Input Y-key:\n");
    get_Int(&y, 0);
    if(y < - size / 2 || y > - size / 2 + size - 1)
    {
    	printf("Error! Input key in correct interval!\n");
    	return 1;
    }

    
}

int D_Delete(Quadrant *root, int size, int capacity) {
	int n, x, y;
    printf("Keys are integers (%d ÷ %d)\nInput X-key of deleted element:\n", -size/2, - size / 2 + size - 1);
    get_Int(&x, 0);
    if(x < - size / 2 || x > - size / 2 + size - 1)
    {
    	printf("Error! Input key in correct interval!\n");
    	return 1;
    }
    printf("Input Y-key of deleted element:\n");
    get_Int(&y, 0);
    if(y < - size / 2 || y > - size / 2 + size - 1)
    {
    	printf("Error! Input key in correct interval!\n");
    	return 1;
    }

    delete_el(root, size, capacity, x, y);

}

int D_Show(Quadrant *root, int size, int capacity, int deep) {
	for (int t = 0; t < deep; t++)
		printf("\t");
	printf("[Addr: %p; Busy: %d] ", root, root->busy);
	
	for (int i = 0; i < capacity; i++) {
		if(root->point[i] == NULL)
			printf("(%d NULL) ", i);
		else 
			printf("(%d [%d; %d] : %s) ", i, root->point[i]->x, root->point[i]->y, root->point[i]->info);
	}
	printf("\n");

	if (root->child[0] != NULL) {
		D_Show(root->child[0], size, capacity, deep+1);
		D_Show(root->child[1], size, capacity, deep+1);
		D_Show(root->child[2], size, capacity, deep+1);
		D_Show(root->child[3], size, capacity, deep+1);
	}
	else {
		return 0;
	}
}

int writeQuad(Quadrant *current, int capacity, FILE *fp) {
	int str_size;
	for(int i = 0; i < capacity; i++) {\
		if (current->point[i] != NULL) {
			fwrite(&(current->point[i]->x), sizeof(int), 1, fp);
			fwrite(&(current->point[i]->y), sizeof(int), 1, fp);
			str_size = strlen(current->point[i]->info) + 1;
			fwrite(&str_size, sizeof(int), 1, fp);
			fwrite(current->point[i]->info, sizeof(char), str_size, fp);
		}
	}
	if(current->child[0] == NULL) 
		return 0;
	else {
		writeQuad(current->child[0], capacity, fp);
		writeQuad(current->child[1], capacity, fp);
		writeQuad(current->child[2], capacity, fp);
		writeQuad(current->child[3], capacity, fp);
	}
}

int D_Save(Quadrant *root, int size, int capacity) {
	char *fileName;
	printf("Input name of file:\n");
	fileName = getStr();
	FILE *fp = fopen(fileName, "w+b");
	if (fp == NULL) {
		printf("File error\n");
		return 1;
	}
	
	fwrite(&size, sizeof(int), 1, fp);
	fwrite(&capacity, sizeof(int), 1, fp);
	writeQuad(root, capacity, fp);
	fclose(fp);
	free(fileName);
	return 0;
}

int D_Load(Quadrant **root, int *size, int *capacity) {
	char *fileName;
	printf("Enter file name:\n");
	fileName = getStr();
	FILE *fp = fopen(fileName, "r+b");
	if(fp == NULL) {
		printf("File error\n");
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	long int f_size = ftell(fp);
	printf("FSIZE: %ld\n", f_size);
	fseek(fp, 0, SEEK_SET);

	fread(size, sizeof(int), 1, fp);
	fread(capacity, sizeof(int), 1, fp);

	*root = create(*size, *capacity);

	int x, y, len;
	char *info;
	while (ftell(fp) != f_size) {
		fread(&x, sizeof(int), 1, fp);
		fread(&y, sizeof(int), 1, fp);
		fread(&len, sizeof(int), 1, fp);
		info = (char *)calloc(len, sizeof(char));
		fread(info, sizeof(char), len, fp);
	
		printf("Read: %d, %d, %s\n", x, y, info);

		if(x < (*root)->xMin || x > (*root)->xMax || y < (*root)->yMin || y > (*root)->yMax) {
			printf("File has been damaged!\n");
			free(info);
			return 1;
		}

		int ae = add_el((*root), x, y, *capacity, info);
		printf("AE: %d\n", ae);
	}

	printf("x0 %d\n", (*root)->point[0]->x);
	free(fileName);
	fclose(fp);
	return 0;

}

int showBTreeStep(bItem *bRoot) {
	if(bRoot == NULL)
		return 0;
	showBTreeStep(bRoot->left);
	printf("x: %d; y: %d, info: %s\n", bRoot->elem->x, bRoot->elem->y, bRoot->elem->info);
	showBTreeStep(bRoot->right);
	return 0;
}

int D_Show_Elems(Quadrant *root, int size, int capacity) {
	bItem **bRoot = (bItem **)calloc(1, sizeof(bItem *));
	stepToShowTree(root, bRoot, capacity);
	showBTreeStep(*bRoot);
	delBTree(*bRoot);
	free(bRoot);
	return 0;
}

int showRect(Quadrant *square, int size, int capacity, int xMin, int xMax, int yMin, int yMax) {
	for(int i = 0; i < capacity; i++) {
		if(square->point[i] != NULL && square->point[i]->x >= xMin && square->point[i]->x <= xMax && square->point[i]->y >= yMin && square->point[i]->y <= yMax) {
			printf("x: %d; y: %d; info: %s\n", square->point[i]->x, square->point[i]->y, square->point[i]->info);
		}
	}

	if(square->child[0] == NULL)
		return 0;
	else {
		for (int k = 0; k < 4; k++)
			if (square->child[k]->xMin <= xMax && square->child[k]->xMax >= xMin && square->child[k]->yMin <= yMax && square->child[k]->yMax >= yMin)
				showRect(square->child[k], size, capacity, xMin, xMax, yMin, yMax);	
	}
}

int D_ShowRange(Quadrant *root, int size, int capacity) {
	int n, xMin, xMax, yMin, yMax;
    char *info = NULL;
    printf("Keys are integers (%d ÷ %d)\nInput minimum X-key:\n", -size/2, - size / 2 + size - 1);
    get_Int(&xMin, 0);
    if(xMin < - size / 2 || xMin > - size / 2 + size - 1)
    {
    	printf("Error! Input keys in correct interval!\n");
    	return 1;
    }
    printf("Input maximum X-key:\n");
    get_Int(&xMax, 0);
    if(xMax < xMin || xMax < - size / 2 || xMax > - size / 2 + size - 1)
    {
    	printf("Error! Input keys in correct interval!\n");
    	return 1;
    }
    printf("Input minimum Y-key:\n");
    get_Int(&yMin, 0);
    if(yMin < - size / 2 || yMin > - size / 2 + size - 1)
    {
    	printf("Error! Input keys in correct interval!\n");
    	return 1;
    }
    printf("Input maximum Y-key:\n");
    get_Int(&yMax, 0);
    if(yMax < yMin || yMax < - size / 2 || yMax > - size / 2 + size - 1)
    {
    	printf("Error! Input keys in correct interval!\n");
    	return 1;
    }

    showRect(root, size, capacity, xMin, xMax, yMin, yMax);
}

int D_DelTree(Quadrant *root, int size, int capacity) {
	delTree(root, size, capacity);
}

int dialog(const char *msgs[], int N) {
    char *errmsg = "";
    int rc;
    int i, n;

    do {
        puts(errmsg);
        errmsg = "You are wrong. Repeat please!\n";

        for (i = 0; i < N; i++) {
            puts(msgs[i]);
        }
        puts("Choose the operation:");

        n = get_nInt(&rc);

        if (n == 0) {
            rc = 0;
        }

    } while (rc < 0 || rc >= N);

    return rc;
}

int main () {
    int rc, n, size, capacity;
    Quadrant **root = (Quadrant **)calloc(1, sizeof(Quadrant *));

    while (1) {
        printf("This is QUADtree program.\n\
Insert 1 to create new tree or insert 0 to load free from file:\n");
        
        n = get_nInt(&rc);
        if (rc == 1) {
            printf("Input SIZE of QUADRANT side.\n\
Keys are from -SIZE div 2 to SIZE div(top top)2 - 1\n");
            get_nInt(&size);
            printf("Input capacity of one quadrant:\n");
            get_nInt(&capacity);

            if (size == 0 || capacity == 0) 
                continue;

            *root = create(size, capacity);
            printf("New tree was created\n");
            printf("Addr :%p\n", *root);
            break;

        }
        else {
        	if (D_Load(root, &size, &capacity) == 0)
            	break;
        }
    }

    while (rc = dialog(msgs, NMsgs)) {
        if (rc == 1) {
            D_Add(*root, size, capacity);
        }
        else if(rc == 3) {
        	D_Delete(*root, size, capacity);
        }
        else if(rc == 4) {
        	D_Show(*root, size, capacity, 0);
        }
        else if(rc == 5) {
        	D_Save(*root, size, capacity);
        }
        else if(rc == 6) {
        	D_Show_Elems(*root, size, capacity);
        }
        else if (rc == 7) {
        	D_ShowRange(*root, size, capacity);
        }
    }

    D_DelTree(*root, size, capacity);
    free(root);
}
