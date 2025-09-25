#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack.h"

int main() {
    Stack_t stk1 = {};
    INIT_STACK(stk1, 10);

    StkDump(&stk1);

    StackPush(&stk1, 12);
    StackPush(&stk1, 5);
    StackPush(&stk1, -1);
    StackPush(&stk1, 2);

    StkDump(&stk1);

    StackPop(&stk1);

    StkDump(&stk1);

    StkDtor(&stk1);
}

