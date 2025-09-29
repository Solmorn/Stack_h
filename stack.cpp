#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack.h"


Stack_Err_t StackCtor(StackInfo* stk, size_t capacity_got, BirthInfo* info_got) {

    assert(stk);
    #ifdef _DEBUG//
    assert(info_got);
    #endif

    if (capacity_got < 0) return CapacityError;
    if (capacity_got < 10) capacity_got = 10;

    stack_type* calloc_ptr = (stack_type*)calloc(capacity_got + 2, sizeof(stack_type));
    if (calloc_ptr == nullptr) return AllocationError;

    stk->data = calloc_ptr + 1;
    (stk->canary_p).canary1 = calloc_ptr;
    (stk->canary_p).canary2 = calloc_ptr + capacity_got + 1;

    *((stk->canary_p).canary1) = stk->canary_v;
    *((stk->canary_p).canary2) = stk->canary_v;

    stk->size = 0;
    stk->capacity = capacity_got;
    stk->info = info_got;

    FillPoison(stk);

    return Ok;
}

Stack_Err_t StackPush(StackInfo* stk, stack_type element) {

    #ifdef _DEBUG
    ASSERT_OK(stk);
    #endif

    if (stk->size == stk->capacity) {

        stk->capacity *= 2;

        stack_type* realloc_ptr = (stack_type*)realloc((stk->canary_p).canary1, (stk->capacity + 2) * sizeof(stack_type));
        if (realloc_ptr == nullptr) return AllocationError;

        stk->data = realloc_ptr + 1;
        (stk->canary_p).canary1 = realloc_ptr;
        (stk->canary_p).canary2 = realloc_ptr + stk->capacity + 1;
    }

    FillPoison(stk);
    stk->data[(stk->size)++] = element;

    return Ok;

}

Stack_Err_t StackPop(StackInfo* stk) {

    #ifdef _DEBUG
    ASSERT_OK(stk);
    #endif

    if (stk->size == 0) return PopSizeStackError;

    stk->data[(stk->size)-- - 1] = stk->poison;

    return Ok;

}

Stack_Err_t StkErr(StackInfo* stk) {

    assert(stk);

    return Ok;

}

void StkDump(StackInfo* stk) {

    assert(stk);

    BirthInfo* info_got = stk->info;
    printf("=====INIT_INFO=====\nFILE: %s /-----/ FUCK: %s /-----/ LINE: %d /-----/ NAME: %s\n\n",
                                info_got->file, info_got->func, info_got->line, info_got->name);

    printf("StkDump(%s[%p]) {\n", info_got->name, &stk);
    printf("    size     = %d\n", stk->size);
    printf("    capacity = %d\n", stk->capacity);
    printf("    poison   = %d\n", stk->poison);
    printf("    canary   = %x\n", stk->canary_v);
    printf("    data[%p] {\n", stk->data);
    printf("         [canary] = %x\n", *((stk->canary_p).canary1));

    for (size_t index = 0; index < stk->capacity; index++) {

        const char* is_poison = "";
        const char* is_filled = "*";

        if (index >= stk->size) is_filled = " ";

        stack_type element = (stk->data)[index];
        if (element == stk->poison) is_poison = "(poison)";

         printf("        %s [%u] = %d %s\n", is_filled, index, element, is_poison);
    }

    printf("         [canary] = %x\n", *((stk->canary_p).canary2));

    printf("    }\n}\n\n");
}

void StkDtor(StackInfo* stk) {

    #ifdef _DEBUG
    ASSERT_OK(stk);
    #endif

    free((stk->canary_p).canary1);

}

void FillPoison(StackInfo* stk) {

    #ifdef _DEBUG
    ASSERT_OK(stk);
    #endif

    for (size_t index = stk->size; index < stk->capacity; index++) {
        (stk->data)[index] = stk->poison;
    }

}

uint64_t CountHash(StackInfo* stk) {
    uint64_t hash =

}
