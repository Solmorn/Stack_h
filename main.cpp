#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack.h"

int main() {
    StackInfo stk1 = {};
    INIT_STACK(stk1, 10);
    StkDump(&stk1);
    StackPush(&stk1, 12);
    StackPush(&stk1, 5);
    StackPush(&stk1, -1);
    StackPush(&stk1, 2);

    switch (4) {
        case 1:
            stk1.data[3] = 33;
            *(stk1.data_canary_p.canary1) = 1039481;
            break;
        case 2:
            stk1.size = -22;
            stk1.capacity = -15;
            break;
        case 3:
            stk1.size = 100;
            break;
        case 4:
            stk1.data[2] = stk1.poison;
            break;
        default:
            break;
    }



    int a = 0;
    StackPush(&stk1, 999);
    StkDump(&stk1);

    StkDtor(&stk1);
}

