
#include "lib/aoc.h"
#include <stdint.h>
#include <sys/cdefs.h>

#define D11_MAX_LNS 1024

uint32_t hash(StringView *node_str) {
    // uint32_t h = 0;
    // for (size_t i = 0; i < node_str->len; ++i) {
    //     h <<= 8;
    //     h |= node_str->buf[i];
    // }
    // uint32_t h = ((uint32_t)0xFFFFFF00 & *(uint32_t*)((void*)node_str->buf)) >> 8;
    // uint32_t h = (*(uint32_t*)(node_str->buf)) >> 8;
    // printf("[%p %p %p] -> %"PRIu32"\n",
    // uint32_t h = ((((uint32_t)(uint8_t)node_str->buf[0]) << 16
    //                | ((uint32_t)(uint8_t)node_str->buf[1]) << 8
    //                | ((uint32_t)(uint8_t)node_str->buf[2])) - 0x616161) % D11_MAX_LNS;
    // printf("["); sv_print(node_str); printf("] %"PRIu32"\n", h);

    uint32_t h = 5381;
    size_t i = 0;
    while (i < node_str->len) {
        h = ((h >> 5) + h) + node_str->buf[i];
        i++;
    }
    return h % D11_MAX_LNS;
}

uint32_t get_index(StringView *node_str, StringView *nodes, int *populated) {
    uint32_t node_hash = hash(node_str);
    if (populated[node_hash]) {
        while (sv_cmp(node_str, &nodes[node_hash]) != 0 && node_hash < D11_MAX_LNS) {
            // printf("Nodes collide! ["); sv_print(&node_str); printf("] & [");
            // sv_print(&node_strs[node_hash]); printf("] -> %"PRIu32"\n", node_hash);
            node_hash++;
        }
    }
    if (node_hash == D11_MAX_LNS) {
        // we can make this circular later
        fprintf(stderr, "ITS FULL!");
        exit(1);
    }
    if (!populated[node_hash]) {
        nodes[node_hash] = *node_str;
        populated[node_hash] = 1;
    }
    return node_hash;
}

typedef struct {
    StringView *nodes;
    int *populated;
} d11set;

uint32_t d11_set_insert(d11set *set, StringView *node_str) {
    uint32_t node_hash = hash(node_str);
    if (set->populated[node_hash]) {
        while (sv_cmp(node_str, &set->nodes[node_hash]) != 0 && node_hash < D11_MAX_LNS) {
            // printf("Nodes collide! ["); sv_print(&node_str); printf("] & [");
            // sv_print(&node_strs[node_hash]); printf("] -> %"PRIu32"\n", node_hash);
            node_hash++;
        }
    }
    if (node_hash == D11_MAX_LNS) {
        // we can make this circular later
        fprintf(stderr, "ITS FULL!");
        exit(1);
    }
    if (!set->populated[node_hash]) {
        set->nodes[node_hash] = *node_str;
        set->populated[node_hash] = 1;
    }
    return node_hash;
}

typedef struct {
    uint32_t data;
} d11qf;

typedef struct {
    d11qf *data;
    size_t qh, qb, size;
} d11queue;

#define D11_Q_CAP 4096

void qpush11(d11queue *q, d11qf e)
{
    if (q->size == D11_Q_CAP) {
        fprintf(stderr, "Could not push to queue, too many elements %zu > %d\n", q->size, D11_Q_CAP);
        exit(1);
    }
    q->data[q->qb++] = e;
    q->qb %= D11_Q_CAP;
    q->size += 1;
}

d11qf qpop11(d11queue *q)
{
    if (q->size == 0) {
        fprintf(stderr, "Could not pop from an empty queue\n");
        exit(1);
    }
    d11qf e = q->data[q->qh];
    q->qh += 1;
    q->qh %= D11_Q_CAP;
    q->size -= 1;
    return e;
}

long long d11_dist(uint32_t start, uint32_t end, uint32_t nbors[D11_MAX_LNS][D11_MAX_LNS], size_t *nbor_cts) {
    d11qf *qdat = (d11qf *)malloc(sizeof(d11qf) * D11_Q_CAP);
    d11queue queue = {.data = qdat, .qh = 0, .qb = 0, .size = 0};

    qpush11(&queue, (d11qf){ .data = start });

    // bfs
    // every time we encounter end, increment
    long long total = 0;
    while (queue.size != 0) {
        d11qf curr = qpop11(&queue);
        // printf("curr: %"PRIu32" -> ", curr.data); sv_print(&node_strs[curr.data]); printf("\n");
        uint32_t *curr_nbors = nbors[curr.data];
        size_t curr_nbors_ct = nbor_cts[curr.data];
        for (size_t i = 0; i < curr_nbors_ct; ++i) {
            uint32_t nbor = curr_nbors[i];
            if (nbor == end) {
                total++;
            } else {
                qpush11(&queue, (d11qf) { .data = nbor });
            }
        }
    }

    free(qdat);

    return total;
}

long long d11_solve_p1(char *input, const size_t input_sz)
{
    StringView lns = {0};
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};

    StringView node_strs[D11_MAX_LNS] = {0};
    int populated[D11_MAX_LNS] = {0};

    uint32_t nbors[D11_MAX_LNS][D11_MAX_LNS] = {0};
    size_t nbor_cts[D11_MAX_LNS] = {0};

    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D11_MAX_LNS);
        split_curr(&split, &lns);
        Splitter ln_split = {.buf=lns.buf, .mx=lns.len};
        delim(&ln_split, ' ');
        StringView node_str;
        split_curr(&ln_split, &node_str);
        --node_str.len;

        uint32_t node_idx = get_index(&node_str, &node_strs[0], &populated[0]);
        // printf("["); sv_print(&node_str); printf("](%"PRIu32")--> ", node_hash);
        while (delim(&ln_split, ' ')) {
            StringView nbor_str;
            split_curr(&ln_split, &nbor_str);
            if (nbor_str.buf[nbor_str.len-1] == '\n') --nbor_str.len;

            uint32_t nbor_idx = get_index(&nbor_str, &node_strs[0], &populated[0]);
            // printf("["); sv_print(&nbor_str); printf("](%"PRIu32") ", nbor_hash);
            nbors[node_idx][nbor_cts[node_idx]] = nbor_idx;
            nbor_cts[node_idx] += 1;
        }
        // printf("\n");
        lns_ct++;
    }

    // int visited[D11_MAX_LNS] = {0};
    StringView start_str = {.buf = "you", .len = 3};
    StringView end_str   = {.buf = "out", .len = 3};
    uint32_t start = get_index(&start_str, &node_strs[0], &populated[0]);
    uint32_t end   = get_index(&end_str,   &node_strs[0], &populated[0]);
    return d11_dist(start, end, nbors, nbor_cts);
}

long long paths(uint32_t src, uint32_t dst, uint32_t *po, size_t pol, uint32_t nbors[D11_MAX_LNS][D11_MAX_LNS], size_t nbor_cts[D11_MAX_LNS]) {
    // printf("src[%"PRIu32"] -> dst[%"PRIu32"]: ", src, dst);
    size_t start = 0;
    while (start < pol && po[start] != dst) {
        start++;
    }
    // printf("[%zu] [%zu] %"PRIu32" == %"PRIu32"\n", start, pol, src, po[start]);
    assert(start < pol && "Did not find dst in the post order!");
    size_t end = 0;
    while (end < pol && po[end] != src) {
        end++;
    }
    assert(end < pol && "Did not find src in the post order!");

    if (start > end) {
        // printf("0 (start[%zu] > end[%zu])\n", start, end);
        return 0;
    }

    int visited[D11_MAX_LNS] = {0};
    visited[dst] = 1;
    for (size_t idx = start+1; idx <= end; ++idx) {
        uint32_t curr = po[idx];
        int count = 0;
        uint32_t *curr_nbors = nbors[curr];
        size_t curr_nbors_ct = nbor_cts[curr];
        for (size_t jdx = 0; jdx < curr_nbors_ct; ++jdx) {
            count += visited[curr_nbors[jdx]];
        }
        visited[curr] = count;
        // printf("%"PRIu32": %d\n", curr, count);
    }

    // printf("%d\n", visited[src]);
    return visited[src];
}

long long d11_solve_p2(char *input, const size_t input_sz)
{
    StringView lns = {0};
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};

    StringView node_strs[D11_MAX_LNS] = {0};
    int populated[D11_MAX_LNS] = {0};

    uint32_t nbors[D11_MAX_LNS][D11_MAX_LNS] = {0};
    size_t nbor_cts[D11_MAX_LNS] = {0};

    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D11_MAX_LNS);
        split_curr(&split, &lns);
        Splitter ln_split = {.buf=lns.buf, .mx=lns.len};
        delim(&ln_split, ' ');
        StringView node_str;
        split_curr(&ln_split, &node_str);
        --node_str.len;

        uint32_t node_idx = get_index(&node_str, &node_strs[0], &populated[0]);
        // printf("["); sv_print(&node_str); printf("](%"PRIu32")--> ", node_hash);
        while (delim(&ln_split, ' ')) {
            StringView nbor_str;
            split_curr(&ln_split, &nbor_str);
            if (nbor_str.buf[nbor_str.len-1] == '\n') --nbor_str.len;

            uint32_t nbor_idx = get_index(&nbor_str, &node_strs[0], &populated[0]);
            // printf("["); sv_print(&nbor_str); printf("](%"PRIu32") ", nbor_hash);
            nbors[node_idx][nbor_cts[node_idx]] = nbor_idx;
            nbor_cts[node_idx] += 1;
        }
        // printf("\n");
        lns_ct++;
    }

    // int visited[D11_MAX_LNS] = {0};
    StringView svr_str = {.buf = "svr", .len = 3};
    StringView dac_str = {.buf = "dac", .len = 3};
    StringView fft_str = {.buf = "fft", .len = 3};
    StringView out_str = {.buf = "out", .len = 3};
    uint32_t svr = get_index(&svr_str, &node_strs[0], &populated[0]);
    uint32_t dac = get_index(&dac_str, &node_strs[0], &populated[0]);
    uint32_t fft = get_index(&fft_str, &node_strs[0], &populated[0]);
    uint32_t out = get_index(&out_str, &node_strs[0], &populated[0]);
    // printf("svr: %"PRIu32"\n", svr);

    // push svr onto the stack
    // while stack is not empty, pop off stack
    // 
    typedef struct {
        uint32_t d;
        int seen;
    } sf;

    int visited[D11_MAX_LNS] = {0};
    uint32_t post_order[D11_MAX_LNS] = {0};
    size_t pol = 0;
    sf stack[D11_MAX_LNS] = {0};
    size_t stack_size = 0;

    stack[stack_size++] = (sf){ .d = svr, .seen = 0 };

    while (stack_size != 0) {
        sf curr = stack[--stack_size];
        if (curr.seen) {
            post_order[pol++] = curr.d;
            continue;
        }
        if (visited[curr.d]) {
            continue;
        }
        visited[curr.d] = 1;
        stack[stack_size++] = (sf){ .d = curr.d, .seen = 1 };

        uint32_t *curr_nbors = nbors[curr.d];
        size_t curr_nbors_ct = nbor_cts[curr.d];
        // sv_print(&node_strs[curr.d]); printf(" -> %zu nbors\n", curr_nbors_ct);
        for (size_t i = 0; i < curr_nbors_ct; ++i) {
            if (!visited[curr_nbors[i]]) {
                stack[stack_size++] = (sf){ .d = curr_nbors[i], .seen = 0 };
            }
        }
    }

    // for (size_t i = 0; i < pol; ++i) {
    //     sv_print(&node_strs[post_order[i]]); printf(" (%"PRIu32")\n", post_order[i]);
    // }


    // svr -> dac -> fft -> out
    // 
    // svr -> fft -> dac -> out
    long long svr_fft = paths(svr, fft, post_order, pol, nbors, nbor_cts);
    long long fft_dac = paths(fft, dac, post_order, pol, nbors, nbor_cts);
    long long dac_out = paths(dac, out, post_order, pol, nbors, nbor_cts);

    long long svr_dac = paths(svr, dac, post_order, pol, nbors, nbor_cts);
    long long dac_fft = paths(dac, fft, post_order, pol, nbors, nbor_cts);
    long long fft_out = paths(fft, out, post_order, pol, nbors, nbor_cts);

    // printf("%lld * %lld * %lld\n", svr_fft, fft_dac, dac_out);
    // printf("%lld * %lld * %lld\n", svr_dac, dac_fft, fft_out);

    long long total = (svr_fft * fft_dac * dac_out) + (svr_dac * dac_fft * fft_out);

    return total;
}

void solve_day11(const char *in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d11_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d11_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day11_part1) {
    char *input_str = 
        "aaa: you hhh\n"
        "you: bbb ccc\n"
        "bbb: ddd eee\n"
        "ccc: ddd eee fff\n"
        "ddd: ggg\n"
        "eee: out\n"
        "fff: out\n"
        "ggg: out\n"
        "hhh: ccc fff iii\n"
        "iii: out\n";

    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d11_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 5)
}

TEST(test_day11_part2) {
    char *input_str = 
        "svr: aaa bbb\n"
        "aaa: fft\n"
        "fft: ccc\n"
        "bbb: tty\n"
        "tty: ccc\n"
        "ccc: ddd eee\n"
        "ddd: hub\n"
        "hub: fff\n"
        "eee: dac\n"
        "dac: fff\n"
        "fff: ggg hhh\n"
        "ggg: out\n"
        "hhh: out\n";

    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d11_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 2)
}

void day11_tests()
{
    RUN_TEST(test_day11_part1);
    RUN_TEST(test_day11_part2);
}

#endif
