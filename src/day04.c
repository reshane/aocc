#include "lib/aoc.h"

#define D4_LINES_MAX 200
#define D4_PTS_MAX 2<<15

void d4_parse_input(char* input, size_t input_sz, StringView* d4_lines, int* d4_ct)
{
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};
    while (lines(&split)) {
        // printf("%d %d\n", (*d4_ct) + 1, D4_PTS_MAX);
        assert("Ran out of space in d4_lines" && D4_LINES_MAX > *d4_ct);
        split_curr(&split, &d4_lines[(*d4_ct)++]);
    }
}

long long d4_count(StringView* d4_lines, int d4_ct, Point* d4_pts, int* d4_pts_head)
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
                assert("d4_pts ran out of space" && (*d4_pts_head) + 1 < D4_PTS_MAX);
                d4_pts[(*d4_pts_head)++] = (Point){.x = c, .y = l};
            }
        }
    }
    return *d4_pts_head;
}

// day 4
long long d4_solve_p1(char* input, size_t input_sz)
{
    StringView d4_lines[D4_LINES_MAX] = {0};
    int d4_ct = 0;

    Point d4_pts[D4_PTS_MAX] = {0};
    int d4_pts_head = 0;

    d4_parse_input(input, input_sz, (StringView*)&d4_lines, &d4_ct);
    return d4_count((StringView*)&d4_lines, d4_ct, (Point*)&d4_pts, &d4_pts_head);
}

long long d4_solve_p2(char* input, size_t input_sz)
{
    StringView d4_lines[D4_LINES_MAX] = {0};
    int d4_ct = 0;

    Point d4_pts[D4_PTS_MAX] = {0};
    int d4_pts_head = 0;

    long long total = 0;
    d4_parse_input(input, input_sz, (StringView*)&d4_lines, &d4_ct);
    while (d4_count((StringView*)&d4_lines, d4_ct, (Point*)&d4_pts, &d4_pts_head) > 0) {
        total += d4_pts_head;
        do {
            Point p = d4_pts[--d4_pts_head];
            assert("d4_lines out of bounds" && D4_LINES_MAX > p.y);
            assert("d4_lines out of bounds x" && d4_lines[p.y].len > p.x);
            d4_lines[p.y].buf[p.x] = '.';
        } while (d4_pts_head > 0);
    }
    return total;
}

void solve_day4(const char* in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d4_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d4_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day4_part1) {
    char *input_str = 
        "..@@.@@@@.\n"
        "@@@.@.@.@@\n"
        "@@@@@.@.@@\n"
        "@.@@@@..@.\n"
        "@@.@@@@.@@\n"
        ".@@@@@@@.@\n"
        ".@.@.@.@@@\n"
        "@.@@@.@@@@\n"
        ".@@@@@@@@.\n"
        "@.@.@@@.@.\n";
    char input[200];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d4_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 13)
}

TEST(test_day4_part2) {
    char *input_str = 
        "..@@.@@@@.\n"
        "@@@.@.@.@@\n"
        "@@@@@.@.@@\n"
        "@.@@@@..@.\n"
        "@@.@@@@.@@\n"
        ".@@@@@@@.@\n"
        ".@.@.@.@@@\n"
        "@.@@@.@@@@\n"
        ".@@@@@@@@.\n"
        "@.@.@@@.@.\n";
    char input[200];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d4_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 43)
}

void day04_tests()
{
    RUN_TEST(test_day4_part1);
    RUN_TEST(test_day4_part2);
}

#endif
