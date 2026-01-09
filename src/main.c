
#include "days.h"
#include "lib/aoc.h"

const char* in_files[12] = {
    "./inputs/01.txt", "./inputs/02.txt", "./inputs/03.txt", "./inputs/04.txt",
    "./inputs/05.txt", "./inputs/06.txt", "./inputs/07.txt", "./inputs/08.txt",
    "./inputs/09.txt", "./inputs/10.txt", "./inputs/11.txt", "./inputs/12.txt",
};
void (*solutions[12])(const char*) = {
    solve_day1, solve_day2, solve_day3, solve_day4,  solve_day5,  solve_day6,
    solve_day7, solve_day8, solve_day9, solve_day10, solve_day11, solve_day12,
};

#ifdef TESTING
void (*tests[12])() = {
    day01_tests, day02_tests, day03_tests, day04_tests,
    day05_tests, day06_tests, day07_tests, day08_tests,
    day09_tests, day10_tests, day11_tests, day12_tests,
};

int main(int argc, char** argv) {
    int last_day = sizeof(tests) / sizeof(tests[0]);
    if (argc > 2) {
        printf("Usage: %s [day ...]\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        int day = atoi(argv[argc - 1]);
        if (day > last_day) {
            printf("No solution for day %d\n", day);
            return 0;
        }
        printf("\033[0;35mRunning tests for day %d:\033[0m\n", day);
        tests[day - 1]();
    } else {
        printf("\033[0;35mRunning tests for all days:\033[0m\n");
        run_tests();
    }
}

#else

int main(int argc, char** argv) {
    int last_day = sizeof(solutions) / sizeof(solutions[0]);
    int day = last_day;
    if (argc > 2) {
        printf("Usage: %s [day ...]\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        day = atoi(argv[argc - 1]);
        if (day > last_day) {
            printf("No solution for day %d\n", day);
            return 0;
        }
    }
    printf("\033[0;35mDay %d: %s\033[0m\n", day, in_files[day - 1]);
    solutions[day - 1](in_files[day - 1]);
    return 0;
}

#endif
