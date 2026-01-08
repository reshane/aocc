#include "lib/aoc.h"

Point parse_range(StringView *rng) {
    Splitter split = {.buf = rng->buf, .st = 0, .sz = 0, .mx = rng->len};
    StringView start_str = {0};
    StringView end_str = {0};

    delim(&split, '-');
    split_curr(&split, &start_str);
    delim(&split, '-');
    split_curr(&split, &end_str);

    return (Point){.x = sv_atoll(&start_str), .y = sv_atoll(&end_str)};
}

// day 5
long long d5_solve_p1(char *input, size_t input_sz) {
    const int lines_max = 1500;
    StringView lns[1500] = {0};
    int ct = 0;
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};
    int parsing_ranges = 1;

    const int max_ranges = 179;
    Point ranges[179] = {0};
    int r_ct = 0;

    while (lines(&split) && parsing_ranges) {
        assert("Could not fit input lines into buffer!" && ct < lines_max);
        split_curr(&split, &lns[ct]);
        if (lns[ct].len != 0) {
            assert("Could not fit ranges into buffer!" && r_ct < max_ranges);
            ranges[r_ct] = parse_range(&lns[ct]);
            r_ct++;
        } else {
            parsing_ranges = 0;
        }
        ct += 1;
    }
    long long total = 0;
    while (lines(&split)) {
        split_curr(&split, &lns[ct]);
        long long ing = sv_atoll(&lns[ct]);
        for (int i = 0; i < r_ct; ++i) {
            if (ing >= ranges[i].x && ing <= ranges[i].y) {
                total += 1;
                break;
            }
        }
        ct += 1;
    }

    return total;
}

int comp_pt_x(const void *a, const void *b) {
    if (((Point *)a)->x == ((Point *)b)->x) {
        return 0;
    } else if (((Point *)a)->x < ((Point *)b)->x) {
        return -1;
    }
    return 1;
}

long long d5_solve_p2(char *input, size_t input_sz) {
    const int lines_max = 200;
    StringView lns[200] = {0};
    int ct = 0;
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};
    int parsing_ranges = 1;

    const int max_ranges = 179;
    Point ranges[179] = {0};
    int r_ct = 0;

    while (lines(&split) && parsing_ranges) {
        assert("Could not fit input lines into buffer!" && ct < lines_max);
        split_curr(&split, &lns[ct]);
        if (lns[ct].len != 0) {
            assert("Could not fit ranges into buffer!" && r_ct < max_ranges);
            ranges[r_ct++] = parse_range(&lns[ct]);
        } else {
            parsing_ranges = 0;
        }
        ct += 1;
    }

    qsort(&ranges[0], (size_t)r_ct, sizeof(ranges[0]), &comp_pt_x);

    int cr_idx = 0;
    for (int i = 1; i < r_ct; ++i) {
        // to combine = cr_idx stays the same
        // ranges[cr_idx].y = ranges[i].y
        // else
        // ranges[++cr_idx] = ranges[i];
        if (ranges[cr_idx].y + 1 < ranges[i].x) {
            if (++cr_idx != i)
                ranges[cr_idx] = ranges[i];
        } else if (ranges[cr_idx].y < ranges[i].y) {
            ranges[cr_idx].y = ranges[i].y;
        }
    }

    long long total = 0;
    for (int i = 0; i <= cr_idx; ++i) {
        total += ranges[i].y - ranges[i].x + 1;
    }

    return total;
}

void solve_day5(const char *in_file) {
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d5_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d5_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day5_part1) {
    char *input_str = "3-5\n"
                      "10-14\n"
                      "16-20\n"
                      "12-18\n"
                      "\n"
                      "1\n"
                      "5\n"
                      "8\n"
                      "11\n"
                      "17\n"
                      "32\n";
    // aoc_fill_input(input_str);
    long long result = d5_solve_p1(input_str, strlen(input_str));
    // printf("%lld\n", result);
    ASSERT(result == 3)
}

TEST(test_day5_part2) {
    char *input_str = "3-5\n"
                      "10-14\n"
                      "16-20\n"
                      "12-18\n"
                      "\n"
                      "1\n"
                      "5\n"
                      "8\n"
                      "11\n"
                      "17\n"
                      "32\n";
    // aoc_fill_input(input_str);
    long long result = d5_solve_p2(input_str, strlen(input_str));
    // printf("%lld\n", result);
    ASSERT(result == 14)
}

void day05_tests() {
    RUN_TEST(test_day5_part1);
    RUN_TEST(test_day5_part2);
}

#endif
