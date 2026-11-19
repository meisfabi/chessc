#include <stdio.h>
#include "board.h"

int main(void) {
    Position position;

    buildBoard(&position);
    printBoard(&position);

    return 0;
}