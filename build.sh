set -e

if [ ! -d "./build" ]; then
    mkdir -p ./build
fi

gcc -c ./src/lib/aoc.c -o ./build/aoc.o
ar rcs ./build/libaoc.a ./build/aoc.o

gcc ./src/*.c -DTESTING -L./build -laoc -o ./build/test
gcc ./src/*.c -L./build -laoc -O3 -o ./build/main
