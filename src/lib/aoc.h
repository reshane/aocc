#ifndef AOC_H
#define AOC_H

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

#ifndef NO_STD_LIB
#include <stdlib.h>
#endif

#ifndef NO_STD_IO
#include <stdio.h>
#endif

#ifndef NO_ASSERT
#include <assert.h>
#endif

#ifndef NO_STD_INT
#include <stdint.h>
#endif

#ifndef NO_STRING
#include <string.h>
#endif

#ifndef BUF_CAP
#define BUF_CAP 1<<16
#endif

int aoc_slurp_file(const char* file_name, char* input, size_t* input_sz);
void aoc_fill_input(const char* input_str, char* input, size_t* input_sz);

// standard helpers
size_t max(size_t a, size_t b);

// string view
typedef struct string_view {
    char *buf;
    size_t len;
} StringView;

void sv_trim_whitespace(StringView *sv);
void sv_trim_left_whitespace(StringView *sv);
void sv_trim_right_whitespace(StringView *sv);
int sv_atoi(StringView *sv);
long long sv_atoll(StringView *sv);
void sv_print(StringView *sv);
int sv_cmp(StringView *s1, StringView *s2);

// splitter
typedef struct splitter {
    char *buf;
    size_t st;
    size_t sz;
    size_t mx;
} Splitter;

int lines(Splitter* split);
int delim(Splitter* split, char c);
void split_curr(Splitter *split, StringView *res);

// point
typedef struct point {
    long long x;
    long long y;
} Point;

// map
typedef struct Key {
    void* data;
    size_t size;
} Key;

typedef struct Value {
    void* data;
    size_t size;
} Value;

typedef struct Entry {
    Key* key;
    void* value;
} Entry;

typedef struct Map {
    Entry* entries;
    size_t size;
    size_t cap;
    int (*cmp_keys)(Key*, Key*);
} Map;

Map* Map_new(int (*k_cmp)(Key*, Key*));
void Map_insert(Map* map, Key* k, void* v);
void* Map_get(Map* map, Key* q_key);
int Map_contains(Map* map, Key* q_key);
size_t Map_size(Map* map);
void Map_free(Map* map);

// int vector

typedef struct {
    size_t size;
    size_t capacity;
    int* data;
} Vector_i;

Vector_i* Vector_i_new();
size_t Vector_i_size(Vector_i* v);
void Vector_i_push(Vector_i* v, int e);
int Vector_i_get(Vector_i* v, size_t idx);
void Vector_i_free(Vector_i* v);

// float vector

typedef struct {
    size_t size;
    size_t capacity;
    float* data;
} Vector_f;

Vector_f* Vector_f_new();
size_t Vector_f_size(Vector_f* v);
void Vector_f_push(Vector_f* v, float e);
float Vector_f_get(Vector_f* v, size_t idx);
void Vector_f_free(Vector_f* v);

// int matrix
typedef struct {
    size_t rows;
    size_t cols;
    int* data;
} Matrix_i;

Matrix_i* Matrix_i_new(size_t rows, size_t cols);
size_t Matrix_i_rows(Matrix_i* m);
size_t Matrix_i_cols(Matrix_i* m);
void Matrix_i_set(Matrix_i* m, size_t x, size_t y, int e);
int Matrix_i_get(Matrix_i* m, size_t x, size_t y);
void Matrix_i_free(Matrix_i* m);

// uint32_t matrix
typedef struct {
    size_t rows;
    size_t cols;
    uint32_t* data;
} Matrix_u32;

Matrix_u32* Matrix_u32_new(size_t rows, size_t cols);
size_t Matrix_u32_rows(Matrix_u32* m);
size_t Matrix_u32_cols(Matrix_u32* m);
void Matrix_u32_set(Matrix_u32* m, size_t x, size_t y, uint32_t e);
uint32_t Matrix_u32_get(Matrix_u32* m, size_t x, size_t y);
void Matrix_u32_free(Matrix_u32* m);

// float matrix
typedef struct {
    size_t rows;
    size_t cols;
    float* data;
} Matrix_f;

Matrix_f* Matrix_f_new(size_t rows, size_t cols);
size_t Matrix_f_rows(Matrix_f* m);
size_t Matrix_f_cols(Matrix_f* m);
void Matrix_f_set(Matrix_f* m, size_t x, size_t y, float e);
float Matrix_f_get(Matrix_f* m, size_t x, size_t y);
void Matrix_f_free(Matrix_f* m);

#endif// AOC_H
