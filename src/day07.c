#include "lib/aoc.h"

#define D7_MAX_LNS 256

// day 7
long long d7_solve_p1(char* input, size_t input_sz)
{
    StringView lns[D7_MAX_LNS];
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D7_MAX_LNS);
        split_curr(&split, &lns[lns_ct++]);
    }
    long long total = 0;
    for (size_t idx = 2; idx < lns_ct; idx += 2) {
        for (size_t jdx = 0; jdx < lns[idx].len; ++jdx) {
            if (lns[idx-2].buf[jdx] == '|' || lns[idx-2].buf[jdx] == 'S') {
                if (lns[idx].buf[jdx] == '^') {
                    total += 1;
                    if (jdx > 0 && lns[idx].buf[jdx-1] != '^') {
                        lns[idx].buf[jdx-1] = '|';
                    }
                    if (jdx < lns[idx].len-1 && lns[idx].buf[jdx+1] != '^') {
                        lns[idx].buf[jdx+1] = '|';
                    }
                } else {
                    lns[idx].buf[jdx] = '|';
                }
            }
        }
    }
    return total;
}

long long d7_solve_p2(char* input, size_t input_sz)
{
    long long weights[D7_MAX_LNS][D7_MAX_LNS];
    StringView lns[D7_MAX_LNS];
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D7_MAX_LNS);
        split_curr(&split, &lns[lns_ct]);
        for (size_t idx = 0; idx < lns[lns_ct].len; ++idx) {
            if (lns[lns_ct].buf[idx] == 'S') {
                weights[lns_ct][idx] = 1;
            } else if (lns[lns_ct].buf[idx] == '^') {
                weights[lns_ct][idx] = -1;
            } else {
                weights[lns_ct][idx] = 0;
            }
        }
        lns_ct++;
    }

    for (size_t idx = 2; idx < lns_ct; idx += 2) {
        for (size_t jdx = 0; jdx < lns[idx].len; ++jdx) {
            if (weights[idx-2][jdx] > 0) {
                if (weights[idx][jdx] == -1) {
                    if (jdx > 0 && weights[idx][jdx-1] > -1) {
                        weights[idx][jdx-1] += weights[idx-2][jdx];
                    }
                    if (jdx < lns[idx].len-1 && weights[idx][jdx+1] > -1) {
                        weights[idx][jdx+1] += weights[idx-2][jdx];
                    }
                } else {
                    weights[idx][jdx] += weights[idx-2][jdx];
                }
            }
        }
    }
    long long total = 0;
    for (int idx = 0; idx < lns[lns_ct-2].len; ++idx) {
        if (weights[lns_ct-2][idx] > 0) {
            total += weights[lns_ct-2][idx];
        }
    }
    return total;
}

void solve_day7(const char* in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d7_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d7_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day7_part1) {
    char *input_str = 
        ".......S.......\n"
        "...............\n"
        ".......^.......\n"
        "...............\n"
        "......^.^......\n"
        "...............\n"
        ".....^.^.^.....\n"
        "...............\n"
        "....^.^...^....\n"
        "...............\n"
        "...^.^...^.^...\n"
        "...............\n"
        "..^...^.....^..\n"
        "...............\n"
        ".^.^.^.^.^...^.\n"
        "...............\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d7_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 21)
}

TEST(test_day7_part2) {
    char *input_str = 
        ".......S.......\n"
        "...............\n"
        ".......^.......\n"
        "...............\n"
        "......^.^......\n"
        "...............\n"
        ".....^.^.^.....\n"
        "...............\n"
        "....^.^...^....\n"
        "...............\n"
        "...^.^...^.^...\n"
        "...............\n"
        "..^...^.....^..\n"
        "...............\n"
        ".^.^.^.^.^...^.\n"
        "...............\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d7_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 40)
}

void day07_tests()
{
    RUN_TEST(test_day7_part1);
    RUN_TEST(test_day7_part2);
}

#endif
