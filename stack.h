#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

static const size_t MAX_SIZE_VALUE = 0x00011A6AAD;

#define HASH_ON
#define CANARY_ON

#ifndef STACK_H
#define STACK_H

typedef int stack_type;
typedef int error_code;

enum Stack_Err_t {
    Ok                        =      0,//
    AllocationError           = 1 << 1,
    PopSizeStackError         = 1 << 2,
    #ifdef _DEBUG
    StackCanaryError          = 1 << 3,
    NullptrDataError          = 1 << 4,
    SizeError                 = 1 << 5,//
    CapacityError             = 1 << 6,//
    SizeMoreThanCapacityError = 1 << 7,//
    PoisonDataError           = 1 << 8,//
    PoisonFillingError        = 1 << 9,//
    DataCanaryError           = 1 << 10,//
    HashError                 = 1 << 11,//
    #endif
};

#ifdef _DEBUG
struct BirthInfo {
    const char* file;
    const char* func;
    const char* name;
    int         line;
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
    StackCtor(&stk, capacity);                                  \
} while(0)

#endif



struct StackInfo {

    #ifdef _DEBUG
    stack_type stack_canary1  = 0x00FA7B012D;
    #endif

    stack_type* data          = 0;
    size_t     size           = 0;
    size_t     capacity       = 0;

    #ifdef _DEBUG
    BirthInfo* info           = nullptr;
    stack_type poison         = 0x00000D1127;
    Canary_p   data_canary_p  = {};//
    stack_type data_canary_v  = 0x00FA7B112D;
    stack_type stack_canary_v = 0x00FA7B012D;
    error_code errors_bit     = 0;

    #ifdef HASH_ON
    uint64_t hash_value       = 5381;
    #endif // HASH_ON

    #endif // _DEBUG

    #ifdef _DEBUG
    stack_type stack_canary2  = 0x00FA7B012D;
    #endif
};

#ifdef _DEBUG
error_code StkErr(StackInfo* stk);
void ASSERT_OK(StackInfo *stk);
void StkDump(StackInfo* stk);
void FillPoison(StackInfo* stk);
uint64_t CountHash(StackInfo* stk);
bool ContainsError(error_code code, Stack_Err_t err);
#endif



error_code StackCtor(StackInfo* stk, size_t capacity, ...);
error_code StackPush(StackInfo* stk, stack_type element);
error_code StackPop(StackInfo* stk, stack_type* element);
void StkDtor(StackInfo* stk);

#endif
