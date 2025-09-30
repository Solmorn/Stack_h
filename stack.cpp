#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack.h"




error_code StackCtor(StackInfo* stk, size_t capacity_got, ...) {

    va_list args;
    va_start(args, capacity_got);


    if (capacity_got > MAX_SIZE_VALUE) return CapacityError;
    if (capacity_got <  10) capacity_got = 10;//

    #ifdef _DEBUG

    BirthInfo* info_got = va_arg(args, BirthInfo*);

    assert(stk);
    assert(info_got);

    stk->info = info_got;

    #ifdef CANARY_ON

    stack_type* calloc_ptr = (stack_type*)calloc(capacity_got + 2, sizeof(stack_type));
    if (calloc_ptr == nullptr) return AllocationError;

    stk->data = calloc_ptr + 1;
    (stk->data_canary_p).canary1 = calloc_ptr;
    (stk->data_canary_p).canary2 = calloc_ptr + capacity_got + 1;

    *((stk->data_canary_p).canary1) = stk->data_canary_v;
    *((stk->data_canary_p).canary2) = stk->data_canary_v;

    #else //canary

    stack_type* calloc_ptr = (stack_type*)calloc(capacity_got, sizeof(stack_type));
    if (calloc_ptr == nullptr) return AllocationError;

    stk->data = calloc_ptr;

    #endif //canary

    va_end(args);

    #else //debug

    stack_type* calloc_ptr = (stack_type*)calloc(capacity_got, sizeof(stack_type));
    if (calloc_ptr == nullptr) return AllocationError;

    stk->data = calloc_ptr;

    #endif //debug

    stk->size = 0;
    stk->capacity = capacity_got;

    #ifdef _DEBUG
    FillPoison(stk);
    #endif

    #ifdef HASH_ON
    stk->hash_value = CountHash(stk);
    #endif

    ASSERT_OK(stk);

    return Ok;
}

error_code StackPush(StackInfo* stk, stack_type element) {

    ASSERT_OK(stk);

    if (stk->size == stk->capacity) {

        stk->capacity *= 2;

        #ifdef CANARY_ON

        stack_type* realloc_ptr = (stack_type*)realloc((stk->data_canary_p).canary1, (stk->capacity + 2) * sizeof(stack_type));
        if (realloc_ptr == nullptr) return AllocationError;//

        stk->data = realloc_ptr + 1;
        (stk->data_canary_p).canary1 = realloc_ptr;
        (stk->data_canary_p).canary2 = realloc_ptr + stk->capacity + 1;

        #else

        stack_type* realloc_ptr = (stack_type*)realloc(stk->data, stk->capacity * sizeof(stack_type));
        if (realloc_ptr == nullptr) return AllocationError;

        stk->data = realloc_ptr;

        #endif
    }

    stk->data[(stk->size)++] = element;

    #ifdef _DEBUG
    FillPoison(stk);
    #endif

    #ifdef HASH_ON
    stk->hash_value = CountHash(stk);
    #endif

    ASSERT_OK(stk);


    return Ok;

}

error_code StackPop(StackInfo* stk, stack_type* element) {

    ASSERT_OK(stk);

    if (stk->size == 0) return PopSizeStackError;

    *element = stk->data[(stk->size) - 1];
    stk->data[(stk->size)-- - 1] = stk->poison;

    #ifdef HASH_ON
    stk->hash_value = CountHash(stk);
    #endif

    ASSERT_OK(stk);

    return Ok;

}

void ASSERT_OK(StackInfo* stk) {

    #ifdef _DEBUG
    if (StkErr(stk) != 0) {
        StkDump(stk);
        printf("ASSERTION FAILED");
        abort();
    }
    #endif

}

error_code StkErr(StackInfo* stk) {

    assert(stk);

    error_code code = 0;

    #ifdef CANARY_ON
    if (stk->stack_canary1 != stk->stack_canary_v || stk->stack_canary2 != stk->stack_canary_v) {
                                code |= StackCanaryError;
        stk->errors_bit = code;
        return code;
    }
    #endif

    bool check_data_allowed   = true;
    bool check_poison_allowed = true;

    if (stk->size     > MAX_SIZE_VALUE) {
                                code |= SizeError;
        check_data_allowed   = false;
    }

    if (stk->capacity > MAX_SIZE_VALUE) {
                                code |= CapacityError;
        check_poison_allowed = false;
    }

    if (stk->capacity < stk->size) {
                                code |= SizeMoreThanCapacityError;
        check_poison_allowed = false;
    }

    if (stk->data == nullptr) {//
                                code |= NullptrDataError;
        stk->errors_bit = code;
        return code;
    }

    for (size_t index = 0; check_data_allowed && index < stk->size; index++) {
        if ((stk->data)[index] == stk->poison) {
                                code |= PoisonDataError;
            break;
        }
    }

    for (size_t index = stk->size; check_poison_allowed && index < stk->capacity; index++) {
        if ((stk->data)[index] != stk->poison) {
                                code |= PoisonFillingError;
            break;
        }
    }

    #ifdef CANARY_ON
    if (*((stk->data_canary_p).canary1) != stk->data_canary_v || *((stk->data_canary_p).canary2) != stk->data_canary_v) {
                                code |= DataCanaryError;
    }
    #endif

    #ifdef HASH_ON
    if (check_data_allowed && (stk->hash_value != CountHash(stk))) {
                                code |= HashError;
    }
    #endif

    stk->errors_bit = code;

    return code;
}

void StkDump(StackInfo* stk) {

    assert(stk);

    BirthInfo* info_got = stk->info;
    printf("=====INIT_INFO=====\nFILE: %s /-----/ FUCK: %s /-----/ LINE: %d /-----/ NAME: %s\n\n",
                                info_got->file, info_got->func, info_got->line, info_got->name);

    printf("ERROR_CODE: %d\n", stk->errors_bit);
    printf("StkDump(%s[%p]) {\n", info_got->name, &stk);

    #ifdef CANARY_ON
    printf("    stack_canary1 = %x\t%s\n", stk->stack_canary1, ContainsError(stk->errors_bit, StackCanaryError)       ? "(BAD!)" : "");
    #endif //canary

    printf("    size          = %lu\t%s\n", stk->size,      ContainsError(stk->errors_bit, SizeError) ||
                                                            ContainsError(stk->errors_bit, SizeMoreThanCapacityError)  ? "(BAD!)" : "");
    printf("    capacity      = %lu\t%s\n", stk->capacity,  ContainsError(stk->errors_bit, CapacityError)              ? "(BAD!)" : "");
    printf("    poison        = %d\n", stk->poison);

    #ifdef CANARY_ON
    printf("    data_canary   = %x\n", stk->data_canary_v);
    printf("    stack_canary  = %x\n", stk->data_canary_v);
    #endif //canary

    #ifdef HASH_ON
    printf("    hash     = %llu\t%s\n", stk->hash_value,ContainsError(stk->errors_bit, HashError)                 ? "(BAD!)" : "");
    #endif
    printf("    data[%p]\t%s", stk->data,              ContainsError(stk->errors_bit, NullptrDataError) ||
                                                       ContainsError(stk->errors_bit, PoisonDataError)  ||
                                                       ContainsError(stk->errors_bit, PoisonFillingError)         ? "(BAD!)" : "");
    if (!(ContainsError(stk->errors_bit, NullptrDataError) || ContainsError(stk->errors_bit, CapacityError))) {

        #ifdef CANARY_ON
        printf(" {\n         [canary] = %x\t%s\n", *((stk->data_canary_p).canary1),
                                                       ContainsError(stk->errors_bit, DataCanaryError)            ? "(BAD!)" : "");
        #endif

        for (size_t index = 0; index < stk->capacity; index++) {
            const char* is_poison = "";
            const char* is_filled = "*";
            if (index >= stk->size) is_filled = " ";
            stack_type element = (stk->data)[index];
            if (element == stk->poison) is_poison = "(poison)";
             printf("        %s [%lu] = %d %s\n", is_filled, index, element, is_poison);
        }
        #ifdef CANARY_ON
        printf("         [canary] = %x\t%s", *((stk->data_canary_p).canary2),
                                                      ContainsError(stk->errors_bit, DataCanaryError)             ? "(BAD!)" : "");
        #endif
        printf("\n    }");
    }

    #ifdef CANARY_ON
    printf("\n    stack_canary2 = %x\t%s\n", stk->stack_canary2, ContainsError(stk->errors_bit, StackCanaryError)     ? "(BAD!)" : "");
    #endif
    printf("\n}\n\n");
}

void StkDtor(StackInfo* stk) {

    ASSERT_OK(stk);

    #ifdef _DEBUG
    free((stk->data_canary_p).canary1);
    #else
    free(stk->data);
    #endif

}

void FillPoison(StackInfo* stk) {

    for (size_t index = stk->size; index < stk->capacity; index++) {
        (stk->data)[index] = stk->poison;
    }

}

uint64_t CountHash(StackInfo* stk) {

    assert(stk);

    uint64_t hash = 5381;

    for (size_t index = 0; index < stk->size; index++) {
        hash = (hash << 5) + hash + (stk->data)[index];
    }

    return hash;
}

bool ContainsError(error_code code, Stack_Err_t err) {
    return err & code;
}

