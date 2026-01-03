#include "lib/aoc.h"

#define D6_MAX_ARGS 1024

// day 6
long long d6_solve_p1(char *input, size_t input_sz)
{
    StringView lns[5] = {0};
    int lns_ct = 0;

    Splitter split = {.buf = input, .mx = input_sz};
    while (lines(&split)) {
        split_curr(&split, &lns[lns_ct++]);
    }

    int args[4][D6_MAX_ARGS] = {0};
    int ops[D6_MAX_ARGS] = {0};
    int x = 0;
    for (int idx = 0; idx < lns_ct-1; ++idx) {
        Splitter ln_split = {.buf = lns[idx].buf, .mx = lns[idx].len-1};
        x = 0;
        while (delim(&ln_split, ' ')) {
            StringView sv = {0};
            split_curr(&ln_split, &sv);
            if (sv.len != 0) {
                assert("Ran out of argument space" && x+1 < D6_MAX_ARGS);
                args[idx][x++] = sv_atoll(&sv);
            }
        }
    }

    Splitter ln_split = {.buf = lns[lns_ct-1].buf, .mx = lns[lns_ct-1].len};
    int op_idx = 0;
    while(delim(&ln_split, ' ') && op_idx < x) {
        StringView sv = {0};
        split_curr(&ln_split, &sv);
        if (sv.len != 0 && (sv.buf[0] == '*' || sv.buf[0] == '+')) {
            ops[op_idx++] = sv.buf[0];
        }
    }


    long long total = 0;
    for (int jdx = 0; jdx < x; ++jdx) {
        long long line_total = 0;
        if (ops[jdx] == '*') line_total++;
        for (int idx = 0; idx < lns_ct-1; ++idx) {
            if (ops[jdx] == '*') {
                line_total *= args[idx][jdx];
            } else {
                line_total += args[idx][jdx];
            }
        }
        total += line_total;
    }

    return total;
}

long long d6_solve_p2(char *input, size_t input_sz)
{
    StringView lns[5] = {0};
    int lns_ct = 0;

    Splitter split = {.buf = input, .mx = input_sz};
    while (lines(&split)) {
        split_curr(&split, &lns[lns_ct++]);
    }

    long long total = 0;
    int args[256] = {0};
    int args_ct = 0;
    int op = ' ';
    for (size_t idx = 0; idx < lns[lns_ct-1].len; ++idx) {
        int operand = 0;
        if (lns[lns_ct-1].buf[idx] != ' ') {
            op = lns[lns_ct-1].buf[idx];
        }
        for (int jdx = 0; jdx < lns_ct-1; ++jdx) {
            if (lns[jdx].buf[idx] != ' ') {
                operand *= 10;
                operand += lns[jdx].buf[idx]-48;
            }
        }
        if (operand == 0) {
            long long sub_total = 0;
            if (op == '*') sub_total++;
            while (args_ct) {
                if (op == '*') sub_total *= args[args_ct-1];
                else sub_total += args[args_ct-1];
                args_ct--;
            }
            total += sub_total;
        } else {
            args[args_ct++] = operand;
        }
    }
    long long sub_total = 0;
    if (op == '*') sub_total++;
    while (args_ct) {
        if (op == '*') sub_total *= args[args_ct-1];
        else sub_total += args[args_ct-1];
        args_ct--;
    }
    total += sub_total;

    return total;
}

void solve_day6(const char *in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d6_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d6_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day6_part1) {
    char *input_str = 
        "123 328  51 64 \n"
        " 45 64  387 23 \n"
        "  6 98  215 314\n"
        "*   +   *   +  \n";
    char input[200];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d6_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 4277556)
}

TEST(test_day6_part2) {
    char *input_str = 
        "123 328  51 64 \n"
        " 45 64  387 23 \n"
        "  6 98  215 314\n"
        "*   +   *   +  \n";
    char input[200];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d6_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 3263827)
}

void day06_tests()
{
    RUN_TEST(test_day6_part1);
    RUN_TEST(test_day6_part2);
}

#endif
