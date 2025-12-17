set -e
gcc -c ./src/lib/aoc.c -o ./build/aoc.o
ar rcs ./build/libaoc.a ./build/aoc.o
gcc ./src/test.c -L./build -laoc -o ./build/test
gcc ./src/main.c -L./build -laoc -o ./build/main
