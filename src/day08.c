#include "lib/aoc.h"

#define D8_MAX_LNS 1024

typedef struct p38 {
    long long x,y,z;
} p38;

// edge - a and b are indices into the pts array
// d is the distance
typedef struct p8e {
    size_t a,b;
    long long d;
} p8e;

long long p38_dist(const p38 *p1, const p38 *p2)
{
    long long dx = p1->x - p2->x;
    long long dy = p1->y - p2->y;
    long long dz = p1->z - p2->z;
    return dx*dx+dy*dy+dz*dz;
}

int comp_p8e(const void *p1, const void *p2)
{
    p8e* a = (p8e*)p1;
    p8e* b = (p8e*)p2;
    if (a->d > b->d) {
        return 1;
    } else if (a->d == b->d) {
        return 0;
    }
    return -1;
}

int comp_szt(const void *sz1, const void *sz2)
{
    size_t a,b;
    a = *(size_t*)sz1;
    b = *(size_t*)sz2;
    if (a > b) {
        return -1;
    } else if (a == b) {
        return 0;
    }
    return 1;
}


// day 8
long long d8p1_solver(char *input, const size_t input_sz, const size_t iterations)
{
    static StringView lns[D8_MAX_LNS];
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    static p38 pts[D8_MAX_LNS];
    static p8e edges[(D8_MAX_LNS*(D8_MAX_LNS-1))/2];
    size_t ect = 0;
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D8_MAX_LNS);
        split_curr(&split, &lns[lns_ct]);

        StringView sv = {0};
        Splitter subsplit = {.buf = lns[lns_ct].buf, .mx = lns[lns_ct].len};
        delim(&subsplit, ',');
        split_curr(&subsplit, &sv);
        long long x = sv_atoll(&sv);
        delim(&subsplit, ',');
        split_curr(&subsplit, &sv);
        long long y = sv_atoll(&sv);
        delim(&subsplit, ',');
        split_curr(&subsplit, &sv);
        long long z = sv_atoll(&sv);

        pts[lns_ct] = (p38){.x=x, .y=y, .z=z};
        for (size_t idx = 0; idx < lns_ct; ++idx) {
            long long dist = p38_dist(&pts[lns_ct], &pts[idx]);
            edges[ect] = (p8e){.a=lns_ct, .b=idx, .d=dist};
            ect++;
        }

        lns_ct++;
    }

    qsort((void*)&edges, ect, sizeof(edges[0]), &comp_p8e);

    // each node starts of as 0 -> not in a group
    // for each edge, put the two nodes in group curr_group
    static size_t groups[D8_MAX_LNS];
    size_t curr_group = 1;
    for (size_t idx = 0; idx < iterations; ++idx) {
        size_t a = edges[idx].a;
        size_t b = edges[idx].b;
        if (groups[a]) {
            // merge into a
            // set all indices in groups to groups[a] where groups[i] = groups[b];
            size_t new = groups[a];
            size_t old = groups[b];
            if (old) {
                for (size_t i = 0; i < ect; ++i) {
                    if (groups[i] == old) {
                        groups[i] = new;
                    }
                }
            } else {
                groups[b] = new;
            }
        } else if (groups[b]) {
            // merge into b
            size_t new = groups[b];
            size_t old = groups[a];
            if (old) {
                for (size_t i = 0; i < ect; ++i) {
                    if (groups[i] == old) {
                        groups[i] = new;
                    }
                }
            } else {
                groups[a] = new;
            }
        } else {
            groups[a] = curr_group;
            groups[b] = curr_group;
            curr_group++;
        }
    }

    static size_t group_cts[(D8_MAX_LNS*(D8_MAX_LNS-1))/2];
    for (size_t x = 0; x < lns_ct; ++x) {
        if (groups[x]) {
            group_cts[groups[x]-1] += 1;
        }
    }
    qsort((void*)&group_cts, ((D8_MAX_LNS*(D8_MAX_LNS-1))/2)-1, sizeof(group_cts[0]), &comp_szt);
    long long total = 1;
    for (size_t x = 0; x < 3; ++x) {
        total *= group_cts[x];
    }
    return total;
}

long long d8_solve_p1(char *input, const size_t input_sz)
{
    return d8p1_solver(input, input_sz, 1000);
}

long long d8_solve_p2(char *input, const size_t input_sz)
{
    static StringView lns[D8_MAX_LNS];
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    static p38 pts[D8_MAX_LNS];
    static p8e edges[(D8_MAX_LNS*(D8_MAX_LNS-1))/2];
    size_t ect = 0;
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D8_MAX_LNS);
        split_curr(&split, &lns[lns_ct]);

        StringView sv = {0};
        Splitter subsplit = {.buf = lns[lns_ct].buf, .mx = lns[lns_ct].len};
        delim(&subsplit, ',');
        split_curr(&subsplit, &sv);
        long long x = sv_atoll(&sv);
        delim(&subsplit, ',');
        split_curr(&subsplit, &sv);
        long long y = sv_atoll(&sv);
        delim(&subsplit, ',');
        split_curr(&subsplit, &sv);
        long long z = sv_atoll(&sv);

        pts[lns_ct] = (p38){.x=x, .y=y, .z=z};
        for (size_t idx = 0; idx < lns_ct; ++idx) {
            long long dist = p38_dist(&pts[lns_ct], &pts[idx]);
            edges[ect] = (p8e){.a=lns_ct, .b=idx, .d=dist};
            ect++;
        }

        lns_ct++;
    }

    qsort((void*)&edges, ect, sizeof(edges[0]), &comp_p8e);

    // each node starts of as 0 -> not in a group
    // for each edge, put the two nodes in group curr_group
    static size_t groups[D8_MAX_LNS];
    size_t grouped = 0;
    size_t curr_group = 1;
    for (size_t idx = 0; idx < ect; ++idx) {
        size_t a = edges[idx].a;
        size_t b = edges[idx].b;
        if (groups[a]) {
            // merge into a
            // set all indices in groups to groups[a] where groups[i] = groups[b];
            size_t new = groups[a];
            size_t old = groups[b];
            if (old) {
                for (size_t i = 0; i < lns_ct; ++i) {
                    if (groups[i] == old) {
                        groups[i] = new;
                    }
                }
            } else {
                groups[b] = new;
                grouped++;
            }
        } else if (groups[b]) {
            // merge into b
            size_t new = groups[b];
            size_t old = groups[a];
            if (old) {
                for (size_t i = 0; i < lns_ct; ++i) {
                    if (groups[i] == old) {
                        groups[i] = new;
                    }
                }
            } else {
                groups[a] = new;
                grouped++;
            }
        } else {
            groups[a] = curr_group;
            groups[b] = curr_group;
            curr_group++;
            grouped += 2;
        }

        if (grouped >= lns_ct) {
            size_t same = 1;
            for (size_t jdx = 1; jdx < lns_ct; ++jdx) {
                if (groups[jdx] != groups[jdx-1]) {
                    jdx = D8_MAX_LNS;
                    same = 0;
                }
            }
            if (same) {
                return pts[a].x * pts[b].x;
            }
        }
    }
    return -1;
}

void solve_day8(const char *in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d8_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d8_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day8_part1) {
    char *input_str = 
        "162,817,812\n"
        "57,618,57\n"
        "906,360,560\n"
        "592,479,940\n"
        "352,342,300\n"
        "466,668,158\n"
        "542,29,236\n"
        "431,825,988\n"
        "739,650,466\n"
        "52,470,668\n"
        "216,146,977\n"
        "819,987,18\n"
        "117,168,530\n"
        "805,96,715\n"
        "346,949,466\n"
        "970,615,88\n"
        "941,993,340\n"
        "862,61,35\n"
        "984,92,344\n"
        "425,690,689\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d8p1_solver(input, input_len, 10);
    // printf("%lld\n", result);
    ASSERT(result == 40)
}

TEST(test_day8_part2) {
    char *input_str = 
        "162,817,812\n"
        "57,618,57\n"
        "906,360,560\n"
        "592,479,940\n"
        "352,342,300\n"
        "466,668,158\n"
        "542,29,236\n"
        "431,825,988\n"
        "739,650,466\n"
        "52,470,668\n"
        "216,146,977\n"
        "819,987,18\n"
        "117,168,530\n"
        "805,96,715\n"
        "346,949,466\n"
        "970,615,88\n"
        "941,993,340\n"
        "862,61,35\n"
        "984,92,344\n"
        "425,690,689\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d8_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 25272)
}

void day08_tests()
{
    RUN_TEST(test_day8_part1);
    RUN_TEST(test_day8_part2);
}

#endif
