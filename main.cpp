#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack.h"

int main() {
    StackInfo stk1 = {};
    INIT_STACK(stk1, 10);

    printf("\n");
    StackPush(&stk1, 12);
    StackPush(&stk1, 5);
    StackPush(&stk1, -1);
    StackPush(&stk1, 2);

    switch (2) {
        case 1:
            stk1.data[3] = 33;

            break;
        case 2:
            stk1.size = -22;
            stk1.capacity = -15;
            break;
        case 3:
            stk1.size = 100;
            break;
        case 4:
            stk1.data[2] = 0x00000D1127;
            break;
        default:
            break;
    }



    error_code err = StackPush(&stk1, 999);

    printf("%d\n", err);

    StkDtor(&stk1);
    printf("RRERER");
}

