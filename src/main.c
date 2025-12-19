
#include "lib/aoc.h"
#include "days.h"

const char* in_files[3] = {
    "./inputs/01.txt",
    "./inputs/02.txt",
    "./inputs/03.txt"
};
void (*solutions[3])(const char*) = {
    solve_day1,
    solve_day2,
    solve_day3
};

#ifdef TESTING
void (*tests[3])() = {
    day01_tests,
    day02_tests,
    day03_tests,
};

int main(int argc, char** argv)
{
    int last_day = sizeof(tests) / sizeof(tests[0]);
    if (argc > 2) {
        printf("Usage: %s [day ...]\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        int day = atoi(argv[argc-1]);
        if (day > last_day) {
            printf("No solution for day %d\n", day);
            return 0;
        }
        printf("\033[0;35mRunning tests for day %d:\033[0m\n", day);
        tests[day-1]();
    } else {
        printf("\033[0;35mRunning tests for all days:\033[0m\n");
        run_tests();
    }
}

#else

int main(int argc, char** argv)
{
    int last_day = sizeof(solutions) / sizeof(solutions[0]);
    int day = last_day;
    if (argc > 2) {
        printf("Usage: %s [day ...]\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        day = atoi(argv[argc-1]);
        if (day > last_day) {
            printf("No solution for day %d\n", day);
            return 0;
        }
    }
    printf("\033[0;35mDay %d: %s\033[0m\n", day, in_files[day-1]);
    solutions[day-1](in_files[day-1]);
    return 0;
}

#endif
