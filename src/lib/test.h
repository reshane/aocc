// test helpers
#define TEST(name) void name()
#define RUN_TEST(name) printf("\033[1m%s\n\033[0m", #name); name()
#define ASSERT(expr) if (!(expr)) { \
    printf("\033[0;31mFAIL: %s\n\033[0m\n", #expr); \
} else { \
    printf("\033[0;32mPASS: %s\n\033[0m\n", #expr); \
}
#define ASSERT_STR_EQ(str1, str2) ASSERT(strcmp(str1, str2) == 0)
#define EPSILON 0.00001
#define ASSERT_F_EQ(f1, f2) if ((f1-f2)<0) { \
    ASSERT((f1-f2)>(-1*EPSILON)); \
} else { \
    ASSERT((f1-f2)<(EPSILON)); \
}
