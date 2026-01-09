
#include "lib/aoc.h"

#define D12_MAX_LNS 2048
#define D12_MAX_SHAPES 6

long long d12_solve_p1(char *input, const size_t input_sz) {
    StringView lns = {0};
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};

    size_t curr = 0;
    size_t sizes[D12_MAX_SHAPES] = {0};

    long long total = 0;
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D12_MAX_LNS);
        split_curr(&split, &lns);

        if (curr < D12_MAX_SHAPES) {
            if (lns.len == 0) {
                curr++;
            } else {
                for (size_t i = 0; i < lns.len; ++i) {
                    if (lns.buf[i] == '#') {
                        ++sizes[curr];
                    }
                }
            }
        } else {
            Splitter ln_split = {
                .buf = lns.buf, .st = 0, .sz = 0, .mx = lns.len};
            StringView tmp = {0};
            delim(&ln_split, 'x');
            split_curr(&ln_split, &tmp);
            long long x = sv_atoll(&tmp);
            delim(&ln_split, ':');
            split_curr(&ln_split, &tmp);
            tmp.buf += 1;
            tmp.len -= 1;
            long long y = sv_atoll(&tmp);
            long long area = x * y;

            long long max = 0;
            delim(&ln_split, ' ');
            while (delim(&ln_split, ' ')) {
                split_curr(&ln_split, &tmp);
                long long ct = sv_atoll(&tmp);
                max += 9 * ct;
            }
            if (max <= area) {
                total++;
            }
        }

        lns_ct++;
    }

    return total;
}

void solve_day12(const char *in_file) {
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d12_solve_p1(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day12_part1) {
    char *input_str =
        "0:\n"
        "###\n"
        "##.\n"
        "##.\n"
        "\n"
        "1:\n"
        "###\n"
        "##.\n"
        ".##\n"
        "\n"
        "2:\n"
        ".##\n"
        "###\n"
        "##.\n"
        "\n"
        "3:\n"
        "##.\n"
        "###\n"
        "##.\n"
        "\n"
        "4:\n"
        "###\n"
        "#..\n"
        "###\n"
        "\n"
        "5:\n"
        "###\n"
        ".#.\n"
        "###\n"
        "\n"
        "12x5: 1 0 1 0 2 2\n"
        "12x5: 1 0 1 0 3 2\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char *)&input, &input_len);
    long long result = d12_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 1)
}

void day12_tests() { RUN_TEST(test_day12_part1); }

#endif
