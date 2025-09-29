#include <stdlib.h>
#include <stdint.h>

#ifndef STACK_H
#define STACK_H

typedef int stack_type;
typedef uint64_t error_code;

enum Stack_Err_t {
    Ok                        =      0,
    AllocationError           = 1 << 1,
    SizeError                 = 1 << 2,
    CapacityError             = 1 << 3,
    SizeMoreThanCapacityError = 1 << 4,
    PopSizeStackError         = 1 << 5,
    PoisonDataError           = 1 << 6,
    WrongPoisonFillingError   = 1 << 7,
    Error                     = 1 << 8,
};

#ifdef _DEBUG
struct BirthInfo {
    const char* file;
    const char* func;
    const char* name;
    size_t      line;
};

struct Canary_p {
    stack_type* canary1;
    stack_type* canary2;
};

#define INIT_STACK(stk, capacity) do {                          \
    BirthInfo info_got = {__FILE__, __func__, #stk, __LINE__};  \
    StackCtor(&stk, capacity, &info_got);                       \
} while(0)

#else

#define INIT_STACK(stk, capacity) do {                          \
    BirthInfo info_got = {};                                    \
    StackCtor(&stk, capacity, &info_got);                       \
} while(0)

#endif



struct StackInfo {
    stack_type* data      = 0;
    size_t      size      = 0;
    size_t      capacity  = 0;

    #ifdef _DEBUG
    BirthInfo* info       = nullptr;
    stack_type poison     = 0x00D1127000;
    Canary_p   canary_p   = {};
    stack_type canary_v   = 0x00FA7B112D;
    error_code errors_bit = 0;
    uint64_t hash_value   = 0;
    #endif
};

#ifdef _DEBUG
Stack_Err_t StkErr(StackInfo* stk);
void StkDump(StackInfo* stk);
void FillPoison(StackInfo* stk);
uint64_t CountHash(StackInfo* stk);
#endif



Stack_Err_t StackCtor(StackInfo* stk, size_t capacity, BirthInfo* info);
Stack_Err_t StackPush(StackInfo* stk, stack_type element);
Stack_Err_t StackPop(StackInfo* stk);
void StkDtor(StackInfo* stk);

#endif
