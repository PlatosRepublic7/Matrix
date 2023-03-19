#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Element {
    int value;
    int row;
    int column;
    struct Element *rowElement;
    struct Element *columnElement;
};

struct Header {
    int index;
    struct Header *header;
    struct Element *element;
};

struct Matrix {
    struct Header *headRowHeader;
    struct Header *headColumnHeader;
    int rows;
    int columns;
    int fillValue;
};


struct Matrix MTRX_New();
struct Matrix MTRX_alloc(char *filename, int fillValue);
void MTRX_free(struct Matrix* m);
int MTRX_getElement(struct Matrix *m, int row, int column);
void MTRX_write(struct Matrix *m, char *filename);
struct Matrix MTRX_add(struct Matrix *a, struct Matrix *b, int fillValue);
struct Matrix MTRX_multiply(struct Matrix *a, struct Matrix *b, int fillValue);
struct Matrix MTRX_transpose(struct Matrix *m);

int main() {
    struct Matrix a, t;
    a = MTRX_alloc("matrixA.txt", 0);
    t = MTRX_transpose(&a);
    MTRX_write(&t, "matrixT.txt");
    MTRX_free(&t);
    MTRX_free(&a);
    return 0;
}

struct Matrix MTRX_New(){
    struct Matrix m;
    m.headRowHeader = NULL;
    m.headColumnHeader = NULL;
    m.rows = 0;
    m.columns = 0;
    m.fillValue = 0;

    return m;
}

struct Matrix MTRX_alloc(char *filename, int fillValue){
    struct Matrix mtrx = MTRX_New();
    struct Header *rowHeader = mtrx.headRowHeader;
    struct Header *colHeader = mtrx.headColumnHeader;

    int row = 0, col = 0, isFill = 0;
    int val = 0;
    char c;
    FILE *mFile = fopen(*filename, "r");

    mtrx.fillValue = fillValue;

    while (1){
        c = fgetc(mFile);
        if (c == EOF)
            break;
        
        if (c == '\n'){
            row++;
            rowHeader->index = row;
            continue;
        }
        
        if (isspace(c)){
            col++;
            colHeader->index = col;
            continue;
        }
        else{
            if (c == '-'){
                isFill = 1;
                c = fgetc(mFile);
                val += (-1) * (int)(c - '0');
                continue;
            }
            else if (!isFill){
                val += (int)(c - '0');
            }
            else
                continue;
        }

    }
    fclose(mFile);

    return mtrx;
}