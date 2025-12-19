
#include "days.h"

#ifdef TESTING

int main()
{
    day01_tests();
    day02_tests();
}

#else

#include "lib/aoc.h"

const char* in_files[2] = {
    "./inputs/01.txt",
    "./inputs/02.txt"
};
void (*solutions[2])(const char*) = {
    solve_day1,
    solve_day2
};


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
    printf("Day %d\n", day);
    solutions[day-1](in_files[day-1]);
    return 0;
}

#endif
