#include "lib/aoc.h"

StringView d4_lines[200] = {0};
int d4_ct = 0;

Point d4_pts[2<<15];
int d4_pts_head = 0;

void d4_reset()
{
    d4_pts_head = 0;
    d4_ct = 0;
}

void d4_parse_input()
{
    d4_reset();
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};
    while (lines(&split)) {
        split_curr(&split, &d4_lines[d4_ct]);
        d4_ct += 1;
    }
}

long long d4_count()
{
    for (int l = 0; l < d4_ct; ++l) {
        for (int c = 0; c < d4_lines[l].len; ++c) {
            int nbors = 0;
            if (l > 0) {
                if (d4_lines[l-1].buf[c] == '@') nbors++;
                if (c > 0 && d4_lines[l-1].buf[c-1] == '@') nbors++;
                if (c < d4_lines[l-1].len-1 && d4_lines[l-1].buf[c+1] == '@') nbors++;
            }
            if (l < d4_ct-1) {
                if (d4_lines[l+1].buf[c] == '@') nbors++;
                if (c > 0 && d4_lines[l+1].buf[c-1] == '@') nbors++;
                if (c < d4_lines[l+1].len-1 && d4_lines[l+1].buf[c+1] == '@') nbors++;
            }
            if (c < d4_lines[l].len-1 && d4_lines[l].buf[c+1] == '@') nbors++;
            if (c > 0 && d4_lines[l].buf[c-1] == '@') nbors++;

            if (nbors < 4 && d4_lines[l].buf[c] == '@') {
                d4_pts[d4_pts_head++] = (Point){.x = c, .y = l};
            }
        }
    }
    return d4_pts_head;
}

// day 4
long long d4_solve_p1()
{
    d4_parse_input();
    return d4_count();
}

long long d4_solve_p2()
{
    long long total = 0;
    d4_parse_input();
    while (d4_count() > 0) {
        total += d4_pts_head;
        do {
            Point p = d4_pts[--d4_pts_head];
            d4_lines[p.y].buf[p.x] = '.';
        } while (d4_pts_head > 0);
    }
    return total;
}

void solve_day4(const char* in_file)
{
    if (slurp_file(in_file) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d4_solve_p1());
    printf("\033[1mPart 2: %lld\n\033[0m", d4_solve_p2());
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day4_part1) {
    char *input_str = "..@@.@@@@.\n"
        "@@@.@.@.@@\n"
        "@@@@@.@.@@\n"
        "@.@@@@..@.\n"
        "@@.@@@@.@@\n"
        ".@@@@@@@.@\n"
        ".@.@.@.@@@\n"
        "@.@@@.@@@@\n"
        ".@@@@@@@@.\n"
        "@.@.@@@.@.\n";
    fill_input(input_str);
    long long result = d4_solve_p1();
    // printf("%lld\n", result);
    ASSERT(result == 13)
}

TEST(test_day4_part2) {
    char *input_str = "..@@.@@@@.\n"
        "@@@.@.@.@@\n"
        "@@@@@.@.@@\n"
        "@.@@@@..@.\n"
        "@@.@@@@.@@\n"
        ".@@@@@@@.@\n"
        ".@.@.@.@@@\n"
        "@.@@@.@@@@\n"
        ".@@@@@@@@.\n"
        "@.@.@@@.@.\n";
    fill_input(input_str);
    long long result = d4_solve_p2();
    // printf("%lld\n", result);
    ASSERT(result == 43)
}

void day04_tests()
{
    RUN_TEST(test_day4_part1);
    RUN_TEST(test_day4_part2);
}

#endif
