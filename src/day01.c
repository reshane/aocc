#include "lib/aoc.h"

// day 1
long long d1_solve_p1(char *input, size_t input_sz)
{
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};

    char d[10];
    char *left = "L";
    StringView lsv = {.buf = left, .len = strlen(left) };
    StringView sv = {0};

    int total = 50;
    int res = 0;
    while (lines(&split)) {
        split_curr(&split, &sv);
        StringView first   = {.buf = sv.buf,   .len = 1};
        StringView second  = {.buf = sv.buf+1, .len = sv.len-1};
        snprintf(d, (int)second.len+1, "%.*s", (int)second.len, second.buf);
        int rot = atoi(d);
        if (sv_cmp(&first, &lsv) == 0) {
            rot *= -1;
        }
        total += rot;
        total %= 100;
        if (total < 0) {
            total += 100;
        }

        if (total == 0) res++;
    }
    return res;
}

long long d1_solve_p2(char *input, size_t input_sz)
{
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};

    char d[10];
    char *left = "L";
    StringView lsv = {.buf = left, .len = strlen(left) };
    StringView sv = {0};

    int curr = 50;
    int total = 0;
    while (lines(&split)) {
        split_curr(&split, &sv);
        StringView first   = {.buf = sv.buf,   .len = 1};
        StringView second  = {.buf = sv.buf+1, .len = sv.len-1};
        snprintf(d, (int)second.len+1, "%.*s", (int)second.len, second.buf);
        int rot = atoi(d);
        if (sv_cmp(&first, &lsv) == 0) {
            rot *= -1;
        }

        int prev = curr;
        if (rot >= 100) {
            total += rot / 100;
        } else if (rot <= -100) {
            total += (-1 * rot) / 100;
        }
        curr += rot % 100;
        if (curr < 0) {
            // negative
            if (prev != 0) {
                total += 1;
            }
            curr = (curr % 100) + 100;
        } else if (curr >= 100) {
            total += 1;
            curr = curr % 100;
        } else if (curr == 0) {
            total += 1;
        }
    }
    return total;
}

void solve_day1(const char *in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d1_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d1_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day1_part1) {
    char *input_str = "L68\n"
        "L30\n"
        "R48\n"
        "L5\n"
        "R60\n"
        "L55\n"
        "L1\n"
        "L99\n"
        "R14\n"
        "L82\n";
    // fill_input(input_str);
    ASSERT(d1_solve_p1(input_str, strlen(input_str)) == 3)
}

TEST(test_day1_part2) {
    char *input_str = "L68\n"
        "L30\n"
        "R48\n"
        "L5\n"
        "R60\n"
        "L55\n"
        "L1\n"
        "L99\n"
        "R14\n"
        "L82\n";
    // aoc_fill_input(input_str);
    ASSERT(d1_solve_p2(input_str, strlen(input_str)) == 6)
}

void day01_tests()
{
    RUN_TEST(test_day1_part1);
    RUN_TEST(test_day1_part2);
}

#endif
