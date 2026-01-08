#include "lib/aoc.h"

#define D10_MAX_LNS 1024
#define D10_Q_CAP (2 << 25)

typedef struct {
    uint64_t data;
    size_t gen;
} qframe;

typedef struct {
    qframe *data;
    size_t qh, qb, size;
} d10queue;

void qpush(d10queue *q, qframe e) {
    if (q->size == D10_Q_CAP) {
        fprintf(stderr, "Could not push to queue, too many elements %zu > %d\n",
                q->size, D10_Q_CAP);
        exit(1);
    }
    q->data[q->qb++] = e;
    q->qb %= D10_Q_CAP;
    q->size += 1;
}

qframe qpop(d10queue *q) {
    if (q->size == 0) {
        fprintf(stderr, "Could not pop from an empty queue\n");
        exit(1);
    }
    qframe e = q->data[q->qh];
    q->qh += 1;
    q->qh %= D10_Q_CAP;
    q->size -= 1;
    return e;
}

void print_bits(const uint64_t a) {
    for (int sh = 63; sh > -1; --sh) {
        printf("%" PRIu64, ((1 << sh) & a) >> sh);
    }
    printf("\n");
}

size_t solve_diagram(qframe *qdat, uint64_t light_bits, uint64_t *btns,
                     size_t btc) {
    size_t presses = 0;
    // static qframe qdat[D10_Q_CAP];
    d10queue queue = {.data = &qdat[0], .size = 0, .qh = 0, .qb = 0};
    qpush(&queue, (qframe){.data = 0LL, .gen = 0});
    while (queue.size != 0) {
        qframe curr = qpop(&queue);
        if (curr.gen > presses) {
            presses = curr.gen;
        }
        if (curr.data == light_bits) {
            break;
        }
        for (size_t bidx = 0; bidx < btc; ++bidx) {
            qpush(&queue, (qframe){.data = curr.data ^ btns[bidx],
                                   .gen = curr.gen + 1});
        }
    }
    return presses;
}

// day 10
long long d10_solve_p1(char *input, const size_t input_sz) {
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    // parse [.#] (0,1)...
    // keep a list of uint64_t for the lights
    // each one has a list of uint64_t for the buttons
    uint64_t lds[D10_MAX_LNS];
    size_t btn_cts[D10_MAX_LNS];
    uint64_t *btns =
        (uint64_t *)malloc(sizeof(uint64_t) * D10_MAX_LNS * D10_MAX_LNS);
    while (lines(&split)) {
        StringView ln = {0};
        split_curr(&split, &ln);
        // parse the light diagrams
        Splitter lnsplit = {.buf = ln.buf, .mx = ln.len};
        delim(&lnsplit, ' ');
        StringView lights = {0};
        split_curr(&lnsplit, &lights);
        lights.buf += 1;
        lights.len -= 2;
        uint64_t light_bits = 0;
        size_t l_idx = 0;
        while (l_idx != lights.len) {
            if (lights.buf[l_idx] == '#') {
                light_bits |= 1;
            }
            if (l_idx < lights.len - 1)
                light_bits <<= 1;
            ++l_idx;
        }
        lds[lns_ct] = light_bits;

        // parse each button
        delim(&lnsplit, '{');
        StringView btns_str = {0};
        split_curr(&lnsplit, &btns_str);
        btns_str.buf += 1;
        btns_str.len -= 2; // remove character fom each side
        Splitter btnsplit = {.buf = btns_str.buf, .mx = btns_str.len};
        size_t btc = 0;
        while (delim(&btnsplit, ')')) {
            StringView sv = {0};
            split_curr(&btnsplit, &sv);
            sv_trim_whitespace(&sv);
            sv.buf += 1;
            sv.len -= 1; // remove first paren

            // each index in the button
            // we have l_idx which is the number of lights_bits
            Splitter comsplit = {.buf = sv.buf, .mx = sv.len};
            uint64_t btn = 0;
            while (delim(&comsplit, ',')) {
                StringView bidx = {0};
                split_curr(&comsplit, &bidx);
                long long b = sv_atoll(&bidx);
                btn |= 1 << ((l_idx - 1) - b);
            }
            btns[lns_ct * D10_MAX_LNS + btc] = btn;
            ++btc;
        }
        btn_cts[lns_ct] = btc;
        lns_ct++;
    }

    qframe *qdat = (qframe *)malloc(sizeof(qframe) * D10_Q_CAP);
    long long total = 0;
    for (size_t idx = 0; idx < lns_ct; ++idx) {
        total += solve_diagram(qdat, lds[idx], &btns[idx * D10_MAX_LNS],
                               btn_cts[idx]);
    }
    free(qdat);
    free(btns);
    return total;
}

#define D10_MAX_JOLTAGES 20
#define D10_MAX_BTNS 20

static inline int d10_fast_gcd(int a, int b) {
    assert(a > 0 && b > 0 && "a and b must be positive integers");
    int d = 0;
    while ((a & 1) == 0 && (b & 1) == 0) {
        a >>= 1;
        b >>= 1;
        ++d;
    }
    while ((a & 1) == 0) {
        a >>= 1;
    }
    while ((b & 1) == 0) {
        b >>= 1;
    }
    while (a != b) {
        if (a > b) {
            a = a - b;
            do {
                a >>= 1;
            } while ((a & 1) == 0);
        } else {
            b = b - a;
            do {
                b >>= 1;
            } while ((b & 1) == 0);
        }
    }
    assert(a == b && "a does not equal b??? how are we here");

    return (1 << d) * a;
}

static inline int d10_gcd(int m, int n) {
    if (n == 0)
        return m;
    if (m == 0)
        return n;
    return d10_fast_gcd((m < 0 ? m * -1 : m), (n < 0 ? n * -1 : n));
}

static inline int d10_gcd_row(const int *const row, size_t len) {
    int prev = row[0];
    for (size_t i = 1; i < len; ++i) {
        prev = d10_gcd(prev, row[i]);
    }
    return prev;
}

void reduce(int *matrix, const size_t n, const size_t m) {
    for (size_t ridx = 0; ridx < n; ++ridx) {
        // find the first non-zero element in the row
        size_t pivot = 0;
        while (pivot < m && matrix[m * ridx + pivot] == 0)
            ++pivot;
        if (pivot < m - 1) {
            // printf("found pivot: %zu, %zu\n", ridx, pivot);
            // if we found it, we need to zero out this idx in all below rows
            for (size_t trow = ridx + 1; trow < n; ++trow) {
                if (matrix[m * trow + pivot] != 0) {
                    // printf("Applying row %zu - row %zu\n", trow, ridx);

                    int a = matrix[m * ridx + pivot],
                        b = matrix[m * trow + pivot];

                    for (size_t cidx = 0; cidx < m; ++cidx) {
                        int c = matrix[m * ridx + cidx],
                            d = matrix[m * trow + cidx];
                        matrix[m * trow + cidx] = (b * c) - (a * d);
                    }
                }
            }
            int row_gcd = d10_gcd_row(&matrix[ridx * m], m);
            // printf("row %zu gcd: %d, last: %d\n", ridx, row_gcd, matrix[ridx
            // * m + (m - 1)]);
            if (row_gcd != 0) {
                if (matrix[ridx * m + (m - 1)] < 0) {
                    row_gcd *= -1;
                }
                for (size_t j = 0; j < m; ++j) {
                    matrix[ridx * m + j] /= row_gcd;
                }
            }
        }
    }
}

void print_matrix(const int *matrix, const size_t n, const size_t m) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            printf("[%d]\t", matrix[i * m + j]);
        }
        printf("\n");
    }
}

void calc_bounds(const int *matrix, int *bounds, const size_t n,
                 const size_t m) {
    for (size_t i = 0; i < n; ++i) {
        // this row indicates a boundaries if none are negative
        int bounded = 1;
        for (size_t j = 0; j < m - 1; ++j) {
            bounded &= matrix[i * m + j] > -1;
        }
        if (bounded) {
            // if there is a non-zero value in the row, that index is bounded by
            // the last value if matrix[i, j] > 0 && (bounds[j] < 0 || bounds[j]
            // < matrix[i, last]) printf("row %zu bounds the value\n", i);
            for (size_t j = 0; j < m - 1; ++j) {
                // printf("bounds[%d] = {%d}, matrix[%d, %d] = {%d}\n", j,
                // bounds[j], i, j, matrix[i * m + j]);
                if (matrix[i * m + j] > 0 &&
                    (bounds[j] == 0 || bounds[j] > (matrix[i * m + (m - 1)] /
                                                    matrix[i * m + j]))) {
                    // printf("%d, %d is bounded by %d\n", i, j, matrix[i * m +
                    // (m - 1)]);
                    bounds[j] = matrix[i * m + (m - 1)] / matrix[i * m + j];
                }
            }
        }
    }
    for (size_t j = 0; j < m - 1; ++j) {
        if (bounds[j] > -1) {
            bounds[m - 1] += bounds[j];
        }
    }
}

int check_vals(const int *matrix, const int *v, const size_t n,
               const size_t m) {
    int valid = 1;
    for (size_t i = 0; i < n; ++i) {
        int res = 0;
        for (size_t j = 0; j < m - 1; ++j) {
            // printf("[%d]\t", matrix[i * m + j]);
            res += v[j] * matrix[i * m + j];
        }
        // printf("[%d] -> [%d] (diff = %d)\n", matrix[i * m + (m - 1)], res,
        //        res - matrix[i * m + (m - 1)]);
        if (matrix[i * m + (m - 1)] != res) {
            valid = 0;
        }
    }
    return valid;
}

// we need an arena to bfs the vectors
#define C_CAP 16
#define V_Q_CAP (C_CAP * (2 << 25))

typedef struct {
    int *data;
    size_t qh, qb, size;
} bumper;

int *push_alloc(bumper *q) {
    if (q->size == (V_Q_CAP / C_CAP)) {
        fprintf(stderr, "Could not push to queue, too many elements %zu > %d\n",
                q->size, (V_Q_CAP / C_CAP));
        exit(1);
    }
    int *ptr = &q->data[q->qb];
    q->qb += C_CAP;
    q->qb %= V_Q_CAP;
    q->size += 1;
    return ptr;
}

int *pop_free(bumper *q) {
    if (q->size == 0) {
        fprintf(stderr, "Could not pop from an empty queue\n");
        exit(1);
    }
    q->qh += C_CAP;
    q->qh %= V_Q_CAP;
    q->size -= 1;
    return &q->data[q->qh];
}

int fill_values(const int *matrix, int *curr, const size_t n, const size_t m) {
    for (size_t r = 0; r < n; ++r) {
        int sum_known = 0;
        size_t unknown_ct = 0;
        size_t unknown_idx = 0;
        for (size_t c = 0; c < m - 1; ++c) {
            if (curr[c] != -1) {
                sum_known += curr[c] * matrix[r * m + c];
            } else if (matrix[r * m + c] != 0) {
                unknown_ct++;
                unknown_idx = c;
            }
        }

        if (unknown_ct == 0) {
            if (sum_known != matrix[r * m + (m - 1)]) {
                // we have encountered and invalid solution
                return 0;
            }
        } else if (unknown_ct == 1) {
            int diff = matrix[r * m + (m - 1)] - sum_known;
            if (diff % matrix[r * m + unknown_idx] != 0) {
                // invalid solution
                return 0;
            }
            int value = diff / matrix[r * m + unknown_idx];
            if (value < 0) {
                // invalid solution
                return 0;
            }
            curr[unknown_idx] = value;
            curr[m - 1] += value;
        }
    }
    return 1;
}

#define M_MAX 20

long long d10_p2_solve_matrix(int *matrix, const size_t n, const size_t m,
                              int *const qdat) {
    int bounds[M_MAX] = {0};
    calc_bounds(matrix, bounds, n, m);

    reduce(matrix, n, m);

    calc_bounds(matrix, bounds, n, m);

    int min = 0, max = 0;
    for (size_t i = 0; i < m - 1; ++i) {
        max += bounds[i];
    }
    min = max;

    bumper q = {.data = qdat, .qh = 0, .qb = 0, .size = 0};
    int *curr = push_alloc(&q);
    for (size_t x = 0; x < m - 1; ++x) {
        curr[x] = -1;
    }
    curr[m - 1] = 0;

    while (q.size != 0) {
        // populate known values - fill in everything that fits
        // sum the known value * row value
        // counting the number of unknowns
        // noting the index for each row (if row[i] != 0
        // if the unknown count == 0, we have none
        // if the unknown count == 1, check the (answer - sum known) % row value
        // = 0.
        //          if so, set that value to (answer - sum known) / row value.
        //
        // if there are no more unknowns, check_vals
        int filled = fill_values(matrix, curr, n, m);
        // printf("able to fill values: %d\n", filled);
        if (filled) {
            int unknowns = 0;
            for (size_t i = 0; i < m - 1; ++i) {
                if (curr[i] == -1)
                    unknowns += 1;
                // printf("[%d]\t", curr[i]);
            }
            // printf("[%d] unknowns: %d\n", curr[m-1], unknowns);
            if (unknowns == 0) {
                if (check_vals(matrix, curr, n, m) && min > curr[m - 1]) {
                    min = curr[m - 1];
                }
            } else {
                // find the first column to show up in row with least unknowns
                int selected_idx = -1;
                size_t selected_unknown = m;
                for (size_t i = 0; i < n; ++i) {
                    // how many unknowns
                    size_t unknown_ct = 0;
                    size_t unknown_idx = 0;
                    for (size_t j = 0; j < m - 1; ++j) {
                        if (curr[j] == -1 && matrix[i * m + j] != 0) {
                            unknown_ct++;
                            unknown_idx = j;
                        }
                    }

                    if (unknown_ct > 0 && unknown_ct < selected_unknown) {
                        selected_idx = unknown_idx;
                        selected_unknown = unknown_ct;
                    }
                }

                assert(selected_idx != -1 &&
                       "Selected index should not be negative");

                for (int i = 0; i <= bounds[selected_idx]; ++i) {
                    if (curr[m - 1] + i <= bounds[m - 1] && curr[m - 1] < min) {
                        int *child = push_alloc(&q);
                        for (size_t j = 0; j < m; ++j) {
                            child[j] = curr[j];
                        }
                        child[selected_idx] = i;
                        child[m - 1] += i;
                    }
                }
            }
        }
        curr = pop_free(&q);
    }

    // printf("min: %d\n", min);
    return min;
}

long long d10_solve_p2(char *input, const size_t input_sz) {
    StringView lns = {0};
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};

    uint64_t levels[D10_MAX_JOLTAGES];
    size_t lvl_ct = 0;

    int btns[D10_MAX_BTNS][D10_MAX_JOLTAGES] = {0};

    int *qdat = (int *)malloc(sizeof(int) * V_Q_CAP);

    long long total = 0;
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D10_MAX_LNS);
        split_curr(&split, &lns);
        Splitter lnsplit = {.buf = lns.buf, .mx = lns.len};

        delim(&lnsplit, ' '); // skip over the lights

        // parse each button
        delim(&lnsplit, '{');
        StringView btns_str = {0};
        split_curr(&lnsplit, &btns_str);
        btns_str.buf += 1;
        btns_str.len -= 2; // remove character fom each side
        Splitter btnsplit = {.buf = btns_str.buf, .mx = btns_str.len};
        size_t btc = 0;
        while (delim(&btnsplit, ')')) {
            StringView sv = {0};
            split_curr(&btnsplit, &sv);
            sv_trim_whitespace(&sv);
            sv.buf += 1;
            sv.len -= 1; // remove first paren

            // each index in the button
            // we have l_idx which is the number of lights_bits
            Splitter comsplit = {.buf = sv.buf, .mx = sv.len};
            while (delim(&comsplit, ',')) {
                StringView bidx = {0};
                split_curr(&comsplit, &bidx);
                int b = sv_atoi(&bidx);
                btns[btc][b] = 1;
            }
            btc++;
        }
        delim(&lnsplit, '}');
        StringView jolts_str = {0};
        split_curr(&lnsplit, &jolts_str);
        jolts_str.buf += 1;
        jolts_str.len -= 1;
        Splitter jsplitter = {.buf = jolts_str.buf, .mx = jolts_str.len};
        while (delim(&jsplitter, ',')) {
            StringView lvl_str = {0};
            split_curr(&jsplitter, &lvl_str);
            int lvl = sv_atoi(&lvl_str);
            levels[lvl_ct++] = lvl;
        }

        // create the augmented matrix
        int aug_matrix[D10_MAX_JOLTAGES * D10_MAX_BTNS + 1] = {0};
        size_t y = lvl_ct, x = btc + 1;
        for (size_t jdx = 0; jdx < lvl_ct; ++jdx) {
            for (size_t idx = 0; idx < btc; ++idx) {
                aug_matrix[jdx * x + idx] = btns[idx][jdx];
                // printf("[%d]\t", aug_matrix[jdx * x + idx]);
            }
            aug_matrix[jdx * x + btc] = levels[jdx];
            // printf("[%d]\n", aug_matrix[jdx * x + btc]);
        }
        total += d10_p2_solve_matrix(aug_matrix, y, x, qdat);

        // reset the buffers
        lvl_ct = 0;
        for (size_t j = 0; j < D10_MAX_JOLTAGES; ++j) {
            for (size_t i = 0; i < D10_MAX_JOLTAGES; ++i) {
                btns[j][i] = 0;
            }
        }

        lns_ct++;
    }
    free(qdat);
    return total;
}

void solve_day10(const char *in_file) {
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d10_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d10_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day10_part1) {
    char *input_str =
        "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
        "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char *)&input, &input_len);
    long long result = d10_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 7)
}

TEST(test_day10_part2) {
    char *input_str =
        "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
        "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
        "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char *)&input, &input_len);
    long long result = d10_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 33)
}

void day10_tests() {
    RUN_TEST(test_day10_part1);
    RUN_TEST(test_day10_part2);
}

#endif
