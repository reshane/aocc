#include "lib/aoc.h"

void d3_parse_input(StringView *lns, int *ct, char *input, size_t input_sz)
{
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};
    while (lines(&split)) {
        split_curr(&split, &lns[*ct]);
        *ct += 1;
    }
}

// day 3
long long d3_solve_p1(char *input, size_t input_sz)
{
    StringView lines[200] = {0};
    int ct = 0;
    d3_parse_input((StringView*)&lines, &ct, input, input_sz);
    long long total = 0;
    for (int b = 0; b < ct; ++b) {
        int line_len = lines[b].len;
        int max_idx = 0;
        int max = -1;
        for (int i = 0; i < line_len-1; ++i) {
            if (max < lines[b].buf[i]-48) {
                max_idx = i;
                max = lines[b].buf[i]-48;
            }
        }
        int sub_max_idx = max_idx+1;
        int sub_max = -1;
        for (int j = max_idx + 1; j < line_len; ++j) {
            if (sub_max < lines[b].buf[j]-48) {
                sub_max_idx = j;
                sub_max = lines[b].buf[j]-48;
            }
        }
        total += max * 10 + sub_max;
    }
    return total;
}

int d3_digits(int d)
{
    int digs = 0;
    while (d > 0) {
        d /= 10;
        digs += 1;
    }
    return digs;
}

long long d3_get_max(StringView *bank)
{
    long long m[256][13] = {0};
    for (size_t i = 1; i <= bank->len; ++i) {
        int q = bank->buf[i-1]-48;
        for (int j = 1; j <= 12; ++j) {
            long long added = m[i][j-1] * 10 + q;
            if (d3_digits(added) > j) {
                m[i][j] = m[i-1][j];
            } else {
                long long a = m[i-1][j];
                long long b = m[i-1][j-1]*10+q;
                if (a > b) {
                    m[i][j] = a;
                } else {
                    m[i][j] = b;
                }
            }
        }
    }
    return m[bank->len][12];
}

long long d3_solve_p2(char *input, size_t input_sz)
{
    long long total = 0;
    // knapsack problem
    StringView banks[200] = {0};
    int ct = 0;
    d3_parse_input((StringView*)&banks, &ct, input, input_sz);
    // printf("%d\n", ct);
    for (int b = 0; b < ct; ++b) {
        sv_trim_whitespace(&banks[b]);
        // printf("%d", b);
        total += d3_get_max(&banks[b]);
    }
    return total;
}

void solve_day3(const char* in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d3_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d3_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day3_part1) {
    char *input_str = "987654321111111\n"
        "811111111111119\n"
        "234234234234278\n"
        "818181911112111\n";
    // fill_input(input_str);
    long long result = d3_solve_p1(input_str, strlen(input_str));
    // printf("%lld\n", result);
    ASSERT(result == 357)
}

TEST(test_day3_part2) {
    char *input_str = "987654321111111\n"
        "811111111111119\n"
        "234234234234278\n"
        "818181911112111\n";
    // fill_input(input_str);
    long long result = d3_solve_p2(input_str, strlen(input_str));
    // printf("%lld\n", result);
    ASSERT(result == 3121910778619)
    // ASSERT(result == 1)
}

void day03_tests()
{
    RUN_TEST(test_day3_part1);
    RUN_TEST(test_day3_part2);
}

#endif
