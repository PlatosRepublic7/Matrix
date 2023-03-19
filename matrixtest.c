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


int get_power(int base, int exp);
int compute_value(char numString[10], int sLen, int isNeg);
struct Matrix new_matrix(int fillValue);
void Push_Element(struct Header *h, char rowCol, int row, int col, int val);
void Push_Header(struct Matrix *m, char rowCol, int index);
int E_List_Length(struct Header *h, char rowCol);
int E_List_Contains_Col(struct Header *h, int col);
int E_List_empty(struct Header *h);
int H_List_empty(struct Matrix *m, char rowCol);
int H_List_Length(struct Matrix *m, char rowCol);
void Make_Columns(struct Matrix *m);
int get_mult_result(struct Matrix *a, struct Matrix *b, int arow, int bcol);


struct Matrix MTRX_alloc(char *filename, int fillValue);
void MTRX_free(struct Matrix* m);
int MTRX_getElement(struct Matrix *m, int row, int column);
void MTRX_write(struct Matrix *m, char *filename);
struct Matrix MTRX_add(struct Matrix *a, struct Matrix *b, int fillValue);
struct Matrix MTRX_multiply(struct Matrix *a, struct Matrix *b, int fillValue);
struct Matrix MTRX_transpose(struct Matrix *m);

int main() {
    struct Matrix a, b, c, d, m, t;
    char *f = "matwrite.txt", *g = "matwrite2.txt", *h = "matrixTrans.txt";
    a = MTRX_alloc("matrixA.txt", 0);
    b = MTRX_alloc("matrixAdd.txt", 0);
    m = MTRX_alloc("matrixMult.txt", 0);
    t = MTRX_transpose(&a);
    c = MTRX_add(&a, &b, 0);
    d = MTRX_multiply(&a, &m, 0);
    MTRX_write(&c, f);
    MTRX_write(&d, g);
    MTRX_write(&t, h);
    MTRX_free(&a);
    MTRX_free(&b);
    MTRX_free(&m);
    MTRX_free(&t);
    return 0;
}

struct Matrix MTRX_transpose(struct Matrix *m) {
    struct Matrix out = new_matrix(m->fillValue);
    int result, i, j, emptyrow = 1;

    out.rows = m->columns;
    out.columns = m->rows;

    for (i = 1; i <= out.rows; i++) {
        for (j = 1; j <= out.columns; j++) {
            result = MTRX_getElement(m, j, i);
            if (result != m->fillValue) {
                if (emptyrow) {
                    Push_Header(&out, 'r', i);
                    emptyrow = 0;
                }
                Push_Element(out.headRowHeader, 'r', i, j, result);
            }
        }
        emptyrow = 1;
    }
    Make_Columns(&out);

    return out;
}

struct Matrix MTRX_multiply(struct Matrix *a, struct Matrix *b, int fillValue) {
    struct Matrix out = new_matrix(fillValue);
    int result, i, j, emptyrow = 1;

    out.rows = a->rows;
    out.columns = b->columns;

    for (i = 1; i <= out.rows; i++) {
        for (j = 1; j <= out.columns; j++) {
            result = get_mult_result(a, b, i, j);
            if (result != fillValue) {
                if (emptyrow) {
                    Push_Header(&out, 'r', i);
                    emptyrow = 0;
                }
                Push_Element(out.headRowHeader, 'r', i, j, result);
            }
        }
        emptyrow = 1;
    }
    Make_Columns(&out);

    return out;
}

int get_mult_result(struct Matrix *a, struct Matrix *b, int arow, int bcol) {
    int result = 0, i;

    for (i = 1; i <= a->columns; i++) {
        result += MTRX_getElement(a, arow, i) * MTRX_getElement(b, i, bcol);
    }

    return result;
}

struct Matrix MTRX_add(struct Matrix *a, struct Matrix *b, int fillValue) {
    struct Matrix out = new_matrix(fillValue);
    int result, i, j, emptyrow = 1;
    
    out.columns = a->columns;
    out.rows = a->rows;

    for (i = 1; i <= out.rows; i++) {
        for (j = 1; j <= out.columns; j++) {
            result = MTRX_getElement(a, i, j) + MTRX_getElement(b, i, j);
            if (result != fillValue) {
                if (emptyrow) {
                    Push_Header(&out, 'r', i);
                    emptyrow = 0;
                }
                Push_Element(out.headRowHeader, 'r', i, j, result);
            }
        }
        emptyrow = 1;
    }
    Make_Columns(&out);

    return out;
}

int E_List_empty(struct Header *h) {
    return h->element == NULL;
}

int E_List_Contains_Col(struct Header *h, int col) {
    struct Element *e;
    int hasItem = 0;
    for (e = h->element; e != NULL; e = e->rowElement) {
        if (e->column == col) {
            hasItem = 1;
            break;
        }
    }

    return hasItem;
}

void Push_Header(struct Matrix *m, char rowCol, int index) {
    struct Header *header = malloc(sizeof(struct Header));
    header->index = index;
    header->element = NULL;
    if (H_List_empty(m, rowCol)) {
        if (rowCol == 'r') {
            m->headRowHeader = header;
            m->headRowHeader->header = NULL;
        }
        else {
            m->headColumnHeader = header;
            m->headColumnHeader->header = NULL;
        }
    }
    else {
        if (rowCol == 'r') {
            header->header = m->headRowHeader;
            m->headRowHeader = header;
        }
        else {
            header->header = m->headColumnHeader;
            m->headColumnHeader = header;
        }
    }
}

void Push_Element(struct Header *h, char rowCol, int row, int col, int val) {
    struct Element *elem = malloc(sizeof(struct Element));
    elem->row = row;
    elem->column = col;
    elem->value = val;
    elem->rowElement = NULL;
    elem->columnElement = NULL;
    if (E_List_empty(h)) {
        if (rowCol == 'r')
            h->element = elem;
        else
            h->element = elem;
    }
    else {
        if (rowCol == 'r') {
            elem->rowElement = h->element;
            h->element = elem;
        }
        else {
            elem->columnElement = h->element;
            h->element = elem;
        }
    }
}

int E_List_Length(struct Header *h, char rowCol) {
    struct Element *e;
    int n = 0;
    if (rowCol == 'r') {
        for (e = h->element; e != NULL; e = e->rowElement) {
            ++n;
        }
    }
    else if (rowCol == 'c') {
        for (e = h->element; e != NULL; e = e->columnElement) {
            ++n;
        }
    }

    return n;
}

int H_List_Length(struct Matrix *m, char rowCol) {
    struct Header *h;
    int n = 0;
    if (rowCol == 'r') {
        for (h = m->headRowHeader; h != NULL; h = h->header) {
            ++n;
        }
    }
    else {
        for (h = m->headColumnHeader; h != NULL; h = h->header) {
            ++n;
        }
    }

    return n;
}

int H_List_empty(struct Matrix *m, char rowCol) {
    if (rowCol == 'r')
        return m->headRowHeader == NULL;
    else
        return m->headColumnHeader == NULL;
}

void Make_Columns(struct Matrix *m) {
    struct Header *h;
    struct Element *e;
    struct Element *temp;
    int i, emptycol = 1;

    for (i = m->columns; i > 0; i--) {
        for (h = m->headRowHeader; h != NULL; h = h->header) {
            if (E_List_Contains_Col(h, i)) {
                temp = h->element;
                while (h->element->column != i) {
                    h->element = h->element->rowElement;
                }
                if (emptycol) {
                    Push_Header(m, 'c', i);
                    m->headColumnHeader->element = h->element;
                    e = h->element;
                    emptycol = 0;
                }
                else {
                    e->columnElement = h->element;
                    e = h->element;
                }
                h->element = temp;
            }
        }
        emptycol = 1;
    }
}

struct Matrix new_matrix(int fillValue) {
    struct Matrix m;
    m.headRowHeader = NULL;
    m.headColumnHeader = NULL;
    m.rows = 0;
    m.columns = 0;
    m.fillValue = fillValue;

    return m;
}

int get_power(int base, int exp) {
    int total = 1, i;
    for (i = 0; i < exp; i++) {
        total *= base;
    }
    return total;
}

int compute_value(char numString[10], int sLen, int isNeg) {
    int total = 0, i;
    int exp = sLen - 1;
    for (i = 0; i < sLen; i++) {
        total += (int)(numString[i] - '0') * get_power(10, exp);
        exp--;
    }

    if (isNeg)
        total *= -1;

    return total;
}

struct Matrix MTRX_alloc(char *filename, int fillValue) {
    struct Matrix mat = new_matrix(fillValue);
    char c;
    char num[10] = "..........";
    int val = 0, row = 1, col = 0, emptyrow = 1, isneg = 0, numlen = 0, i;
    FILE *infile = fopen(filename, "r");

    /* This loop parses through the file and extracts all relevant matrix information
        while also allocating the necessary structs and initializing them correctly */
    while (1) {
        c = fgetc(infile);
        if (c == EOF)
            break;
        
        if (c == (char)(fillValue + '0')) {
            col++;
        }
        else if (isdigit(c) || c == '-') {
            if (c == '-') {
                isneg = 1;
                c = fgetc(infile);
            }

            while (!isspace(c)) {
                num[numlen] = c;
                numlen++;
                c = fgetc(infile);
                if (c == '\n') {
                    ungetc(c, infile);
                    break;
                }
            }

            /* Compute value, row, column while parsing. Then create new element 
             when required. Row header nodes first, then column header nodes through
             a second pass. */
            val = compute_value(num, numlen, isneg);
            col++;
            
            if (val != fillValue) {
                if (emptyrow) {
                    Push_Header(&mat, 'r', row);
                    emptyrow = 0;
                }
                Push_Element(mat.headRowHeader, 'r', row, col, val);
            }

            for (i = 0; i < 10; i++) {
                if (num[i] == '.')
                    break;
                else
                    num[i] = '.';
            }
            isneg = 0;
            numlen = 0;
            val = 0;
        }
        else if (c == '\n') {
            row++;
            mat.columns = col;
            col = 0;
            emptyrow = 1;
        }
        else
            continue;
    }
    mat.rows = row - 1;
    fclose(infile);

    /* Iterate through row lists and create column headers and appropriate pointers
        to elements */
    Make_Columns(&mat);

    return mat;
}

void MTRX_free(struct Matrix* m) {
    struct Header *h;
    struct Element *e;


    for (h = m->headRowHeader; h != NULL; h = h->header) {
        for (e = h->element; e != NULL; e = e->rowElement) {
            if (e->rowElement == NULL) {
                free(e);
                break;
            }
            free(e);
        }
    }

    while (!H_List_empty(m, 'r')) {
        h = m->headRowHeader;
        m->headRowHeader = m->headRowHeader->header;
        free(h);
    }

    while (!H_List_empty(m, 'c')) {
        h = m->headColumnHeader;
        m->headColumnHeader = m->headColumnHeader->header;
        free(h);
    }
}

int MTRX_getElement(struct Matrix *m, int row, int column) {
    struct Header *rh;
    struct Element *e;

    for (rh = m->headRowHeader; rh != NULL; rh = rh->header) {
        if (rh->index != row)
            continue;
        for (e = rh->element; e != NULL; e = e->rowElement) {
            if (e->column == column)
                return e->value;
            else if (e->column < column)
                break;
        }
        break;
    }

    return m->fillValue;
}

void MTRX_write(struct Matrix *m, char *filename) {
    int rows = m->rows, col = m->columns, i, j, val;
    FILE *outfile = fopen(filename, "w");
    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= col; j++) {
            val = MTRX_getElement(m, i, j);
            if (val == m->fillValue) {
                if (j == col)
                    fprintf(outfile, "%d\n", m->fillValue);
                else
                    fprintf(outfile, "%d ", m->fillValue);
            }
            else {
                if (j == col)
                    fprintf(outfile, "%d\n", val);
                else
                    fprintf(outfile, "%d ", val);
            }
        }
    }
    fclose(outfile);
}
