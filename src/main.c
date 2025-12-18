#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "lib/aoc.h"

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

Map* Map_new(int (*k_cmp)(Key*, Key*))
{
    Map* new = (Map*) malloc(sizeof(Map));
    if (!new) {
        printf("ERROR! could not initialize map!");
        exit(1);
    }
    new->size = 0;
    new->cap = 0;
    new->entries = NULL;
    new->cmp_keys = k_cmp;
    return new;
}

void Map_insert(Map* map, Key* k, void* v)
{
    if (map->cap == 0) {
        map->cap = 1;
        map->entries = (Entry*) malloc(map->cap * sizeof(Entry));
        if (!map->entries) {
            exit(1);
        }
        Entry entry = { k, v };
        map->entries[map->size++] = entry;
        return;
    }
    for (size_t i = 0; i < map->size; ++i) {
        if (map->cmp_keys(map->entries[i].key, k) == 0) {
            map->entries[i].value = v;
            return;
        }
    }
    if (map->size == map->cap) {
        map->cap *= 2;
        Entry* newMem = (Entry*) realloc(map->entries, map->cap * sizeof(Entry));
        if (!newMem) {
            printf("ERROR: Could not increase the size of the map");
            exit(1);
        }
        map->entries = newMem;
    }
    Entry entry = { k, v };
    map->entries[map->size++] = entry;
}

void* Map_get(Map* map, Key* q_key)
{
    for (size_t i = 0; i < map->size; ++i) {
        // printf("[%s] Entry { [%s], %d } strcmp -> %d\n", q_key, map->entries[i].key, map->entries[i].value, strcmp(map->entries[i].key, q_key));
        if (map->cmp_keys((void*)map->entries[i].key, q_key) == 0) {
            return map->entries[i].value;
        }
    }
    return NULL;
}

int Map_contains(Map* map, Key* q_key)
{
    for (size_t i = 0; i < map->size; ++i) {
        // printf("[%s] Entry { [%s], %d } strcmp -> %d\n", q_key, map->entries[i].key, map->entries[i].value, strcmp(map->entries[i].key, q_key));
        if (map->cmp_keys((void*)map->entries[i].key, q_key) == 0) {
            return 1;
        }
    }
    return 0;
}

size_t Map_size(Map* map)
{
    return map->size;
}

void Map_free(Map* map)
{
    free(map->entries);
    free(map);
}

size_t max(size_t a, size_t b)
{
    if (a < b) {
        return b;
    }
    return a;
}

int keys_cmp(Key* k1, Key* k2)
{
    return strncmp(k1->data, k2->data, max(k1->size, k2->size));
}

#define BUF_CAP 4096*8
char input[BUF_CAP];
size_t input_sz;

void slurp_file(const char* file_name)
{
    // int open(const char *path, int oflag, ..); 
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Could not get file descriptor from %s", file_name);
        close(fd);
        return;
    }
    // int fstat(int fildes, struct stat *buf);
    struct stat statbuf;
    if (fstat(fd,  &statbuf) == -1) {
        fprintf(stderr, "Could not stat the file: %s", file_name);
        close(fd);
        return;
    }

    // ssize_t read(int filedes, void *buf, size_t nbyte);
    off_t num_bytes = statbuf.st_size;
    if (num_bytes > BUF_CAP) {
        fprintf(stderr, "Could not fit input (%lld bytes) into input buffer (%d bytes)", num_bytes, BUF_CAP);
        close(fd);
        return;
    }
    read(fd, input, num_bytes);
    input_sz = (size_t)num_bytes;

    // close the file
    close(fd);
}

void fill_input()
{
    /*
    char *input_str = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,\n"
                "1698522-1698528,446443-446449,38593856-38593862,565653-565659,\n"
                "824824821-824824827,2121212118-2121212124";
                */
    char *input_str = "L68\n"
        "L30\n"
        "R48\n"
        "L51\n"
        "R60\n"
        "L55\n"
        "L11\n"
        "L99\n"
        "R14\n"
        "L82\n";
    size_t i = 0;
    // printf("%d\n", sizeof(char)*strlen(input_str));
    // printf("%d\n", strlen(input_str));
    size_t input_len = strlen(input_str);
    snprintf(input, input_len, "%s", input_str);
    input_sz = input_len;
}

typedef struct String_View {
    char *buf;
    size_t len;
} StringView;

typedef struct splitter {
    char *buf;
    size_t st;
    size_t sz;
    size_t mx;
} Splitter;

int lines(Splitter* split)
{
    split->st += split->sz;
    if (split->buf[split->st] == '\n') split->st++;
    split->sz = 1;
    while (split->sz < split->mx) {
        split->sz++;
        if (split->buf[split->st+split->sz] == '\n') {
            break;
        }
    }
    // printf("[%c]\n", split->buf[split->sz]);
    // printf("%zu, %zu\n", split->st, split->sz);
    return split->st < split->mx;
}

void curr(Splitter *split, StringView *res)
{
    res->buf = &split->buf[split->st];
    res->len = split->sz;
}

void printsv(StringView *sv)
{
    printf("%.*s", (int)sv->len, sv->buf);
}

int main()
{
    /*
    Map* my_map = Map_new(&keys_cmp);
    // printf("%zu\n", Map_size(my_map));
    int v = 0;
    Key k1 = { .data = "shane", .size = sizeof(char)*strlen("shane") };
    int v2 = 0;
    Key k2 = { .data = "notshane", .size = sizeof(char)*strlen("notshane") };
    printf("k = { %s, %zu }\n", (char*)k1.data, k1.size);
    printf("k = { %s, %zu }\n", (char*)k2.data, k2.size);
    Map_insert(my_map, &k1, &v);
    Map_insert(my_map, &k2, &v2);
    printf("%s: %d\n", (char*)k1.data, *(int*)Map_get(my_map, &k1));
    printf("%s: %d\n", (char*)k2.data, *(int*)Map_get(my_map, &k2));
    printf("Map_contains(\"notshane\") = %d\n", Map_contains(my_map, &k2));
    Map_free(my_map);

    printf("%s", input);
    printf("%zu\n", strlen(input));

    printf("%d\n", atoi("69"));
    */

    // fill_input();

    const char* in_file = "./inputs/01.txt";
    slurp_file(in_file);

    // printf("%s\n", input);

    // printf("%d\n", input_sz);
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};

    // char line[256];
    StringView sv = {0};
    while (lines(&split)) {
        // printf(line, "%.*s", (int)split.sz+1, split.buf + split.st);
        // printf("%zu, %zu\n", split.st, split.sz);
        curr(&split, &sv);
        printsv(&sv);
        printf("\n");
        // snprintf(line, split.sz+1, "%.*s", (int)split.sz, split.buf + split.st);
        // printf("%s\n", line);
    }

    return 0;
}


