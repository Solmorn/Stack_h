#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


Stack_Err_t StackCtor(Stack_t* stk, size_t capacity_got, BirthInfo* info_got) {

    if (capacity_got < 128) capacity_got = 128;

    stk->data = (int*)calloc(capacity_got, sizeof(int));
    stk->size = 0;
    stk->capacity = capacity_got;
    stk->info = info_got;

    FillPoison(stk);

    return Ok;
}

Stack_Err_t StackPush(Stack_t* stk, int element) {

    if (stk->size == stk->capacity) {
        stk->capacity *= 2;
        stk->data = (int*)realloc(stk->data, stk->capacity);
    }

    stk->data[(stk->size)++] = element;

    return Ok;

}

Stack_Err_t StackPop(Stack_t* stk) {

    if (stk->size == 0) return Error;

    if (stk->size * 4 < stk->capacity) {
        stk->capacity /= 2;
        stk->data = (int*)realloc(stk->data, stk->capacity);
    }

    stk->data[(stk->size)-- - 1] = stk->poison;

    return Ok;

}

Stack_Err_t StkErr(Stack_t* stk) {

    return Ok;

}

void StkDump(Stack_t* stk) {
    BirthInfo* info_got = stk->info;
    printf("=====INIT_INFO=====\nFILE: %s /-----/ FUCK: %s /-----/ LINE: %d /-----/ NAME: %s\n\n",
                                info_got->file, info_got->func, info_got->line, info_got->name);

    printf("StkDump(%s[%p]) {\n", info_got->name, &stk);
    printf("    size     = %d\n", stk->size);
    printf("    capacity = %d\n", stk->capacity);
    printf("    poison   = %d\n", stk->poison);
    printf("    data[%p] {\n", stk->data);

    for (size_t index = 0; index < stk->capacity; index++) {

        char* is_poison = "";
        char* is_filled = "*";
        if (index >= stk->size) is_filled = " ";

        int element = (stk->data)[index];
        if (element == stk->poison) is_poison = "(poison)";

         printf("        %s [%u] = %d %s\n", is_filled, index, element, is_poison);
    }

    printf("    }\n}\n\n");
}

void StkDtor(Stack_t* stk) {

    free(stk);

}

void FillPoison(Stack_t* stk) {

    for (size_t index = 0; index < stk->capacity; index++) {
        (stk->data)[index] = stk->poison;
    }

}
