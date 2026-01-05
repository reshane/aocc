#include "lib/aoc.h"

#define D9_MAX_LNS 1024

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

long long area(const Point *const p1, const Point *const p2)
{
    long long x1 = MIN(p1->x, p2->x);
    long long x2 = MAX(p1->x, p2->x);
    long long y1 = MIN(p1->y, p2->y);
    long long y2 = MAX(p1->y, p2->y);
    long long dx = x2-x1+1;
    long long dy = y2-y1+1;
    return dx*dy;
}

// day 9
long long d9_solve_p1(char *input, const size_t input_sz)
{
    static StringView lns[D9_MAX_LNS];
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    Point pts[D9_MAX_LNS];
    long long max = 0;
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D9_MAX_LNS);
        split_curr(&split, &lns[lns_ct]);
        // split each by comma
        Splitter ln_split = {.buf = lns[lns_ct].buf, .mx = lns[lns_ct].len};
        StringView sv = {0};
        delim(&ln_split, ',');
        split_curr(&ln_split, &sv);
        long long x = sv_atoll(&sv);
        delim(&ln_split, ',');
        split_curr(&ln_split, &sv);
        long long y = sv_atoll(&sv);
        pts[lns_ct] = (Point){.x=x, .y=y};
        for (size_t idx = 0; idx < lns_ct; ++idx) {
            long long t_area = area(&pts[idx], &pts[lns_ct]);
            if (t_area > max) {
                max = t_area;
            }
        }
        lns_ct++;
    }
    long long total = max;
    return total;
}

long long comp_ll(long long a, long long b)
{
    long long d = a-b;
    if (a < b) {
        return 1;
    } else if (a > b) {
        return -1;
    }
    return d;
}

// a and b are indices into the pts array
// w is the area they create
typedef struct p9e {
    size_t a,b;
    long long w;
} p9e;

int comp_area(const void *a, const void *b)
{
    p9e ar1 = *(p9e*)a;
    p9e ar2 = *(p9e*)b;
    return comp_ll(ar1.w, ar2.w);
}

long long d9_solve_p2(char *input, const size_t input_sz)
{
    static StringView lns[D9_MAX_LNS];
    size_t lns_ct = 0;
    Splitter split = {.buf = input, .mx = input_sz};
    static Point pts[D9_MAX_LNS];
    static Point perim[D9_MAX_LNS*D9_MAX_LNS];
    size_t perimct = 0;
    static p9e areas[D9_MAX_LNS*D9_MAX_LNS];
    size_t areas_ct = 0;
    while (lines(&split)) {
        assert("lns ran out of space!" && lns_ct < D9_MAX_LNS);
        split_curr(&split, &lns[lns_ct]);
        // split each by comma
        Splitter ln_split = {.buf = lns[lns_ct].buf, .mx = lns[lns_ct].len};
        StringView sv = {0};
        delim(&ln_split, ',');
        split_curr(&ln_split, &sv);
        long long x = sv_atoll(&sv);
        delim(&ln_split, ',');
        split_curr(&ln_split, &sv);
        long long y = sv_atoll(&sv);
        pts[lns_ct] = (Point){.x=x, .y=y};
        for (size_t idx = 0; idx < lns_ct; ++idx) {
            areas[areas_ct++] = (p9e){.a=idx, .b=lns_ct, .w=area(&pts[idx], &pts[lns_ct])};
        }
        lns_ct++;
    }
    for (size_t idx = 0; idx < lns_ct; ++idx) {
        Point curr = pts[(idx+1)%lns_ct];
        Point prev = pts[idx];
        Point dir = (Point){.x=comp_ll(prev.x,curr.x), .y=comp_ll(prev.y,curr.y)};
        do {
            // update, add
            prev.x += dir.x;
            prev.y += dir.y;
            assert("perim has run out of space!" && perimct < D9_MAX_LNS*D9_MAX_LNS);
            perim[perimct++] = prev;
        } while (prev.x != curr.x || prev.y != curr.y);
    }

    qsort((void*)&areas, areas_ct, sizeof areas[0], &comp_area);

    for (size_t idx = 0; idx < areas_ct; ++idx) {
        p9e t = areas[idx];
        int valid = 1;
        for (size_t p = 0; p < perimct; ++p) {
            long long x1 = MIN(pts[t.a].x, pts[t.b].x);
            long long x2 = MAX(pts[t.a].x, pts[t.b].x);
            long long y1 = MIN(pts[t.a].y, pts[t.b].y);
            long long y2 = MAX(pts[t.a].y, pts[t.b].y);
            if (perim[p].x > x1 && perim[p].x < x2 && perim[p].y > y1 && perim[p].y < y2) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            return t.w;
        }
    }
    return -1;
}

void solve_day9(const char *in_file)
{
    char input[BUF_CAP];
    size_t input_sz;
    if (aoc_slurp_file(in_file, input, &input_sz) != 0) {
        return;
    }
    printf("\033[1mPart 1: %lld\n\033[0m", d9_solve_p1(input, input_sz));
    printf("\033[1mPart 2: %lld\n\033[0m", d9_solve_p2(input, input_sz));
}

#ifdef TESTING
#include "lib/test.h"

TEST(test_day9_part1) {
    char *input_str = 
        "7,1\n"
        "11,1\n"
        "11,7\n"
        "9,7\n"
        "9,5\n"
        "2,5\n"
        "2,3\n"
        "7,3\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d9_solve_p1(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 50)
}

TEST(test_day9_part2) {
    char *input_str = 
        "7,1\n"
        "11,1\n"
        "11,7\n"
        "9,7\n"
        "9,5\n"
        "2,5\n"
        "2,3\n"
        "7,3\n";
    char input[512];
    size_t input_len = 0;
    aoc_fill_input(input_str, (char*)&input, &input_len);
    long long result = d9_solve_p2(input, input_len);
    // printf("%lld\n", result);
    ASSERT(result == 24)
}

void day09_tests()
{
    RUN_TEST(test_day9_part1);
    RUN_TEST(test_day9_part2);
}

#endif
