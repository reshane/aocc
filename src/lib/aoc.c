#include "aoc.h"

// vector

Vector_i* Vector_i_new() {
    Vector_i* new = (Vector_i*) malloc(sizeof(Vector_i));
    if (!new) {
        printf("ERROR: Could initialize vector!");
        exit(1);
    }
    new->size = 0;
    new->capacity = 0;
    new->data = NULL;
    return new;
}

void Vector_i_push(Vector_i* v, int e) {
    if (v->capacity == 0) {
        v->capacity = 1;
        v->data = (int*) malloc(v->capacity * sizeof(int));
        if (!v->data) exit(1);
    }
    else if (v->size == v->capacity) {
        v->capacity *= 2;
        int* newMem = (int*) realloc(v->data, v->capacity * sizeof(int));
        if (!newMem) {
            // we have an error here... uh-oh!
            printf("ERROR: Could not increase the size of the vector");
            exit(1);
        }
        v->data = newMem;
    }
    v->data[v->size++] = e;
}

int Vector_i_get(Vector_i* v, size_t idx) {
    assert(idx < v->size);
    return v->data[idx];
}

size_t Vector_i_size(Vector_i* v) {
    return v->size;
}

size_t Vector_i_capacity(Vector_i* v) {
    return v->capacity;
}

void Vector_i_free(Vector_i* v) {
    free(v->data);
    free(v);
}

Vector_i* Vector_i_convolve(Vector_i* a, Vector_i* b) {
    Vector_i* result = Vector_i_new();
    // iterate b [Vector_i_size(b)-1..0]
    // iterate a [0..Vector_i_size(a)-1]
    int a_size = Vector_i_size(a);
    int b_size = Vector_i_size(b);

    // a[0] * b[0]
    // a[0] * b[1] + a[1] * b[0]
    // a[0] * b[2] + a[1] * b[1] + a[2] * b[0]
    // ...
    // a[1] * b[9] + a[2] * b[8] * a[3] * b[7] ... 
    // a[2] * b[9] + a[3] * b[8] * a[4] * b[7] ... 
    // one variable going from 0 -> a.size i
    // a indexed by a variable counting from 0->i
    // b indexed by a variable counting from i->0
    int a_start = 0, b_start = 0;
    for (int i=0; i<a_size+b_size; ++i) {
        int j=a_start,k=b_start,total=0;
        while (j<a_size && k>-1) {
            total += Vector_i_get(a, j) * Vector_i_get(b, k);
            j++;
            k--;
        }
        Vector_i_push(result, total);
        if (b_start == b_size-1) {
            a_start++;
        } else {
            b_start++;
        }
    }

    return result;
}

Vector_f* Vector_f_new() {
    Vector_f* new = (Vector_f*) malloc(sizeof(Vector_f));
    if (!new) {
        printf("ERROR: Could initialize vector!");
        exit(1);
    }
    new->size = 0;
    new->capacity = 0;
    new->data = NULL;
    return new;
}

void Vector_f_push(Vector_f* v, float e) {
    if (v->capacity == 0) {
        v->capacity = 1;
        v->data = (float*) malloc(v->capacity * sizeof(float));
        if (!v->data) exit(1);
    }
    else if (v->size == v->capacity) {
        v->capacity *= 2;
        float* newMem = (float*) realloc(v->data, v->capacity * sizeof(float));
        if (!newMem) {
            // we have an error here... uh-oh!
            printf("ERROR: Could not increase the size of the vector");
            exit(1);
        }
        v->data = newMem;
    }
    v->data[v->size++] = e;
}

float Vector_f_get(Vector_f* v, size_t idx) {
    assert(idx < v->size);
    return v->data[idx];
}

size_t Vector_f_size(Vector_f* v) {
    return v->size;
}

size_t Vector_f_capacity(Vector_f* v) {
    return v->capacity;
}

void Vector_f_free(Vector_f* v) {
    free(v->data);
    free(v);
}

Vector_f* Vector_f_convolve(Vector_f* a, Vector_f* b) {
    Vector_f* result = Vector_f_new();
    // iterate b [Vector_f_size(b)-1..0]
    // iterate a [0..Vector_f_size(a)-1]
    int a_size = Vector_f_size(a);
    int b_size = Vector_f_size(b);

    // a[0] * b[0]
    // a[0] * b[1] + a[1] * b[0]
    // a[0] * b[2] + a[1] * b[1] + a[2] * b[0]
    // ...
    // a[1] * b[9] + a[2] * b[8] * a[3] * b[7]
    // a[2] * b[9] + a[3] * b[8] * a[4] * b[7]
    // one variable going from 0 -> a.size i
    // a indexed by a variable counting from 0->i
    // b indexed by a variable counting from i->0
    int a_start = 0, b_start = 0;
    for (int i=0; i<a_size+b_size; ++i) {
        int j=a_start,k=b_start;
        float total=0;
        while (j<a_size && k>-1) {
            total += Vector_f_get(a, j) * Vector_f_get(b, k);
            j++;
            k--;
        }
        Vector_f_push(result, total);
        if (b_start == b_size-1) {
            a_start++;
        } else {
            b_start++;
        }
    }

    return result;
}

// int matrix
Matrix_i* Matrix_i_new(size_t cols, size_t rows) {
    Matrix_i* new = (Matrix_i*) malloc(sizeof(Matrix_i));
    if (!new) {
        printf("ERROR: Could not initialize matrix");
        exit(1);
    }
    new->rows = rows;
    new->cols = cols;
    new->data = (int*) malloc(rows*cols*sizeof(int));
    if (!new->data) {
        printf("ERROR: Could not initialize matrix data");
        exit(1);
    }
    return new;
}

size_t Matrix_i_rows(Matrix_i* m) {
    return m->rows;
}

size_t Matrix_i_cols(Matrix_i* m) {
    return m->cols;
}

void Matrix_i_set(Matrix_i* m, size_t x, size_t y, int e) {
    assert(x < m->cols);
    assert(y < m->rows);
    size_t idx = (m->rows * y) + x;
    m->data[idx] = e;
}

int Matrix_i_get(Matrix_i* m, size_t x, size_t y) {
    assert(x < m->cols);
    assert(y < m->rows);
    size_t idx = (m->rows * y) + x;
    return m->data[idx];
}

void Matrix_i_free(Matrix_i* m) {
    free(m->data);
    free(m);
}

// uint32 matrix
Matrix_u32* Matrix_u32_new(size_t cols, size_t rows) {
    Matrix_u32* new = (Matrix_u32*) malloc(sizeof(Matrix_u32));
    if (!new) {
        printf("ERROR: Could not initialize matrix");
        exit(1);
    }
    new->rows = rows;
    new->cols = cols;
    new->data = (uint32_t*) malloc(rows*cols*sizeof(uint32_t));
    if (!new->data) {
        printf("ERROR: Could not initialize matrix data");
        exit(1);
    }
    return new;
}

size_t Matrix_u32_rows(Matrix_u32* m) {
    return m->rows;
}

size_t Matrix_u32_cols(Matrix_u32* m) {
    return m->cols;
}

void Matrix_u32_set(Matrix_u32* m, size_t x, size_t y, uint32_t e) {
    assert(x < m->cols);
    assert(y < m->rows);
    size_t idx = (m->rows * y) + x;
    m->data[idx] = e;
}

uint32_t Matrix_u32_get(Matrix_u32* m, size_t x, size_t y) {
    assert(x < m->cols);
    assert(y < m->rows);
    size_t idx = (m->rows * y) + x;
    return m->data[idx];
}

void Matrix_u32_free(Matrix_u32* m) {
    free(m->data);
    free(m);
}

// float matrix

Matrix_f* Matrix_f_new(size_t cols, size_t rows) {
    Matrix_f* new = (Matrix_f*) malloc(sizeof(Matrix_f));
    if (!new) {
        printf("ERROR: Could not initialize matrix");
        exit(1);
    }
    new->rows = rows;
    new->cols = cols;
    new->data = (float*) malloc(rows*cols*sizeof(float));
    if (!new->data) {
        printf("ERROR: Could not initialize matrix data");
        exit(1);
    }
    return new;
}

size_t Matrix_f_rows(Matrix_f* m) {
    return m->rows;
}

size_t Matrix_f_cols(Matrix_f* m) {
    return m->cols;
}

void Matrix_f_set(Matrix_f* m, size_t x, size_t y, float e) {
    assert(x < m->cols);
    assert(y < m->rows);
    size_t idx = (m->rows * y) + x;
    m->data[idx] = e;
}

float Matrix_f_get(Matrix_f* m, size_t x, size_t y) {
    assert(x < m->cols);
    assert(y < m->rows);
    size_t idx = (m->rows * y) + x;
    return m->data[idx];
}

void Matrix_f_free(Matrix_f* m) {
    free(m->data);
    free(m);
}

