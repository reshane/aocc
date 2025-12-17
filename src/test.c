#include <string.h>

// tests

#include "lib/aoc.h"

// test helpers
int failed = 0;
#define TEST(name) void name()
#define RUN_TEST(name) printf("\n\033[1m%s\n\033[0m", #name); name()
#define ASSERT(expr) if (!(expr)) { \
    failed = 1; \
    printf("\033[0;31mFAIL: %s\n\033[0m", #expr); \
} else { \
    printf("\033[0;32mPASS: %s\n\033[0m", #expr); \
}
#define ASSERT_STR_EQ(str1, str2) ASSERT(strcmp(str1, str2) == 0)
#define EPSILON 0.00001
#define ASSERT_F_EQ(f1, f2) if ((f1-f2)<0) { \
    ASSERT((f1-f2)>(-1*EPSILON)); \
} else { \
    ASSERT((f1-f2)<(EPSILON)); \
}

TEST(test_Vector_i) {
    Vector_i* my_vec = Vector_i_new();
    Vector_i_push(my_vec, 3);
    ASSERT(Vector_i_get(my_vec, 0) == 3);
    Vector_i_free(my_vec);
}

int main() {
    RUN_TEST(test_Vector_i);
    return 0;
}
