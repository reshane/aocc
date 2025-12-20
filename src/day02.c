#include "lib/aoc.h"

int is_invalid(StringView *sv)
{
    if (sv->len % 2 != 0) return 1;

    int i = 0, j = sv->len/2;
    while (j < sv->len) {
        if (sv->buf[i] != sv->buf[j]) {
            return 1;
        }
        i++; j++;
    }
    return 0;
}

// day 2
long long d2_solve_p1()
{
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};

    StringView sv = {0};

    long long total = 0;
    while (delim(&split, ',')) {
        split_curr(&split, &sv);
        sv_trim_whitespace(&sv);
        StringView first = {0};
        StringView second = {0};
        Splitter subsplit = {.buf = sv.buf, .st = 0, .mx = sv.len};
        delim(&subsplit, '-');
        split_curr(&subsplit, &first);
        delim(&subsplit, '-');
        split_curr(&subsplit, &second);

        long long f = sv_atoll(&first);
        long long s = sv_atoll(&second);

        StringView t_str = {0};
        char buf[64];
        for (long long i = f; i <= s; ++i) {
            sprintf(buf, "%lld", i);
            t_str.buf = buf;
            t_str.len = strlen(buf);
            if (is_invalid(&t_str) == 0) {
                total += i;
            }
        }
    }
    return total;
}

int repeats_n(StringView *sv, int div)
{
    StringView control = {.buf = sv->buf, .len = div};
    int s = div;
    while (s < sv->len) {
        StringView t = {.buf = sv->buf + s, .len = div};
        if (sv_cmp(&t, &control) != 0) {
            return 1;
        }
        s += div;
    }
    return 0;
}

int is_invalid_2(StringView *sv)
{
    int half_len = sv->len / 2;
    for (int i = 1; i <= half_len; ++i) {
        if (sv->len % i == 0 && repeats_n(sv, i) == 0) {
            return 0;
        }
    }
    return 1;
}

long long d2_solve_p2()
{
    Splitter split = {.buf = input, .st = 0, .sz = 0, .mx = input_sz};

    StringView sv = {0};

    long long total = 0;
    while (delim(&split, ',')) {
        split_curr(&split, &sv);
        sv_trim_whitespace(&sv);
        StringView first = {0};
        StringView second = {0};
        Splitter subsplit = {.buf = sv.buf, .st = 0, .mx = sv.len};
        delim(&subsplit, '-');
        split_curr(&subsplit, &first);
        delim(&subsplit, '-');
        split_curr(&subsplit, &second);

        long f = sv_atoll(&first);
        long s = sv_atoll(&second);

        StringView t_str = {0};
        char buf[64];
        for (long i = f; i <= s; ++i) {
            sprintf(buf, "%ld", i);
            t_str.buf = buf;
            t_str.len = strlen(buf);
            if (is_invalid_2(&t_str) == 0) {
                total += i;
            }
        }
    }
    return total;
}

void solve_day2(const char* in_file)
{
    if (slurp_file(in_file) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d2_solve_p1());
    printf("\033[1mPart 2: %lld\n\033[0m", d2_solve_p2());
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day2_part1) {
    char *input_str = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,\n"
        "1698522-1698528,446443-446449,38593856-38593862,565653-565659,\n"
        "824824821-824824827,2121212118-2121212124\n";
    fill_input(input_str);
    long long result = d2_solve_p1();
    // printf("%lld\n", result);
    ASSERT(result == 1227775554)
}

TEST(test_day2_sv_trims) {
    char* t_string = "   \n\t shane\n\t\t  \t";
    char* a_string = "shane\n\t\t  \t";
    StringView sv1 = {.buf = t_string, .len = strlen(t_string) };
    StringView sv2 = {.buf = a_string, .len = strlen(a_string) };
    sv_trim_left_whitespace(&sv1);
    ASSERT(sv_cmp(&sv1, &sv2) == 0)

    t_string = "   \n\t shane\n\t\t  \t";
    a_string = "   \n\t shane";
    sv1.buf = t_string;
    sv1.len = strlen(t_string);
    sv2.buf = a_string;
    sv2.len = strlen(a_string);
    sv_trim_right_whitespace(&sv1);
    ASSERT(sv_cmp(&sv1, &sv2) == 0)

    t_string = "   \n\t shane\n\t\t  \t";
    a_string = "shane";
    sv1.buf = t_string;
    sv1.len = strlen(t_string);
    sv2.buf = a_string;
    sv2.len = strlen(a_string);
    sv_trim_whitespace(&sv1);
    ASSERT(sv_cmp(&sv1, &sv2) == 0)
}

TEST(test_day2_is_invalid) {
    long long t = 565659;
    char buf[64];
    sprintf(buf, "%lld", t);
    StringView sv1 = {.buf = buf, .len = strlen(buf) };
    long res = is_invalid_2(&sv1);
    // printf("%ld\n", res);
    ASSERT(res != 0);

    t = 565650;
    sprintf(buf, "%lld", t);
    sv1.buf = buf;
    sv1.len = strlen(buf);
    res = is_invalid_2(&sv1);
    // printf("%ld\n", res);
    ASSERT(res != 0);

    t = 565656;
    sprintf(buf, "%lld", t);
    sv1.buf = buf;
    sv1.len = strlen(buf);
    res = is_invalid_2(&sv1);
    // printf("%ld\n", res);
    ASSERT(res == 0);
}

TEST(test_day2_part2) {
    char *input_str = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,\n"
        "1698522-1698528,446443-446449,38593856-38593862,565653-565659,\n"
        "824824821-824824827,2121212118-2121212124\n";
    fill_input(input_str);
    long long result = d2_solve_p2();
    // printf("%lld\n", result);
    ASSERT(result == 4174379265)
}

void day02_tests()
{
    RUN_TEST(test_day2_sv_trims);
    RUN_TEST(test_day2_part1);
    RUN_TEST(test_day2_part2);
    RUN_TEST(test_day2_is_invalid);
}

#endif
