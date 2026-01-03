set -e

if [ ! -d "./build" ]; then
    mkdir -p ./build
fi

gcc -c ./src/lib/aoc.c -o ./build/aoc.o -Wall -Wextra
ar rcs ./build/libaoc.a ./build/aoc.o

gcc ./src/*.c -Wall -Wextra -DTESTING -L./build -laoc -o ./build/test
gcc ./src/*.c -Wall -Wextra -L./build -laoc -O3 -o ./build/main
